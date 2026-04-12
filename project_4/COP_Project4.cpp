#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <iomanip>
#include <limits>

using namespace std;

struct Edge;
struct Vertex;

// Represents a single vertex in the graph using adjacency list
struct Vertex
{
    string name;
    vector<Edge *> incidentEdges;
};

// Represents an undirected edge connecting two vertices
struct Edge
{
    Vertex *endpoint1;
    Vertex *endpoint2;
    double weight;
};

// Main graph class implementing graph operations using adjacency list
class Graph
{
private:
    vector<Vertex *> vertexList;
    vector<Edge *> edgeList;

    // Find a vertex by its name
    Vertex *findVertexByName(const string &vertexName)
    {
        for (Vertex *vertex : vertexList)
        {
            if (vertex->name == vertexName)
                return vertex;
        }
        return nullptr;
    }

    // Find an edge between two vertices (undirected)
    Edge *findEdgeBetween(Vertex *vertex1, Vertex *vertex2)
    {
        for (Edge *edge : edgeList)
        {
            bool isConnected = (edge->endpoint1 == vertex1 && edge->endpoint2 == vertex2) ||
                               (edge->endpoint1 == vertex2 && edge->endpoint2 == vertex1);
            if (isConnected)
                return edge;
        }
        return nullptr;
    }

    // DFS to find paths with minimum 3 vertices and minimum weight
    // Explores all possible paths and tracks the one with lowest total distance
    bool searchPathDFS(Vertex *current, Vertex *target, set<Vertex *> &visited,
                       vector<Vertex *> &currentPath, double currentWeight,
                       vector<Vertex *> &shortestPath, double &shortestWeight)
    {
        visited.insert(current);
        currentPath.push_back(current);

        // Found target with minimum 3 vertices
        if (current == target && currentPath.size() >= 3)
        {
            // Update shortest path if this one has lower weight
            if (shortestPath.empty() || currentWeight < shortestWeight)
            {
                shortestPath = currentPath;
                shortestWeight = currentWeight;
            }
        }

        // Explore neighbors (prune if current weight already exceeds best found)
        if (shortestPath.empty() || currentWeight < shortestWeight)
        {
            for (Edge *edge : current->incidentEdges)
            {
                Vertex *neighbor = (edge->endpoint1 == current) ? edge->endpoint2 : edge->endpoint1;

                if (visited.find(neighbor) == visited.end())
                {
                    searchPathDFS(neighbor, target, visited, currentPath,
                                  currentWeight + edge->weight, shortestPath, shortestWeight);
                }
            }
        }

        // Backtrack
        currentPath.pop_back();
        visited.erase(current);
        return !shortestPath.empty();
    }

public:
    // Constructor: Load graph from file
    // File format: Line 1 has vertex names, following lines have "vertex1 vertex2 weight"
    Graph(const string &filename)
    {
        ifstream inputFile(filename);

        if (!inputFile.is_open())
        {
            cerr << "Error: Could not open file '" << filename << "'" << endl;
            return;
        }

        string line;

        // Read vertices from first line
        if (getline(inputFile, line))
        {
            istringstream vertexStream(line);
            string vertexName;

            while (vertexStream >> vertexName)
            {
                Vertex *newVertex = new Vertex();
                newVertex->name = vertexName;
                vertexList.push_back(newVertex);
            }
        }

        // Read edges from remaining lines
        while (getline(inputFile, line))
        {
            if (line.empty())
                continue;

            istringstream edgeStream(line);
            string vertex1Name, vertex2Name;
            double edgeWeight;

            if (edgeStream >> vertex1Name >> vertex2Name >> edgeWeight)
            {
                insertEdge(vertex1Name, vertex2Name, edgeWeight);
            }
        }

        inputFile.close();
    }

    // Destructor: Free all allocated memory
    ~Graph()
    {
        for (Edge *edge : edgeList)
            delete edge;

        for (Vertex *vertex : vertexList)
            delete vertex;
    }

    // Check if graph was successfully loaded
    bool isEmpty() const
    {
        return vertexList.empty();
    }

    // Insert a new edge between two vertices
    bool insertEdge(const string &vertex1Name, const string &vertex2Name, double edgeWeight)
    {
        Vertex *vertex1 = findVertexByName(vertex1Name);
        Vertex *vertex2 = findVertexByName(vertex2Name);

        if (vertex1 == nullptr || vertex2 == nullptr)
            return false;

        // No parallel edges allowed
        if (findEdgeBetween(vertex1, vertex2) != nullptr)
            return false;

        Edge *newEdge = new Edge();
        newEdge->endpoint1 = vertex1;
        newEdge->endpoint2 = vertex2;
        newEdge->weight = edgeWeight;

        edgeList.push_back(newEdge);
        vertex1->incidentEdges.push_back(newEdge);
        vertex2->incidentEdges.push_back(newEdge);

        return true;
    }

    // Remove a vertex and all its incident edges
    bool removeVertex(const string &vertexName)
    {
        Vertex *targetVertex = findVertexByName(vertexName);

        if (targetVertex == nullptr)
            return false;

        // Remove all incident edges
        vector<Edge *> incidentEdgesCopy = targetVertex->incidentEdges;
        for (Edge *edge : incidentEdgesCopy)
        {
            Vertex *otherVertex = (edge->endpoint1 == targetVertex) ? edge->endpoint2 : edge->endpoint1;
            otherVertex->incidentEdges.erase(
                remove(otherVertex->incidentEdges.begin(),
                       otherVertex->incidentEdges.end(), edge),
                otherVertex->incidentEdges.end());

            edgeList.erase(
                remove(edgeList.begin(), edgeList.end(), edge),
                edgeList.end());
            delete edge;
        }

        // Remove vertex from graph
        vertexList.erase(
            remove(vertexList.begin(), vertexList.end(), targetVertex),
            vertexList.end());
        delete targetVertex;

        return true;
    }

    // Display all edges incident on a vertex
    // Format: "vertex1 to vertex2 is X.XX mi"
    void displayIncidentEdges(const string &vertexName)
    {
        Vertex *targetVertex = findVertexByName(vertexName);

        if (targetVertex == nullptr)
        {
            cout << "Vertex not found." << endl;
            return;
        }

        if (targetVertex->incidentEdges.empty())
        {
            cout << "No incident edges." << endl;
            return;
        }

        for (Edge *edge : targetVertex->incidentEdges)
        {
            Vertex *otherVertex = (edge->endpoint1 == targetVertex) ? edge->endpoint2 : edge->endpoint1;
            cout << vertexName << " to " << otherVertex->name << " is "
                 << fixed << setprecision(2) << edge->weight << " mi" << endl;
        }
    }

    // Find shortest path between two vertices (minimum 3 vertices)
    // Explores all valid paths and returns the one with minimum total weight
    vector<string> findShortestPath(const string &startName, const string &endName)
    {
        Vertex *startVertex = findVertexByName(startName);
        Vertex *endVertex = findVertexByName(endName);

        if (startVertex == nullptr || endVertex == nullptr)
            return {};

        // Use DFS to explore all paths and find one with minimum weight and 3+ vertices
        set<Vertex *> visitedSet;
        vector<Vertex *> currentPath;
        vector<Vertex *> shortestPath;
        double shortestWeight = numeric_limits<double>::max();

        searchPathDFS(startVertex, endVertex, visitedSet, currentPath,
                      0.0, shortestPath, shortestWeight);

        // Convert path to string format
        if (!shortestPath.empty())
        {
            vector<string> result;
            for (Vertex *vertex : shortestPath)
                result.push_back(vertex->name);
            return result;
        }

        return {};
    }
};

// Display the main menu options
void displayMainMenu()
{
    cout << "What would you like to do?" << endl;
    cout << "------------" << endl;
    cout << "1. Find edges incident on a vertex" << endl;
    cout << "2. Find a path in the graph" << endl;
    cout << "3. Insert an edge" << endl;
    cout << "4. Erase a vertex" << endl;
    cout << "5. Exit" << endl;
    cout << "Enter your choice: ";
}

// Handle menu option 1: Find incident edges
void handleIncidentEdges(Graph &graph)
{
    cout << "Enter a vertex: ";
    string vertexName;
    cin >> vertexName;
    cin.ignore();
    graph.displayIncidentEdges(vertexName);
    cout << endl;
}

// Handle menu option 2: Find path
void handleFindPath(Graph &graph)
{
    cout << "Enter start vertex: ";
    string startVertex;
    cin >> startVertex;
    cin.ignore();

    cout << "Enter end vertex: ";
    string endVertex;
    cin >> endVertex;
    cin.ignore();

    vector<string> path = graph.findShortestPath(startVertex, endVertex);

    if (path.empty())
    {
        cout << "No path found (must have at least 3 vertices)." << endl;
    }
    else
    {
        for (size_t i = 0; i < path.size(); i++)
        {
            cout << path[i];
            if (i < path.size() - 1)
                cout << " to ";
        }
        cout << endl;
    }
    cout << endl;
}

// Handle menu option 3: Insert edge
void handleInsertEdge(Graph &graph)
{
    cout << "Enter first vertex: ";
    string vertex1;
    cin >> vertex1;
    cin.ignore();

    cout << "Enter second vertex: ";
    string vertex2;
    cin >> vertex2;
    cin.ignore();

    cout << "Enter edge weight: ";
    double weight;
    if (!(cin >> weight))
    {
        // Clear input buffer if extraction failed
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Error: Invalid weight. Please enter a number." << endl;
        cout << endl;
        return;
    }
    cin.ignore();

    if (graph.insertEdge(vertex1, vertex2, weight))
    {
        cout << "Edge inserted." << endl;
    }
    else
    {
        cout << "Error: Could not insert edge." << endl;
    }
    cout << endl;
}

// Handle menu option 4: Remove vertex
void handleRemoveVertex(Graph &graph)
{
    cout << "Enter vertex to erase: ";
    string vertexName;
    cin >> vertexName;
    cin.ignore();

    if (graph.removeVertex(vertexName))
    {
        cout << "Vertex removed." << endl;
    }
    else
    {
        cout << "Error: Vertex not found." << endl;
    }
    cout << endl;
}

// Main program
int main()
{
    cout << "Hello!" << endl;
    cout << "Enter the file name: ";
    string inputFileName;
    cin >> inputFileName;
    cin.ignore();

    // Load graph from file
    Graph graph(inputFileName);

    // Check if graph was successfully loaded
    if (graph.isEmpty())
    {
        cout << "Failed to load graph. Exiting program." << endl;
        return 1;
    }

    cout << "Thank you. Your graph is ready." << endl;
    cout << "------------" << endl;

    // Main menu loop
    int userChoice;
    while (true)
    {
        displayMainMenu();
        if (!(cin >> userChoice))
        {
            // Clear input buffer if extraction failed
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid choice. Please enter a number between 1 and 5." << endl
                 << endl;
            continue;
        }
        cin.ignore();

        switch (userChoice)
        {
        case 1:
            handleIncidentEdges(graph);
            break;
        case 2:
            handleFindPath(graph);
            break;
        case 3:
            handleInsertEdge(graph);
            break;
        case 4:
            handleRemoveVertex(graph);
            break;
        case 5:
            cout << "Goodbye!" << endl;
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl
                 << endl;
        }
    }

    return 0;
}
