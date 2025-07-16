#include <stdio.h>
#include "header.h"
#define MAX 100
int main()
{
    int graph[MAX][MAX];
    char places[MAX][MAX];
    int n = readCSV("places.csv", graph, places);

    if (n == 0)
    {
        return 1;
    }

    while (1)
    {
        printf("\nMenu:\n");
        printf("1. Display Locations\n");
        printf("2. Find Shortest Path\n");
        printf("3. Add Location\n");
        printf("4. Delete Location\n");
        printf("5. Add Path\n");
        printf("6. Delete Path\n");
        printf("7. Alternate Path to avoid specific location via shortest route\n");
        printf("8. Find Shortest Path with Stops\n");
        printf("9. Save Current Journey\n");
        printf("10. Display Saved Journey\n");
        printf("11. Clear Saved Journey\n");
        printf("12. Exit\n");

        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);
        getchar(); // Clear newline

        switch(choice)
        {
            case 1:
                displayLocations(places, n);
                break;
            
            case 2:
            {
                char start[100], end[100];
                printf("Enter starting location: ");
                fgets(start, sizeof(start), stdin);
                trim(start);
                printf("Enter ending location: ");
                fgets(end, sizeof(end), stdin);
                trim(end);

                int startIndex = findLocationIndex(places, n, start);
                int endIndex = findLocationIndex(places, n, end);

                if (startIndex == -1 || endIndex == -1)
                {
                    printf("Invalid locations.\n");
                    continue;
                }

                dijkstra(graph, n, startIndex, endIndex, places);
                break;
            }
            
            case 3:
                addLocation(graph, places, &n);
                writeCSV("places.csv", graph, places, n);
                break;
            
            case 4:
                deleteLocation(graph, places, &n);
                writeCSV("places.csv", graph, places, n);
                break;
            
            case 5:
                addPath(graph, places, n);
                writeCSV("places.csv", graph, places, n);
                break;
            
            case 6:
                deletePath(graph, places, n);
                writeCSV("places.csv", graph, places, n);
                break;
            
            case 7:
            {
                char start[100], end[100], avoid[100];
                printf("Enter starting location: ");
                fgets(start, sizeof(start), stdin);
                trim(start);
                printf("Enter ending location: ");
                fgets(end, sizeof(end), stdin);
                trim(end);
                printf("Enter location to avoid: ");
                fgets(avoid, sizeof(avoid), stdin);
                trim(avoid);

                int startIndex = findLocationIndex(places, n, start);
                int endIndex = findLocationIndex(places, n, end);
                int avoidIndex = findLocationIndex(places, n, avoid);

                if (startIndex == -1 || endIndex == -1 || avoidIndex == -1)
                {
                    printf("One or more locations are invalid.\n");
                    continue;
                }

                findAlternatePath(graph, n, startIndex, endIndex, avoidIndex, places);
                break;
            }
            
            case 8:
            {
                char source[100], destination[100];
                char stops[MAX][100];
                int stopCount;

                printf("Enter starting location: ");
                fgets(source, sizeof(source), stdin);
                trim(source);

                printf("Enter ending location: ");
                fgets(destination, sizeof(destination), stdin);
                trim(destination);

                printf("Enter the number of stops: ");
                scanf("%d", &stopCount);
                getchar(); // Clear newline

                for (int i = 0; i < stopCount; i++) {
                    printf("Enter stop %d: ", i + 1);
                    fgets(stops[i], sizeof(stops[i]), stdin);
                    trim(stops[i]);
                }

                findShortestPathWithStops(graph, n, places, source, destination, stops, stopCount);
                break;
            }
            
            case 9:
            {
                char start[100], end[100];
                char stops[MAX][100];
                int stopCount = 0;

                printf("Enter starting location: ");
                fgets(start, sizeof(start), stdin);
                trim(start);

                printf("Enter ending location: ");
                fgets(end, sizeof(end), stdin);
                trim(end);

                printf("Enter the number of stops (0 if none): ");
                scanf("%d", &stopCount);
                getchar(); // Clear newline

                for (int i = 0; i < stopCount; i++) {
                    printf("Enter stop %d: ", i + 1);
                    fgets(stops[i], sizeof(stops[i]), stdin);
                    trim(stops[i]);
                }

                saveJourney(start, end, stops, stopCount);
                
                break;
            }
            
            case 10:
                 displaySavedJourneys();
                break;
            
            case 11:
                removeSpecificJourney();
                break;
            
            case 12:
                printf("Exiting.\n");
                return 0;
            
            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}