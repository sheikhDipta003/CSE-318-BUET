import heapq

# Define the n-puzzle solver using A* search algorithm
def solve_n_puzzle(start_state):
    goal_state = tuple(range(1, len(start_state) + 1))
    open_set = [(0, start_state)]
    came_from = {}
    g_score = {start_state: 0}
    f_score = {start_state: heuristic(start_state, goal_state)}
    explored_nodes = 0
    expanded_nodes = 0

    while open_set:
        current = heapq.heappop(open_set)[1]
        explored_nodes += 1

        if current == goal_state:
            return reconstruct_path(came_from, current), g_score[current], explored_nodes, expanded_nodes

        for neighbor in get_neighbors(current):
            expanded_nodes += 1
            tentative_g_score = g_score[current] + 1

            if neighbor not in g_score or tentative_g_score < g_score[neighbor]:
                came_from[neighbor] = current
                g_score[neighbor] = tentative_g_score
                f_score[neighbor] = tentative_g_score + heuristic(neighbor, goal_state)
                heapq.heappush(open_set, (f_score[neighbor], neighbor))

    return None, None, explored_nodes, expanded_nodes

# Heuristic function: Manhattan distance
def heuristic(state, goal_state):
    n = int(len(state) ** 0.5)
    distance = 0

    for i in range(n):
        for j in range(n):
            value = state[i * n + j]
            if value != 0:
                target_x = (value - 1) % n
                target_y = (value - 1) // n
                distance += abs(i - target_y) + abs(j - target_x)

    return distance

# Get neighboring states by moving the blank tile
def get_neighbors(state):
    n = int(len(state) ** 0.5)
    blank_index = state.index(0)
    neighbors = []

    if blank_index % n > 0:
        left_neighbor = list(state)
        left_neighbor[blank_index], left_neighbor[blank_index - 1] = left_neighbor[blank_index - 1], left_neighbor[blank_index]
        neighbors.append(tuple(left_neighbor))

    if blank_index % n < n - 1:
        right_neighbor = list(state)
        right_neighbor[blank_index], right_neighbor[blank_index + 1] = right_neighbor[blank_index + 1], right_neighbor[blank_index]
        neighbors.append(tuple(right_neighbor))

    if blank_index >= n:
        up_neighbor = list(state)
        up_neighbor[blank_index], up_neighbor[blank_index - n] = up_neighbor[blank_index - n], up_neighbor[blank_index]
        neighbors.append(tuple(up_neighbor))

    if blank_index < len(state) - n:
        down_neighbor = list(state)
        down_neighbor[blank_index], down_neighbor[blank_index + n] = down_neighbor[blank_index + n], down_neighbor[blank_index]
        neighbors.append(tuple(down_neighbor))

    return neighbors

# Reconstruct the path from start to goal state
def reconstruct_path(came_from, current):
    total_path = [current]
    while current in came_from:
        current = came_from[current]
        total_path.append(current)
    total_path.reverse()
    return total_path

# Check if the puzzle is solvable
def is_solvable(state):
    inversions = 0
    n = len(state)

    for i in range(n):
        for j in range(i + 1, n):
            if state[i] != 0 and state[j] != 0 and state[i] > state[j]:
                inversions += 1

    blank_row = n - (state.index(0) // int(n ** 0.5))  # Zero-based row index of the blank tile from bottom
    if n % 2 == 0:  # Even-sized puzzle
        if blank_row % 2 == 0:
            return inversions % 2 == 0
        else:
            return inversions % 2 != 0
    else:  # Odd-sized puzzle
        return inversions % 2 == 0

# Example usage
start_state = (1, 0, 3, 4, 2, 5, 7, 8, 6)  # Replace with your desired start state

if is_solvable(start_state):
    path, optimal_cost, explored_nodes, expanded_nodes = solve_n_puzzle(start_state)
    if path:
        print("Solution found!")
        print("Optimal Cost:", optimal_cost)
        print("Steps:")
        for i, state in enumerate(path):
            print("Step", i + 1)
            print(state)
            print("--------------")
        print("Explored Nodes:", explored_nodes)
        print("Expanded Nodes:", expanded_nodes)
    else:
        print("No solution found.")
else:
    print("The puzzle is not solvable.")
