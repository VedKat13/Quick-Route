// #ifndef LOGIC_H
// #define LOGIC_H

#include <stdbool.h>

// Maximum number of places
#define MAX 100 

// ANSI color codes for styling
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"

// Structure definitions
typedef struct {
    int vertex;
    int distance;
} MinHeapNode;

typedef struct {
    int size;
    int capacity;
    int *position;         // Maps vertex to heap position
    MinHeapNode **array;   // Heap node array
} MinHeap;

// Function prototypes
MinHeapNode *newMinHeapNode(int vertex, int distance);
MinHeap *createMinHeap(int capacity);
void swapMinHeapNode(MinHeapNode **a, MinHeapNode **b);
void minHeapify(MinHeap *minHeap, int idx);
MinHeapNode *extractMin(MinHeap *minHeap);
void decreaseKey(MinHeap *minHeap, int vertex, int distance);
bool isInMinHeap(MinHeap *minHeap, int vertex);
const char *getColor(int distance);

void dijkstraWithPath(int graph[MAX][MAX], int n, int start, int end, char places[MAX][MAX], int dist[MAX], int parent[MAX]);
void dijkstra(int graph[MAX][MAX], int n, int start, int end, char places[MAX][MAX]);
void findAlternatePath(int graph[MAX][MAX], int n, int start, int end, int avoid, char places[MAX][MAX]);


// Trim function to remove leading and trailing spaces
void trim(char *str);

// Sort locations alphabetically and update the graph
void sortLocations(char places[MAX][MAX], int graph[MAX][MAX], int *n);

// Display available locations
void displayLocations(char places[MAX][MAX], int n);

// Find the index of a location in the places array
int findLocationIndex(char places[MAX][100], int size, const char *location);

// Read CSV file to initialize the graph and locations
int readCSV(const char *filename, int graph[MAX][MAX], char places[MAX][MAX]);

// Write the graph and locations to CSV
void writeCSV(const char *filename, int graph[MAX][MAX], char places[MAX][MAX], int n);

// Add a new path between locations
void addPath(int graph[MAX][MAX], char places[MAX][100], int n);

// Delete a path between locations
void deletePath(int graph[MAX][MAX], char places[MAX][100], int n);

// Add a new location to the graph
void addLocation(int graph[MAX][MAX], char places[MAX][MAX], int *n);

// Delete a location from the graph
void deleteLocation(int graph[MAX][MAX], char places[MAX][MAX], int *n);

// Display all saved journeys
void displaySavedJourneys();

// Save a journey to the CSV
void saveJourney(const char *start, const char *end, char stops[MAX][100], int stopCount);

// Remove a specific journey from the saved journeys
void removeSpecificJourney();

// Find the shortest path including stops between locations
void findShortestPathWithStops(int graph[MAX][MAX], int n, char places[MAX][MAX], char *source, char *destination, char stops[MAX][100], int stopCount);

// #endif // LOCATION_MANAGER_H