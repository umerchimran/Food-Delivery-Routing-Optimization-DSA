#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <limits>

using namespace std;

const int MAX_N = 100; // Maximum grid size
const int MAX_R = 10; // Maximum number of restaurants
const int MAX_O = 10; // Maximum number of orders per restaurant

// classto represent a node in the grid
class Node {
public:

    int id;
    int x, y;
};

// class to represent an edge in the graph
class EDGE 

{
public:
    int u, v;
    int weight;
};

// class to represent a restaurant
class Restaurant {
public:
    string name;
    int location;
    int numOrders;
    Node orders[MAX_O];
};

// class to represent a rider's route
class Route 
{
public:
    int NUM_NODES;
    Node nodes[MAX_N];
};

void READ_INPUT(int& NUM_TEST_CASES, int& N, int& I, int& R, Restaurant restaurants[], ifstream& file)
{
    file >> NUM_TEST_CASES;


    for (int t = 0; t < NUM_TEST_CASES; t++) 
    {
        file >> N >> I >> R;
        for (int r = 0; r < R; r++) {
            file >> restaurants[r].name >> restaurants[r].location >> restaurants[r].numOrders;
            for (int o = 0; o < restaurants[r].numOrders; o++) {
                file >> restaurants[r].orders[o].id >> restaurants[r].orders[o].x >> restaurants[r].orders[o].y; 
            }
        }
    }
}

void dijkstra_ALGO(int numNodes, EDGE edges[], int start, int end, Route& route)
{
    int dist[MAX_N];
    int prev[MAX_N];
    bool visited[MAX_N];
     
                // Initialize distances, previous nodes, and visited array
    for (int i = 0; i < numNodes; i++)
    {
        dist[i] = numeric_limits<int>::max();

        prev[i] = -1; 

        visited[i] = false;
    }

    dist[start] = 0;

    // Find the shortest path
    for (int i = 0; i < numNodes; i++)
    {
        int u = -1;
        for (int j = 0; j < numNodes; j++) {
            if (!visited[j] && (u == -1 || dist[j] < dist[u])) {
                u = j;
            }
        }

        if (u == -1) break;

        visited[u] = true;

        for (int j = 0; j < numNodes; j++) {
            if (edges[u * numNodes + j].weight != 0 && !visited[j]) 
            {
                int newDist = dist[u] + edges[u * numNodes + j].weight;

                if (newDist < dist[j]) 
                {
                    dist[j] = newDist;

                    prev[j] = u;
                }
            }
        }
    }

    // Build the route

    route.NUM_NODES = 0;

    int curr = end; // Declare and define curr here
    while (curr != -1) {
        if (curr >= 0 && curr < numNodes)
              {  // Check bounds of prev array

            if (route.NUM_NODES < MAX_N)
            {
                route.nodes[route.NUM_NODES++] = { curr, 0, 0 };

            }
            curr = prev[curr];        // Update curr with the previous node in the shortest path
        }
        else {
            break; // Exit loop if curr is out of bounds
        }
    }
}
// Function to write output to a file

void W_OUTPUT(int& totalTime, ofstream& file) 
{
    file << totalTime << endl;
}

void PRINT_GRAPH(int N, EDGE edges[]) 
{
    cout << "Grid Graph Representation:" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int nodeId = i * N + j;
            cout << "Node " << nodeId << " Neighbors: ";
            // Check if the node has a neighbor below
            if (i < N - 1) {
                int neighborId = nodeId + N;
                cout << neighborId << " ";
            }
            // Check if the node has a neighbor to the right
            if (j < N - 1) {
                int neighborId = nodeId + 1;
                cout << neighborId << " ";
            }
            cout << endl;
        }
    }
}

int main() 
{
    int numTestCases;

    int N, I, R;
    Restaurant restaurants[MAX_R];

    ifstream inputFile("input.txt");

    READ_INPUT(numTestCases, N, I, R, restaurants, inputFile);

    ofstream outputFile("output_debug.txt");

    for (int t = 0; t < numTestCases; t++) {
        // Print the grid graph representation
        PRINT_GRAPH(N, nullptr); // You can pass nullptr for the edges since it's not needed for printing

        int numNodes = N * N;

        EDGE* edges = new (std::nothrow) EDGE[numNodes * numNodes];
        if (!edges) 
        {
            // Handle the error, e.g., print an error message and exit

            std::cerr << "Error: unable to allocate memory for edges" << std::endl;
            return 1;
        }
        int edgeIndex = 0;

        // Create edges for the grid
        for (int i = 0; i < N; i++) 
        {
            for (int j = 0; j < N; j++) {
                int nodeId = i * N + j;
                if (i < N - 1) {
                    edges[edgeIndex++] = { nodeId, nodeId + N, 1 };
                }
                if (j < N - 1) {
                    edges[edgeIndex++] = { nodeId, nodeId + 1, 1 };
                }
            }
        }

        // Create edges for restaurants and orders
        for (int r = 0; r < R; r++)
        {
            int RESTAURANT_X = restaurants[r].location / N;

            int RESTAURANT_Y = restaurants[r].location % N;

            int RESTAURANT_NODE_ID = RESTAURANT_X * N + RESTAURANT_Y;

            for (int o = 0; o < restaurants[r].numOrders; o++) 
            {
                int orderX = restaurants[r].orders[o].x;

                int orderY = restaurants[r].orders[o].y;

                int orderNodeID = orderX * N + orderY;

                edges[edgeIndex++] = { RESTAURANT_NODE_ID, orderNodeID, 1 };

            }
        }
        int TIME = 0;

        for (int r = 0; r < R; r++)
        {
            int RESTAURANT_LOCA = restaurants[r].location;

            for (int o = 0; o < restaurants[r].numOrders; o++) 
            {
                int ORDER_LOCATION = restaurants[r].orders[o].id;

                Route route;

                dijkstra_ALGO(numNodes, edges, RESTAURANT_LOCA, ORDER_LOCATION, route);

                cout << "Route numNodes: " << route.NUM_NODES << endl;

                cout << "Route nodes: ";

                for (int i = 0; i < route.NUM_NODES; i++)
                {
                    cout << route.nodes[i].id << " ";
                }
                cout << endl;
                // Calculate the route time using the adjacency matrix directly
                int routeTime = 0;

                for (int i = 0; i < route.NUM_NODES - 1; i++) 
                {
                    int u = route.nodes[i].id;

                    int v = route.nodes[i + 1].id;

                    int distance = 1; // Since all edges have a weight of 1

                    routeTime += distance;
                }

                TIME += routeTime;
            }
        }
        outputFile << TIME << endl; // Write the total time to the output file
    }
    return 0;
}