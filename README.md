## Sports Complex Planning Project

### Overview

This repository contains the implementation of an optimization project aimed at efficiently allocating zones to locations within a newly planned sports complex in Delhi. The objective is to minimize the total walking time for sports enthusiasts and visitors. The project utilizes a Greedy Hill Climbing algorithm with Random Restart to achieve this goal.

### Problem Statement

In the sports complex, various zones, such as basketball courts, table tennis rooms, gyms, and swimming pools, need to be allocated to specific locations. The allocation should consider the number of daily walks between different zones and the time taken to walk between locations. The project aims to find an optimal allocation that reduces the overall walking time within the complex.

### Input

The project takes input in the form of a configuration file, including:
- Total processing time available in minutes.
- Number of zones (z).
- Number of locations (l).
- Matrices N (representing the number of daily walks between zones) and T (representing the time taken to walk between locations).

### Output

The output is a sequence of Z unique numbers, each less than or equal to L, indicating the zone-location mapping. For example:
```
2 4 1
```
This represents zone 1 in location 2, zone 2 in location 4, and zone 3 in location 1, with a total walking time of 30 minutes for this allocation.

### Approach

The project implements a Greedy Hill Climbing algorithm with Random Restart to find an optimal solution. The approach involves:
1. Initializing the allocation randomly.
2. Evaluating the total walking time for the current allocation.
3. Exploring neighboring allocations by swapping zone locations and calculating walking times.
4. Selecting the neighboring allocation with the least walking time.
5. Iteratively minimizing walking time and implementing random restarts to escape local optima.

### How to Run

To run the project, execute the following command in the terminal:
```
./run.sh input.txt output.txt
```
This reads the input from `input.txt` and outputs the allocation result to `output.txt`.

### Evaluation

The project's performance is evaluated based on the quality of the allocations generated. The goal is to minimize walking time for various test inputs.

### Language Used

The project is implemented in C++.

### Collaborators

- Sharad Kumar (https://github.com/sharad-kr/)
- Rishabh Verma (https://github.com/rish106/)



---

Feel free to explore the code and experiment with different input scenarios to observe the algorithm's performance in optimizing the sports complex zone allocation.
