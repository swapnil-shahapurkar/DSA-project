# README.md

## Project Overview
This project is a C-based application designed to assist users in finding the shortest path between cities and to provide information on hotels, lodges, and restaurants in those cities. The application utilizes Dijkstra's algorithm for pathfinding and incorporates various string matching algorithms to enhance user interaction and data retrieval.

## Features
- **Shortest Path Calculation**: Implements Dijkstra's algorithm to find the shortest distance between two cities based on a given adjacency matrix.
- **Data Loading**: Loads city names, distances, and adjacency matrices from text files.
- **Information Retrieval**: Retrieves and displays information about hotels, lodges, and restaurants based on user input.
- **Sorting and Filtering**: Allows users to sort accommodations based on price or rating and filter results within specified price ranges.

## Algorithms Used

### 1. Dijkstra's Algorithm
Dijkstra's algorithm is used for finding the shortest path between nodes in a graph. In this application:
- The algorithm initializes distances from the source city to all other cities as infinite and sets the distance to the source city as zero.
- It iteratively selects the city with the smallest distance that has not yet been processed, updates the distances of its neighbors, and keeps track of the path using a parent array.

### 2. String Matching Algorithms
The application employs various string matching algorithms to facilitate user queries:

#### a. **Bad Character Rule (Boyer-Moore Algorithm)**
This algorithm is utilized in the `checklist` function to efficiently search for city names:
- It preprocesses the pattern (city name) by creating a bad character table that records the last occurrence of each character in the pattern.
- During the search, it skips sections of text based on mismatches, improving search efficiency.

#### b. **Case-Insensitive Comparison**
Functions like `strcmpi` are used throughout the code to ensure that comparisons between user input and stored city names are case-insensitive, enhancing user experience.

### 3. Merge Sort
Merge sort is used for sorting accommodations:
- The `mergesorthotel` function sorts hotel data based on ratings or prices. It divides the array into halves, recursively sorts each half, and merges them back together in sorted order.

### 4. Linear Search
In functions like `load_hotels`, `load_lodges`, and `load_restaurants`, linear search is employed to filter results based on user input:
- This approach iterates through all entries to find matches with specified criteria (e.g., location).

## File Structure
The application consists of the following key components:

### Data Files
- `adjacency_matrix.txt`: Contains the adjacency matrix representing the connections between cities.
- `distance_matrix.txt`: Contains the distance matrix for calculating shortest paths.
- `cities_list.txt`: Lists cities with their corresponding IDs.
- `hotels.txt`: Contains information about various hotels including name, location, rating, price, phone number, and email.
- `lodges.txt`: Similar to hotels but for lodges.
- `restaurants.txt`: Contains details about restaurants including name, location, type, rating, range, and phone number.

### Source Code
- `main.c`: The main source file containing all function definitions and logic for loading data, processing user input, and displaying results.

## Key Functions
1. **Data Loading Functions**
   - `load_distance()`: Loads the distance matrix from a file.
   - `load_adjacency()`: Loads the adjacency matrix from a file.
   - `load_cities()`: Loads city names from a file.
   - `load_hotels()`, `load_lodges()`, `load_restaurants()`: Load respective accommodation data.

2. **Pathfinding**
   - `dijkstra(int src, int dest)`: Implements Dijkstra's algorithm to find the shortest path between two cities.

3. **User Interaction**
   - `KTM()`: Prompts the user for source and destination cities and initiates the pathfinding process.
   - Various filtering functions for hotels and lodges that allow sorting by price or rating.

4. **Utility Functions**
   - Helper functions for string manipulation and data formatting (e.g., `replaceCharWithSpace()`).

## Compilation Instructions
To compile the program:
```bash
gcc main.c -o travel_assistant
```

## Usage Instructions
1. Ensure all required data files are present in the same directory as the executable.
2. Run the program:
```bash
./travel_assistant
```
3. Follow on-screen prompts to enter source and destination cities or to retrieve accommodation information.

## Example Usage
1. Enter a source city when prompted.
2. Enter a destination city.
3. View the shortest distance and path details displayed on the console.
4. Access hotel or lodge information by entering relevant queries.

## Conclusion
This application serves as a comprehensive tool for travelers seeking efficient routes between cities while also providing essential accommodation options. By incorporating advanced algorithms for pathfinding and string matching, it is designed to be user-friendly with straightforward prompts guiding users through its features.

