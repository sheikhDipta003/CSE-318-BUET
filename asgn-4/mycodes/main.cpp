#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <algorithm>
#include <numeric>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// Attribute Values:
//    buying       v-high (3), high (2), med (1), low (0)
//    maint        v-high (3), high (2), med (1), low (0)
//    doors        2 (0), 3 (1), 4 (2), 5more (3)
//    persons      2 (0), 4 (2), more (4)
//    lug_boot     small (0), med (1), big (2)
//    safety       low (0), med (1), high (2)

// class labels:
// unacc (0), acc (1), good (2), vgood (3)


// Sample data structure representing a dataset
struct DataSample {
    vector<int> features;
    int label;
};

// Recursive function to build the decision tree
struct TreeNode {
    int attributeIndex;
    int label;
    map<int, TreeNode*> children;
};

// prints all elements of any container in given outputstream
template <typename Container>
void showElements(const Container& container, std::ostream& os = std::cout){
    os << "{";
    auto it = container.begin();
    if (it != container.end()) {
        os << *it;
        it++;
    }
    for (; it != container.end(); it++) {
        os << ", " << *it;
    }
    os << "}";
}

double calculateEntropy(const vector<DataSample>& dataset);
double calculateInformationGain(const vector<DataSample>& dataset,int attributeIndex,int totalAttributes);
int findBestAttribute(const vector<DataSample>& dataset, int totalAttributes);
TreeNode* buildDecisionTree(const vector<DataSample>& dataset,const vector<string>& attributeNames,int totalAttributes);
int classifySample(const TreeNode* node, const vector<int>& sample);
DataSample parseCSVRow(const string& row);



int main() {
    // Sample dataset
    ifstream inFile("car.data");
    string line;
    vector<DataSample> dataset;

    while (getline(inFile, line)) {
        DataSample sample = parseCSVRow(line);
        // showElements(sample.features);
        // cout << sample.label << endl;
        // cout << endl;
        dataset.push_back(sample);
    }

    // Attributes
    vector<string> attributeNames = {"buying", "maint", "doors", "persons", "lug_boot", "safety"};
    int totalAttributes = attributeNames.size(), numExperiments = 20;
    vector<double> accuracies;

    for (int experiment = 0; experiment < numExperiments; ++experiment) {
        // Shuffle the dataset and split into training and testing sets
        random_shuffle(dataset.begin(), dataset.end());
        size_t trainSize = dataset.size() * 0.8;
        vector<DataSample> trainingSet(dataset.begin(), dataset.begin() + trainSize);
        vector<DataSample> testingSet(dataset.begin() + trainSize, dataset.end());

        TreeNode* root = buildDecisionTree(trainingSet, attributeNames, totalAttributes);

        // Testing the decision tree on the testing data
        int correctPredictions = 0;
        for (const auto& sample : testingSet) {
            int predictedLabel = classifySample(root, sample.features);
            if (predictedLabel == sample.label) {
                correctPredictions++;
            }
        }

        double accuracy = static_cast<double>(correctPredictions) / testingSet.size() * 100.0;
        accuracies.push_back(accuracy);

        // TODO: Free memory by deallocating the decision tree nodes
    }

    // Calculate and report average accuracy, mean accuracy, and standard deviation
    double sumAccuracies = accumulate(accuracies.begin(), accuracies.end(), 0.0);
    double averageAccuracy = sumAccuracies / numExperiments;

    double squaredDifferencesSum = 0.0;
    for (double accuracy : accuracies) {
        squaredDifferencesSum += (accuracy - averageAccuracy) * (accuracy - averageAccuracy);
    }
    double variance = squaredDifferencesSum / numExperiments;
    double standardDeviation = sqrt(variance);

    cout << "Average Test Accuracy: " << averageAccuracy << "%" << endl;
    cout << "Standard Deviation of Accuracy: " << standardDeviation << endl;

    return 0;
}

// Function to calculate entropy
double calculateEntropy(const vector<DataSample>& dataset) {
    map<int, int> labelCounts;
    for (const auto& sample : dataset) {
        labelCounts[sample.label]++;
    }

    double entropy = 0.0;
    for (const auto& labelCount : labelCounts) {
        double probability = static_cast<double>(labelCount.second) / dataset.size();
        entropy -= probability * log2(probability);
    }

    return entropy;
}

// Function to calculate information gain
double calculateInformationGain(const vector<DataSample>& dataset,
                                 int attributeIndex,
                                 int totalAttributes) {
    map<int, vector<DataSample>> splitData;

    for (const auto& sample : dataset) {
        splitData[sample.features[attributeIndex]].push_back(sample);
    }

    double weightedEntropy = 0.0;
    for (const auto& entry : splitData) {
        double probability = static_cast<double>(entry.second.size()) / dataset.size();
        double entropy = calculateEntropy(entry.second);
        weightedEntropy += probability * entropy;
    }

    return calculateEntropy(dataset) - weightedEntropy;
}

// Function to find the best attribute to split on
int findBestAttribute(const vector<DataSample>& dataset, int totalAttributes) {
    double maxInformationGain = -1.0;
    int bestAttribute = -1;

    for (int attributeIndex = 0; attributeIndex < totalAttributes; ++attributeIndex) {
        double informationGain = calculateInformationGain(dataset, attributeIndex, totalAttributes);
        if (informationGain > maxInformationGain) {
            maxInformationGain = informationGain;
            bestAttribute = attributeIndex;
        }
    }

    return bestAttribute;
}

TreeNode* buildDecisionTree(const vector<DataSample>& dataset,
                            const vector<string>& attributeNames,
                            int totalAttributes) {
    TreeNode* node = new TreeNode();

    // Check if all samples have the same label
    bool sameLabel = true;
    int firstLabel = dataset[0].label;
    for (const auto& sample : dataset) {
        if (sample.label != firstLabel) {
            sameLabel = false;
            break;
        }
    }

    if (sameLabel) {
        node->label = firstLabel;
        return node;
    }

    int bestAttribute = findBestAttribute(dataset, totalAttributes);
    node->attributeIndex = bestAttribute;

    map<int, vector<DataSample>> splitData;
    for (const auto& sample : dataset) {
        splitData[sample.features[bestAttribute]].push_back(sample);
    }

    for (const auto& entry : splitData) {
        node->children[entry.first] = buildDecisionTree(entry.second, attributeNames, totalAttributes);
    }

    return node;
}

// Function to classify a sample using the decision tree
int classifySample(const TreeNode* node, const vector<int>& sample) {
    if (node->children.empty()) {
        return node->label;
    }

    int attributeValue = sample[node->attributeIndex];
    if (node->children.find(attributeValue) != node->children.end()) {
        return classifySample(node->children.at(attributeValue), sample);
    } else {
        // Handle missing attribute values
        return -1; // Return a special value for handling missing branches
    }
}


DataSample parseCSVRow(const string& row) {
    istringstream ss(row);
    string token;
    DataSample sample;
    
    while (getline(ss, token, ',')) {
        if (ss.peek() == '\n') {
            token.erase(remove(token.begin(), token.end(), '\n'), token.end());
        }
        if (ss.peek() == '\r') {
            token.erase(remove(token.begin(), token.end(), '\r'), token.end());
        }

        if(token == "low" || token == "small" || token == "2"){
            sample.features.push_back(0);
        }
        else if(token == "med" || token == "3"){
            sample.features.push_back(1);
        }
        else if(token == "high" || token == "big" || token == "4"){
            sample.features.push_back(2);
        }
        else if(token == "v-high" || token == "5more"){
            sample.features.push_back(3);
        }
        else if(token == "more"){
            sample.features.push_back(4);
        }
        else if(token =="unacc"){
            sample.label = 0;
        }
        else if(token =="acc"){
            sample.label = 1;
        }
        else if(token =="good"){
            sample.label = 2;
        }
        else if(token =="vgood"){
            sample.label = 3;
        }
    }

    return sample;
}
