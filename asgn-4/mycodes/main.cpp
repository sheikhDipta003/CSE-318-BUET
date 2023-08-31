#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <unordered_set>
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

int N = 6;  //total number of attributes

// Sample data structure representing a dataset
struct DataSample {
    vector<int> features;
    int label;
};

// Recursive function to build the decision tree
struct TreeNode {
    int attributeIndex;
    int label;
    unordered_map<int, TreeNode*> children;
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

DataSample parseCSVRow(const string& row);
double calculateEntropy(const vector<DataSample>& dataset);
double calculateInformationGain(const vector<DataSample>& dataset,int attributeIndex);
TreeNode* buildDecisionTree(const std::vector<DataSample>& dataset, std::unordered_set<int>& usedAttributes);
int pluralityLabel(const std::vector<DataSample>& dataset);
int classifySample(const TreeNode* node, const vector<int>& sample);



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
    // vector<string> attributeNames = {"buying", "maint", "doors", "persons", "lug_boot", "safety"};
    int numExperiments = 20;
    vector<double> accuracies;

    for (int i = 0; i < numExperiments; i++) {
        // Shuffle the dataset and split into training and testing sets
        random_shuffle(dataset.begin(), dataset.end());
        size_t trainSize = dataset.size() * 0.8;
        vector<DataSample> trainSet(dataset.begin(), dataset.begin() + trainSize);
        vector<DataSample> testSet(dataset.begin() + trainSize, dataset.end());

        std::unordered_set<int> usedAttributes;
        TreeNode* root = buildDecisionTree(trainSet, usedAttributes);

        // Testing the decision tree on the testing data
        int correctPredictions = 0;
        for (const DataSample& sample : testSet) {
            int predictedLabel = classifySample(root, sample.features);
            if (predictedLabel == sample.label) {
                correctPredictions++;
            }
        }

        double accuracy = static_cast<double>(correctPredictions) / testSet.size() * 100.0;
        // cout << "Experiment no. " << i << ", Test Accuracy: " << accuracy << "%" << endl;
        accuracies.push_back(accuracy);
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

    cout << "\nAverage Test Accuracy: " << averageAccuracy << "%" << endl;
    cout << "Standard Deviation of Accuracy: " << standardDeviation << endl << endl;

    return 0;
}

// Function to calculate entropy
double calculateEntropy(const vector<DataSample>& dataset) {
    unordered_map<int, int> labelCounts;

    //counts how many samples are there corresponding to a particular class label
    for (const DataSample& sample : dataset) {
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
double calculateInformationGain(const vector<DataSample>& dataset, int attributeIndex) {
    unordered_map<int, vector<DataSample>> splitData;

    //example: if attributeIndex==2 [doors], then this loop will trace how many samples have 2 doors, then add those samples to the vector under key '2' in the unordered_map. Similarly, it traces how many samples have 4 doors, then adds them into the vector under the key '4' and so on.
    for (const DataSample& sample : dataset) {
        splitData[sample.features[attributeIndex]].push_back(sample);
    }

    double weightedEntropy = 0.0;
    for (const auto& entry : splitData) {
        double weight = static_cast<double>(entry.second.size()) / dataset.size();
        double entropy = calculateEntropy(entry.second);
        weightedEntropy += weight * entropy;
    }

    return calculateEntropy(dataset) - weightedEntropy;
}

TreeNode* buildDecisionTree(const std::vector<DataSample>& dataset, std::unordered_set<int>& usedAttributes) {
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

    //find the best attr to expand based on the max info gain
    double maxInformationGain = -1.0;
    int bestAttribute = -1;

    for (int i = 0; i < N; i++) {
        if (usedAttributes.find(i) == usedAttributes.end()) {
            double informationGain = calculateInformationGain(dataset, i);
            if (informationGain > maxInformationGain) {
                maxInformationGain = informationGain;
                bestAttribute = i;
            }
        }
    }

    if (bestAttribute == -1) {
        // No more available attributes to split on
        // Choose pluralityLabel of remaining examples as the node's label
        node->label = pluralityLabel(dataset);
        return node;
    }

    node->attributeIndex = bestAttribute;
    usedAttributes.insert(bestAttribute);

    unordered_map<int, vector<DataSample>> splitData;
    //list of all the samples with that have a particular value of the chosen 'bestAttribute' in the corresponding column

    for (const auto& sample : dataset) {
        splitData[sample.features[bestAttribute]].push_back(sample);
    }

    for (const auto& entry : splitData) {
        node->children[entry.first] = buildDecisionTree(entry.second, usedAttributes);
    }

    return node;
}

// Function to find the majority label in a dataset
int pluralityLabel(const std::vector<DataSample>& dataset) {
    std::unordered_map<int, int> labelCounts;
    for (const auto& sample : dataset) {
        labelCounts[sample.label]++;
    }

    int majorityLabel = -1;
    int maxCount = -1;

    for (const auto& labelCount : labelCounts) {
        if (labelCount.second > maxCount) {
            maxCount = labelCount.second;
            majorityLabel = labelCount.first;
        }
    }

    return majorityLabel;
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
        return node->label;
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
