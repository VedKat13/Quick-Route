#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include<stdbool.h>
#include "header.h"
// Function to create a new MinHeap node
MinHeapNode *newMinHeapNode(int vertex, int distance)
{
    // Allocate memory for a new MinHeapNode and assign the vertex and distance values
    MinHeapNode *node = (MinHeapNode *)malloc(sizeof(MinHeapNode));
    node->vertex = vertex;
    node->distance = distance;
    return node;
}

// Function to create a MinHeap with a given capacity
MinHeap *createMinHeap(int capacity)
{
    // Allocate memory for the MinHeap and initialize its properties
    MinHeap *minHeap = (MinHeap *)malloc(sizeof(MinHeap));
    minHeap->size = 0;  // Initialize heap size as 0
    minHeap->capacity = capacity;  // Set the heap capacity
    minHeap->position = (int *)malloc(capacity * sizeof(int));  // Initialize position array
    minHeap->array = (MinHeapNode **)malloc(capacity * sizeof(MinHeapNode *));  // Initialize array of nodes
    return minHeap;
}

// Swap function for heap nodes (helper function for heapify)
void swapMinHeapNode(MinHeapNode **a, MinHeapNode **b)
{
    // Swap the nodes in the heap
    MinHeapNode *temp = *a;
    *a = *b;
    *b = temp;
}

// Min-Heapify function to maintain the min-heap property
void minHeapify(MinHeap *minHeap, int idx)
{
    int smallest = idx;
    int left = 2 * idx + 1;  // Left child index
    int right = 2 * idx + 2; // Right child index

    // Compare with left child
    if (left < minHeap->size && minHeap->array[left]->distance < minHeap->array[smallest]->distance)
        smallest = left;

    // Compare with right child
    if (right < minHeap->size && minHeap->array[right]->distance < minHeap->array[smallest]->distance)
        smallest = right;

    // If the smallest is not the current index, swap and heapify
    if (smallest != idx)
    {
        // Swap positions in the heap
        MinHeapNode *smallestNode = minHeap->array[smallest];
        MinHeapNode *idxNode = minHeap->array[idx];

        // Update the position of swapped nodes
        minHeap->position[smallestNode->vertex] = idx;
        minHeap->position[idxNode->vertex] = smallest;

        // Swap nodes in the heap
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

        // Recursively heapify the affected sub-tree
        minHeapify(minHeap, smallest);
    }
}

// Extract the minimum node (root) from the heap and re-heapify
MinHeapNode *extractMin(MinHeap *minHeap)
{
    if (minHeap->size == 0)
        return NULL;  // If heap is empty, return NULL

    // Get the root (minimum node)
    MinHeapNode *root = minHeap->array[0];
    MinHeapNode *lastNode = minHeap->array[minHeap->size - 1];

    // Move the last node to the root and update its position
    minHeap->array[0] = lastNode;
    minHeap->position[root->vertex] = minHeap->size - 1;
    minHeap->position[lastNode->vertex] = 0;

    // Decrease the heap size
    minHeap->size--;
    // Re-heapify the root to maintain min-heap property
    minHeapify(minHeap, 0);

    return root;  // Return the extracted root (min element)
}

// Function to assign a color based on the distance (used for visualization)
const char *getColor(int distance)
{
    if (distance < 10)
    {
        return GREEN;  // green for very close distances
    }
    if (distance < 20)
    {
        return YELLOW;  // yellow for medium distances
    }
    if (distance < 50)
    {
        return CYAN;  // cyan for larger distances
    }
    return RED;  // Color for very large distances
}

// Decrease the distance of a vertex in the heap and maintain heap property
void decreaseKey(MinHeap *minHeap, int vertex, int distance)
{
    int i = minHeap->position[vertex];  // Get the index of the vertex in the heap

    // Update the distance of the vertex
    minHeap->array[i]->distance = distance;

    // Move the vertex up the heap to maintain the min-heap property
    while (i && minHeap->array[i]->distance < minHeap->array[(i - 1) / 2]->distance)
    {
        // Update the position array with swapped indices
        minHeap->position[minHeap->array[i]->vertex] = (i - 1) / 2;
        minHeap->position[minHeap->array[(i - 1) / 2]->vertex] = i;

        // Swap the nodes in the heap
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);

        // Move to the parent node
        i = (i - 1) / 2;
    }
}

// Check if a vertex is present in the MinHeap
bool isInMinHeap(MinHeap *minHeap, int vertex)
{
    // If position is smaller than the heap size, the vertex is in the heap
    return minHeap->position[vertex] < minHeap->size;
}

// Read the CSV file to create the graph and places array
int readCSV(const char *filename, int graph[MAX][MAX], char places[MAX][MAX])
{
    FILE *file = fopen(filename, "r");  // Open the CSV file
    if (!file)
    {
        printf("Error: Could not open file %s\n", filename);  // Handle file opening failure
        return 0;  // Return 0 if file cannot be opened
    }

    char line[1024];
    int n = 0;

    // Read the first line (place names) and store them in the places array
    if (fgets(line, sizeof(line), file))
    {
        char *token = strtok(line, ",");
        while (token)
        {
            trim(token);  // Trim whitespace from token
            strcpy(places[n], token);  // Store the place name
            token = strtok(NULL, ",");  // Get the next token
            n++;
        }
    }

    // Read the adjacency matrix (graph distances) and store them in the graph array
    for (int row = 0; row < n; row++)
    {
        if (fgets(line, sizeof(line), file))
        {
            int col = 0;
            char *token = strtok(line, ",");
            while (token)
            {
                graph[row][col++] = atoi(token);  // Convert token to integer and store in the graph matrix
                token = strtok(NULL, ",");  // Get the next token
            }
        }
    }

    fclose(file);  // Close the file after reading
    return n;  // Return the number of places
}

// Write the updated CSV file
void writeCSV(const char *filename, int graph[MAX][MAX], char places[MAX][MAX], int n)
{
    // Open the file for writing
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        printf("Error: Could not open file %s for writing.\n", filename);
        return; // If file can't be opened, return from the function
    }

    // Write place names (first row)
    for (int i = 0; i < n; i++)
    {
        // Print each place name in the file
        fprintf(file, "%s", places[i]);
        if (i < n - 1)  // Don't add comma after the last place name
            fprintf(file, ",");
    }
    fprintf(file, "\n"); // Newline after the first row

    // Write the adjacency matrix (distances)
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            // Print each distance in the matrix
            fprintf(file, "%d", graph[i][j]);
            if (j < n - 1) // Don't add comma after the last element in the row
                fprintf(file, ",");
        }
        fprintf(file, "\n"); // Newline after each row of the matrix
    }

    fclose(file); // Close the file after writing
}

// Dijkstra's Algorithm with Path Tracking
void dijkstraWithPath(int graph[MAX][MAX], int n, int start, int end, char places[MAX][MAX], int dist[MAX], int parent[MAX]) {
    // Create a Min-Heap for efficiently finding the next vertex with the minimum distance
    MinHeap *minHeap = createMinHeap(n);

    // Initialize the distances and parents for each vertex
    for (int v = 0; v < n; v++) {
        dist[v] = INT_MAX;   // Set all distances to a large number (infinity)
        parent[v] = -1;      // Set parent as -1 for all vertices initially
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->position[v] = v; // Set position of each vertex in the heap
    }

    dist[start] = 0;  // Distance to the starting vertex is 0
    decreaseKey(minHeap, start, dist[start]); // Update the heap with the start vertex

    minHeap->size = n; // Set the size of the heap

    // Main loop to extract the minimum distance vertex and update its neighbors
    while (minHeap->size > 0) {
        MinHeapNode *minNode = extractMin(minHeap); // Extract the node with the smallest distance
        int u = minNode->vertex;

        // Update the distances for all neighboring vertices
        for (int v = 0; v < n; v++) {
            if (graph[u][v] && isInMinHeap(minHeap, v) && dist[u] != INT_MAX &&
                dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v]; // Update the distance for vertex v
                parent[v] = u; // Set parent of v to u
                decreaseKey(minHeap, v, dist[v]); // Update the heap
            }
        }
    }
}

// Dijkstra's Algorithm to find the shortest path
void dijkstra(int graph[MAX][MAX], int n, int start, int end, char places[MAX][MAX])
{
    int dist[MAX];    // Array to store distances from the start vertex
    int parent[MAX];  // Array to store the parent of each vertex for path reconstruction
    MinHeap *minHeap = createMinHeap(n);  // Create the Min-Heap

    // Initialize all distances to infinity and parents to -1
    for (int v = 0; v < n; v++)
    {
        dist[v] = INT_MAX;
        parent[v] = -1;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->position[v] = v; // Set the position of each vertex in the heap
    }

    dist[start] = 0;  // Distance to the start vertex is 0
    decreaseKey(minHeap, start, dist[start]); // Update the heap with the start vertex

    minHeap->size = n; // Set the heap size to the number of vertices

    // Main loop to extract the minimum distance vertex and update its neighbors
    while (minHeap->size > 0)
    {
        MinHeapNode *minNode = extractMin(minHeap); // Extract the node with the smallest distance
        int u = minNode->vertex;

        // Update the distances for all neighbors of u
        for (int v = 0; v < n; v++)
        {
            if (graph[u][v] && isInMinHeap(minHeap, v) && dist[u] != INT_MAX &&
                dist[u] + graph[u][v] < dist[v])
            {
                dist[v] = dist[u] + graph[u][v]; // Update the distance for vertex v
                parent[v] = u; // Set parent of v to u
                decreaseKey(minHeap, v, dist[v]); // Update the heap with the new distance
            }
        }
    }

    // If there is no path to the destination
    if (dist[end] == INT_MAX)
    {
        printf("No path exists from %s to %s.\n", places[start], places[end]);
        return; // Exit if no path exists
    }

    // Print the shortest path and its distance
    printf("\nShortest path from %s to %s:\n", places[start], places[end]);
    int path[MAX], pathLength = 0, temp = end;

    // Reconstruct the path from end to start
    while (temp != -1)
    {
        path[pathLength++] = temp;
        temp = parent[temp];
    }

    // Print the path with distances between each vertex
    for (int i = pathLength - 1; i > 0; i--)
    {
        int u = path[i], v = path[i - 1];
        printf("%s%s%s --(%s%d%skm)--> ", CYAN, places[u], RESET, getColor(graph[u][v]), graph[u][v], RESET);
    }
    printf("%s%s%s\n", CYAN, places[path[0]], RESET); // Print the destination vertex
    printf("Total Distance: %dkm\n", dist[end]); // Print the total distance of the path
}

// Function to find alternate path avoiding a given location
void findAlternatePath(int graph[MAX][MAX], int n, int start, int end, int avoid, char places[MAX][MAX])
{
    int dist[MAX];
    int parent[MAX];
    MinHeap *minHeap = createMinHeap(n);

    for (int v = 0; v < n; v++)
    {
        dist[v] = INT_MAX;
        parent[v] = -1;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->position[v] = v;
    }

    dist[start] = 0;
    decreaseKey(minHeap, start, dist[start]);
    minHeap->size = n;

    while (minHeap->size > 0)
    {
        MinHeapNode *minNode = extractMin(minHeap);
        int u = minNode->vertex;

        if (u == avoid)
            continue;

        for (int v = 0; v < n; v++)
        {
            if (v == avoid)
                continue;
            if (graph[u][v] && isInMinHeap(minHeap, v) && dist[u] != INT_MAX &&
                dist[u] + graph[u][v] < dist[v])
            {
                dist[v] = dist[u] + graph[u][v];
                parent[v] = u;
                decreaseKey(minHeap, v, dist[v]);
            }
        }
    }

    if (dist[end] == INT_MAX)
    {
        printf("No alternate path exists from %s to %s, avoiding %s.\n", places[start], places[end], places[avoid]);
        return;
    }

    printf("\nAlternate path from %s to %s avoiding %s:\n", places[start], places[end], places[avoid]);
    int temp = end, path[MAX], pathLength = 0;
    while (temp != -1)
    {
        path[pathLength++] = temp;
        temp = parent[temp];
    }

    for (int i = pathLength - 1; i > 0; i--)
    {
        int u = path[i], v = path[i - 1];
        int distance = graph[u][v];

        printf("%s%s%s --(%s%d%skm)--> ", CYAN, places[u], RESET, getColor(distance), distance, RESET);
    }
    printf("%s%s%s\n", CYAN, places[path[0]], RESET);
    printf("Total Distance: %dkm\n", dist[end]);
}
// Function to trim spaces from the start and end of the string
void trim(char *str)
{
    char *end;

    // Skip over any leading spaces
    while (isspace((unsigned char)*str)) // While the current character is a space, move to the next character
        str++;

    // Set end to point to the last character of the string
    end = str + strlen(str) - 1; // Calculate the pointer to the last character

    // Skip over any trailing spaces
    while (end > str && isspace((unsigned char)*end)) // While there are spaces at the end
        end--; // Move backward

    // Null-terminate the string at the last non-space character
    *(end + 1) = '\0'; // Add a null terminator after the last non-space character
}


// Function to sort locations alphabetically and update the graph
void sortLocations(char places[MAX][MAX], int graph[MAX][MAX], int *n)
{
    // Loop through all the places and compare them
    for (int i = 0; i < *n - 1; i++) // Outer loop for sorting places
    {
        for (int j = i + 1; j < *n; j++) // Inner loop for comparing places
        {
            // If the current place is greater than the next place alphabetically
            if (strcmp(places[i], places[j]) > 0) // Compare places alphabetically
            {
                // Swap the place names
                char temp[100]; // Temporary array to hold one place name
                strcpy(temp, places[i]); // Copy place[i] to temp
                strcpy(places[i], places[j]); // Copy place[j] to place[i]
                strcpy(places[j], temp); // Copy temp to place[j]

                // Swap the corresponding distances in the graph
                for (int k = 0; k < *n; k++) // Loop through all distances in the graph
                {
                    int tempVal = graph[i][k]; // Store the distance from place[i] to place[k] in tempVal
                    graph[i][k] = graph[j][k]; // Swap the distance from place[i] to place[k] with place[j]
                    graph[j][k] = tempVal; // Swap the distance from place[j] to place[k]

                    tempVal = graph[k][i]; // Store the distance from place[k] to place[i] in tempVal
                    graph[k][i] = graph[k][j]; // Swap the distance from place[k] to place[i] with place[k] to place[j]
                    graph[k][j] = tempVal; // Swap the distance from place[k] to place[j]
                }
            }
        }
    }
}

// Function to display all available locations
void displayLocations(char places[MAX][MAX], int n)
{
    printf("\nAvailable Locations:\n"); // Print the heading for available locations
    for (int i = 0; i < n; i++) // Loop through all locations
    {
        // Print each location with a number
        printf("%d. %s\n", i + 1, places[i]); // Print location number and name
    }
    printf("\n"); // Print a newline after displaying all locations
}

// Function to find the index of a location in the places array
int findLocationIndex(char places[MAX][100], int size, const char *location)
{
    for (int i = 0; i < size; i++) // Loop through all locations
    {
        // If the location matches, return the index
        if (strcmp(places[i], location) == 0) // Compare location name with the current place
        {
            return i; // Found, return index
        }
    }
    // If not found, return -1
    return -1; // Not found
}

// Function to add a new path between two locations
void addPath(int graph[MAX][MAX], char places[MAX][100], int n)
{
    char place1[100], place2[100]; // Variables to hold the names of two locations
    int distance; // Variable to hold the distance between the two locations

    // Get the first location from user input
    printf("Enter the first location: ");
    fgets(place1, sizeof(place1), stdin); // Read the first location name
    trim(place1); // Trim any extra spaces from the input

    // Get the second location from user input
    printf("Enter the second location: ");
    fgets(place2, sizeof(place2), stdin); // Read the second location name
    trim(place2); // Trim any extra spaces from the input

    // Get the distance between the two locations
    printf("Enter the distance between %s and %s: ", place1, place2); // Ask for the distance
    scanf("%d", &distance); // Read the distance
    getchar(); // Consume the newline character left by scanf

    // Find the indices of the two locations in the places array
    int index1 = findLocationIndex(places, n, place1); // Find index of place1
    int index2 = findLocationIndex(places, n, place2); // Find index of place2

    // If any location doesn't exist, show an error
    if (index1 == -1 || index2 == -1) // If either index is not found
    {
        printf("Error: One or both locations do not exist.\n"); // Print error message
        return; // Exit the function
    }

    // Update the graph with the new distance (undirected path)
    graph[index1][index2] = distance; // Set the distance from place1 to place2
    graph[index2][index1] = distance; // Set the distance from place2 to place1

    // Confirm that the path has been added
    printf("Path added successfully between %s and %s with distance %d.\n", place1, place2, distance); // Print success message
}

// Function to delete a path between two locations
void deletePath(int graph[MAX][MAX], char places[MAX][100], int n)
{
    char place1[100], place2[100]; // Variables to hold the names of two locations

    // Get the first location from user input
    printf("Enter the first location: ");
    fgets(place1, sizeof(place1), stdin); // Read the first location name
    trim(place1); // Trim any extra spaces from the input

    // Get the second location from user input
    printf("Enter the second location: ");
    fgets(place2, sizeof(place2), stdin); // Read the second location name
    trim(place2); // Trim any extra spaces from the input

    // Find the indices of the locations
    int index1 = findLocationIndex(places, n, place1); // Find index of place1
    int index2 = findLocationIndex(places, n, place2); // Find index of place2

    // If any location doesn't exist, show an error
    if (index1 == -1 || index2 == -1) // If either index is not found
    {
        printf("Error: One or both locations do not exist.\n"); // Print error message
        return; // Exit the function
    }

    // Set the distance to 0 to indicate no path
    graph[index1][index2] = 0; // Set the distance from place1 to place2 to 0
    graph[index2][index1] = 0; // Set the distance from place2 to place1 to 0

    // Confirm that the path has been deleted
    printf("Path deleted successfully between %s and %s.\n", place1, place2); // Print success message
}

// Add a new location to the graph and update the places array
void addLocation(int graph[MAX][MAX], char places[MAX][MAX], int *n)
{
    if (*n >= MAX) // Check if the maximum number of locations is reached
    {
        printf("Maximum locations reached.\n");
        return;
    }

    char newPlace[100];
    printf("Enter the name of the new location: ");
    fgets(newPlace, sizeof(newPlace), stdin);
    trim(newPlace); // Remove extra spaces or newlines

    strcpy(places[*n], newPlace); // Add the new location to the places array

    // Initialize distances for the new location
    for (int i = 0; i <= *n; i++)
    {
        graph[*n][i] = graph[i][*n] = 0;
    }

    (*n)++; // Increment the number of locations
    sortLocations(places, graph, n); // Keep locations sorted for consistency
}

// Delete a location from the graph and places array
void deleteLocation(int graph[MAX][MAX], char places[MAX][MAX], int *n)
{
    char placeToDelete[100];
    printf("Enter the name of the location to delete: ");
    fgets(placeToDelete, sizeof(placeToDelete), stdin);
    trim(placeToDelete);

    // Find the index of the location to delete
    int index = -1;
    for (int i = 0; i < *n; i++)
    {
        if (strcmp(places[i], placeToDelete) == 0)
        {
            index = i;
            break;
        }
    }

    if (index == -1) // Location not found
    {
        printf("Location not found.\n");
        return;
    }

    // Shift locations in the places array to remove the deleted location
    for (int i = index; i < *n - 1; i++)
    {
        strcpy(places[i], places[i + 1]);
    }

    // Shift rows and columns in the adjacency matrix
    for (int i = index; i < *n - 1; i++)
    {
        for (int j = 0; j < *n; j++)
        {
            graph[i][j] = graph[i + 1][j];
        }
    }

    for (int j = index; j < *n - 1; j++)
    {
        for (int i = 0; i < *n - 1; i++)
        {
            graph[i][j] = graph[i][j + 1];
        }
    }

    (*n)--; // Decrease the number of locations
    sortLocations(places, graph, n); // Maintain sorted order
}

// Display saved journeys from a CSV file
void displaySavedJourneys() {
    FILE *file = fopen("temp.csv", "r");
    if (!file) // Check if the file exists
    {
        printf("No saved journeys found.\n");
        return;
    }

    char line[1024]; // Buffer to read each line
    int journeyNumber = 1;

    printf("\n--- Saved Journeys ---\n");

    while (fgets(line, sizeof(line), file))
    {
        trim(line); // Remove extra spaces or newlines
        if (strlen(line) == 0) continue; // Skip empty lines

        // Parse journey details
        char *start = strtok(line, ",");
        char *end = strtok(NULL, ",");
        char *stopCountStr = strtok(NULL, ",");
        int stopCount = atoi(stopCountStr);

        printf("\nJourney %d:\n", journeyNumber++);
        printf("Starting Location: %s\n", start);
        printf("Ending Location: %s\n", end);
        printf("Number of Stops: %d\n", stopCount);

        if (stopCount > 0)
        {
            printf("Stops:\n");
            for (int i = 0; i < stopCount; i++)
            {
                char *stop = strtok(NULL, ",");
                if (stop)
                {
                    printf("%d. %s\n", i + 1, stop);
                }
                else
                {
                    printf("Error reading stop %d.\n", i + 1);
                    break;
                }
            }
        }
        else
        {
            printf("No stops in this journey.\n");
        }
    }

    fclose(file);
}

// Save a journey to a CSV file
void saveJourney(const char *start, const char *end, char stops[MAX][100], int stopCount) {
    FILE *file = fopen("temp.csv", "a"); // Open in append mode
    if (!file)
    {
        printf("Error: Could not open temp.csv for writing.\n");
        return;
    }

    // Write start and end locations
    fprintf(file, "%s,%s,", start, end);

    // Write number of stops
    fprintf(file, "%d,", stopCount);

    // Write stops
    for (int i = 0; i < stopCount; i++) {
        fprintf(file, "%s", stops[i]);
        if (i < stopCount - 1) {
            fprintf(file, ","); // Add comma between stops
        }
    }

    fprintf(file, "\n"); // End the line
    fclose(file);
    printf("Journey saved successfully.\n");
}

// Remove a specific saved journey from the CSV file
void removeSpecificJourney() {
    FILE *file = fopen("temp.csv", "r");
    if (!file) // Check if the file exists
    {
        printf("No saved journeys found.\n");
        return;
    }

    char lines[MAX][1024]; // Array to store each line (journey)
    int lineCount = 0;

    // Read all lines from the file
    while (fgets(lines[lineCount], sizeof(lines[lineCount]), file)) {
        trim(lines[lineCount]); // Remove extra spaces or newlines
        if (strlen(lines[lineCount]) > 0) { // Skip empty lines
            lineCount++;
        }
    }
    fclose(file);

    if (lineCount == 0) {
        printf("No saved journeys found.\n");
        return;
    }

    // Display all journeys
    printf("\n--- Saved Journeys ---\n");
    for (int i = 0; i < lineCount; i++) {
        char temp[1024];
        strcpy(temp, lines[i]);
        char *start = strtok(temp, ",");
        char *end = strtok(NULL, ",");
        char *stopCountStr = strtok(NULL, ",");
        int stopCount = atoi(stopCountStr);

        printf("\nJourney %d:\n", i + 1);
        printf("Starting Location: %s\n", start);
        printf("Ending Location: %s\n", end);
        printf("Number of Stops: %d\n", stopCount);

        if (stopCount > 0) {
            printf("Stops:\n");
            for (int j = 0; j < stopCount; j++) {
                char *stop = strtok(NULL, ",");
                if (stop) {
                    printf("%d. %s\n", j + 1, stop);
                }
            }
        }
    }

    // Prompt user for the journey to delete
    int choice;
    printf("\nEnter the number of the journey you want to delete: ");
    scanf("%d", &choice);

    if (choice < 1 || choice > lineCount) {
        printf("Invalid choice. No changes made.\n");
        return;
    }

    // Write remaining journeys back to the file
    file = fopen("temp.csv", "w");
    if (!file) {
        printf("Error: Could not open temp.csv for writing.\n");
        return;
    }

    for (int i = 0; i < lineCount; i++) {
        if (i + 1 != choice) { // Skip the chosen journey
            fprintf(file, "%s\n", lines[i]);
        }
    }

    fclose(file);
    printf("Journey %d deleted successfully.\n", choice);
}

// Find the shortest path considering stops
// Function to find the shortest path between a source and destination, including specified stops
void findShortestPathWithStops(int graph[MAX][MAX], int n, char places[MAX][MAX], char *source, char *destination, char stops[MAX][100], int stopCount) {
    int totalDistance = 0; // Variable to store the total distance of the journey
    int startIndex = findLocationIndex(places, n, source); // Find the index of the source location
    int endIndex = findLocationIndex(places, n, destination); // Find the index of the destination location

    // Check if the source or destination is invalid
    if (startIndex == -1 || endIndex == -1) {
        printf("Invalid source or destination.\n");
        return;
    }

    // Array to hold the full path of the journey
    int fullPath[MAX], fullPathLength = 0;

    // Create a journey array containing the source, stops, and destination in travel order
    char journey[MAX][100];
    strcpy(journey[0], source); // Add the source to the journey
    for (int i = 0; i < stopCount; i++) {
        strcpy(journey[i + 1], stops[i]); // Add all stops to the journey
    }
    strcpy(journey[stopCount + 1], destination); // Add the destination to the journey

    // Loop through each segment of the journey (source -> stop1, stop1 -> stop2, ..., last stop -> destination)
    for (int i = 0; i <= stopCount; i++) {
        int segStart = findLocationIndex(places, n, journey[i]); // Get the start index of the current segment
        int segEnd = findLocationIndex(places, n, journey[i + 1]); // Get the end index of the current segment

        // Check if the segment's start or end point is invalid
        if (segStart == -1 || segEnd == -1) {
            printf("Invalid stop in the journey: %s or %s.\n", journey[i], journey[i + 1]);
            return;
        }

        int dist[MAX], parent[MAX]; // Arrays to store distances and parent nodes for Dijkstra's algorithm
        dijkstraWithPath(graph, n, segStart, segEnd, places, dist, parent); // Find the shortest path for the segment

        // Array to store the path for the current segment
        int segmentPath[MAX], segmentPathLength = 0;
        int temp = segEnd;
        
        // Retrieve the segment path using the parent array
        while (temp != -1) {
            segmentPath[segmentPathLength++] = temp;
            temp = parent[temp];
        }

        // Add the segment path to the full path, avoiding duplication of connecting nodes
        for (int j = segmentPathLength - 1; j >= 0; j--) {
            if (fullPathLength == 0 || fullPath[fullPathLength - 1] != segmentPath[j]) {
                fullPath[fullPathLength++] = segmentPath[j];
            }
        }

        // Add the segment's distance to the total distance
        totalDistance += dist[segEnd];
    }

    // Display the full journey path with distances
    printf("\nShortest path from %s to %s via stops:\n", source, destination);
    for (int i = 0; i < fullPathLength - 1; i++) {
        int u = fullPath[i], v = fullPath[i + 1];
        printf("%s%s%s --(%s%d%skm)--> ", CYAN, places[u], RESET, getColor(graph[u][v]), graph[u][v], RESET);
    }
    // Print the final destination in the path
    printf("%s%s%s\n", CYAN, places[fullPath[fullPathLength - 1]], RESET);
    printf("Total Distance: %dkm\n", totalDistance); // Print the total distance of the journey
}
