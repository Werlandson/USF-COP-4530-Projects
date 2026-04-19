/*
 * Names: Tuan Minh Duong, Waylon Erlandson, and Duc Long Nguyen
 * Description: Processes a formatted .txt into a graph data structure, which can return
 * edges incident to a vertex, find a path in the graph, insert an edge, or erase a vertex
 * according to user input.
 * */
#include <algorithm>
#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

// Twelve hyphens: visual separator used in the assignment menu (before/around the prompt)
static const char *const MENU_RULE = "------------";

// Strip leading/trailing ASCII whitespace from a token (used after splitting on tabs)
static string trimWhitespace(const string &s)
{
    size_t start = 0;
    while (start < s.size() && isspace(static_cast<unsigned char>(s[start])))
        ++start;
    size_t end = s.size();
    while (end > start && isspace(static_cast<unsigned char>(s[end - 1])))
        --end;
    return s.substr(start, end - start);
}

// Split one text line on delim; empty fields after trim are skipped
static vector<string> splitOnDelimiter(const string &line, char delim)
{
    vector<string> out;
    string part;
    stringstream ss(line);
    while (getline(ss, part, delim))
    {
        string t = trimWhitespace(part);
        if (!t.empty())
            out.push_back(t);
    }
    return out;
}

// First file line: vertex names separated by tabs (handout) or by spaces (input.txt)
static vector<string> parseVertexLine(const string &line)
{
    vector<string> names;
    if (line.find('\t') != string::npos)
        return splitOnDelimiter(line, '\t');
    istringstream verticesLine(line);
    string name;
    while (verticesLine >> name)
        names.push_back(name);
    return names;
}

// One edge line: endpoint, endpoint, numeric label — tabs or spaces between fields
static bool parseEdgeLine(const string &line, string &aName, string &bName, double &weight)
{
    if (line.find('\t') != string::npos)
    {
        vector<string> fields = splitOnDelimiter(line, '\t');
        if (fields.size() >= 3)
        {
            aName = fields[0];
            bName = fields[1];
            istringstream wstream(fields[2]);
            if (wstream >> weight)
                return true;
        }
    }
    istringstream edgeLine(line);
    return static_cast<bool>(edgeLine >> aName >> bName >> weight);
}

class Edge; // Forward declaration so Vertex can store Edge pointers

// Vertex stores a location name and all edges touching that location
class Vertex
{
private:
    string element;
    vector<Edge *> incident;

    // Constructor is private so Graph controls vertex creation
    explicit Vertex(string value) : element(std::move(value)) {}

public:
    // Return the data stored in this vertex
    const string &operator*() const { return element; }

    // Return all edges incident on this vertex
    vector<Edge *> incidentEdges() const { return incident; }

    // True if this vertex shares an edge with vertex v
    bool isAdjacentTo(const Vertex *v) const;

    friend class Graph;
    friend class Edge;
};

// Edge stores two endpoints and a numeric label (distance)
class Edge
{
private:
    Vertex *u;
    Vertex *v;
    double label;

    // Constructor is private so Graph controls edge creation
    Edge(Vertex *a, Vertex *b, double x) : u(a), v(b), label(x) {}

public:
    // Return this edge's label
    double operator*() const { return label; }

    // Return both endpoints as a pair
    pair<Vertex *, Vertex *> endVertices() const { return {u, v}; }

    // Given one endpoint, return the other endpoint
    Vertex *opposite(Vertex *vertex) const
    {
        if (vertex == u)
            return v;
        if (vertex == v)
            return u;
        return nullptr;
    }

    // True if this edge and e share at least one endpoint
    bool isAdjacentTo(const Edge *e) const
    {
        return e != nullptr && (u == e->u || u == e->v || v == e->u || v == e->v);
    }

    // True if this edge touches the given vertex
    bool isIncidentOn(const Vertex *vertex) const
    {
        return vertex != nullptr && (vertex == u || vertex == v);
    }

    friend class Graph;
};

// Check adjacency by scanning this vertex's incident edges
bool Vertex::isAdjacentTo(const Vertex *v) const
{
    if (v == nullptr)
        return false;

    for (Edge *e : incident)
    {
        // const_cast is safe here because opposite() does not mutate vertex
        if (e->opposite(const_cast<Vertex *>(this)) == v)
            return true;
    }
    return false;
}

// Graph ADT using an adjacency-list style representation
class Graph
{
public:
    // Result values for detailed insertEdge validation output
    enum class InsertEdgeStatus
    {
        inserted,
        null_vertex,
        duplicate_edge,
        parallel_edge
    };

private:
    vector<Vertex *> vertexStore;
    vector<Edge *> edgeStore;

    // DFS helper: finds any path from current to target with at least 3 vertices
    bool dfsPath(Vertex *current, Vertex *target, unordered_set<Vertex *> &visited,
                 vector<Vertex *> &path, vector<Vertex *> &answer) const
    {
        if (current == target && path.size() >= 3)
        {
            answer = path;
            return true;
        }

        for (Edge *e : current->incident)
        {
            Vertex *next = e->opposite(current);
            if (visited.find(next) != visited.end())
                continue;

            // Standard backtracking: choose
            visited.insert(next);
            path.push_back(next);
            if (dfsPath(next, target, visited, path, answer))
                return true;
            // Backtrack: un-choose
            path.pop_back();
            visited.erase(next);
        }

        return false;
    }

public:
    // Default constructor for an empty graph
    Graph() = default;

    // Build a graph from an input file (line 1 = vertices; following lines = edges)
    explicit Graph(const string &filename)
    {
        ifstream input(filename);
        if (!input.is_open())
        {
            cerr << "Error: Could not open file '" << filename << "'" << endl;
            return;
        }

        string line;
        // Line 1 lists every vertex name (tab-separated or space-separated)
        if (getline(input, line))
        {
            vector<string> vertexNames = parseVertexLine(line);
            for (const string &name : vertexNames)
                insertVertex(name);
        }

        // Each non-empty line adds one undirected edge (two names + label)
        while (getline(input, line))
        {
            if (line.empty())
                continue;

            string aName;
            string bName;
            double weight = 0.0;
            if (!parseEdgeLine(line, aName, bName, weight))
                continue;

            Vertex *a = findVertex(aName);
            Vertex *b = findVertex(bName);
            if (a != nullptr && b != nullptr)
                insertEdge(a, b, weight);
        }
    }

    // Release dynamically allocated edges and vertices
    ~Graph()
    {
        for (Edge *e : edgeStore)
            delete e;
        for (Vertex *v : vertexStore)
            delete v;
    }

    // Return all vertices in the graph
    vector<Vertex *> vertices() const { return vertexStore; }

    // Return all edges in the graph
    vector<Edge *> edges() const { return edgeStore; }

    // Insert a vertex with value x if it does not already exist
    Vertex *insertVertex(const string &x)
    {
        if (findVertex(x) != nullptr)
            return nullptr;
        Vertex *vertex = new Vertex(x);
        vertexStore.push_back(vertex);
        return vertex;
    }

    // Required ADT function. Inserts edge if valid; otherwise returns nullptr
    Edge *insertEdge(Vertex *v, Vertex *w, double x)
    {
        InsertEdgeStatus status = insertEdgeChecked(v, w, x);
        if (status != InsertEdgeStatus::inserted)
            return nullptr;

        Edge *edge = new Edge(v, w, x);
        edgeStore.push_back(edge);
        v->incident.push_back(edge);
        w->incident.push_back(edge);
        return edge;
    }

    // Validate edge insertion request and report specific failure reason
    InsertEdgeStatus insertEdgeChecked(Vertex *v, Vertex *w, double x) const
    {
        if (v == nullptr || w == nullptr)
            return InsertEdgeStatus::null_vertex;

        Edge *existing = findEdge(v, w);
        if (existing == nullptr)
            return InsertEdgeStatus::inserted;

        // Same endpoints + same label -> duplicate edge request
        if (fabs((*(*existing)) - x) < 1e-9)
            return InsertEdgeStatus::duplicate_edge;

        // Same endpoints + different label -> parallel edge request
        return InsertEdgeStatus::parallel_edge;
    }

    // Remove a vertex and all incident edges
    bool eraseVertex(Vertex *v)
    {
        if (v == nullptr)
            return false;

        auto it = find(vertexStore.begin(), vertexStore.end(), v);
        if (it == vertexStore.end())
            return false;

        vector<Edge *> toDelete = v->incident;
        for (Edge *e : toDelete)
            eraseEdge(e);

        vertexStore.erase(it);
        delete v;
        return true;
    }

    // Remove a single edge from the graph and both endpoint lists
    bool eraseEdge(Edge *e)
    {
        if (e == nullptr)
            return false;

        auto edgeIt = find(edgeStore.begin(), edgeStore.end(), e);
        if (edgeIt == edgeStore.end())
            return false;

        // Helper to remove this edge pointer from one vertex adjacency list
        auto eraseFromIncident = [e](Vertex *vertex)
        {
            vertex->incident.erase(remove(vertex->incident.begin(), vertex->incident.end(), e),
                                   vertex->incident.end());
        };

        eraseFromIncident(e->u);
        eraseFromIncident(e->v);
        edgeStore.erase(edgeIt);
        delete e;
        return true;
    }

    // Find vertex by its name; returns nullptr if missing
    Vertex *findVertex(const string &name) const
    {
        for (Vertex *v : vertexStore)
        {
            if ((*(*v)) == name)
                return v;
        }
        return nullptr;
    }

    // Find an undirected edge between vertices a and b
    Edge *findEdge(Vertex *a, Vertex *b) const
    {
        for (Edge *e : edgeStore)
        {
            if ((e->u == a && e->v == b) || (e->u == b && e->v == a))
                return e;
        }
        return nullptr;
    }

    // Find any path between start and end with at least 3 vertices
    vector<Vertex *> findPathAtLeastThree(const string &startName, const string &endName) const
    {
        Vertex *start = findVertex(startName);
        Vertex *end = findVertex(endName);
        if (start == nullptr || end == nullptr)
            return {};

        unordered_set<Vertex *> visited;
        vector<Vertex *> path;
        vector<Vertex *> answer;

        // Seed DFS with the start node
        visited.insert(start);
        path.push_back(start);
        // DFS writes the first valid path into "answer"
        dfsPath(start, end, visited, path, answer);
        return answer;
    }
};

// Print twelve dashes, the prompt, twelve dashes again, then the four numbered actions
static void printMenu()
{
    cout << MENU_RULE << endl;
    cout << "What would you like to do?" << endl;
    cout << MENU_RULE << endl;
    cout << "1. Find edges incident on a vertex" << endl;
    cout << "2. Find a path in the graph" << endl;
    cout << "3. Insert an edge" << endl;
    cout << "4. Erase a vertex" << endl;
}

// Menu option 1: print all incident edges for a vertex
static void optionIncidentEdges(const Graph &graph)
{
    cout << "Enter a vertex: ";
    string name;
    cin >> name;

    Vertex *vertex = graph.findVertex(name);
    if (vertex == nullptr)
    {
        cout << "Error: Vertex not found." << endl
             << endl;
        return;
    }

    vector<Edge *> incidents = vertex->incidentEdges();
    if (incidents.empty())
    {
        cout << "No incident edges." << endl
             << endl;
        return;
    }

    for (Edge *edge : incidents)
    {
        // Neighbor across this edge (the endpoint that is not the queried vertex)
        Vertex *other = edge->opposite(vertex);
        cout << *(*vertex) << " to " << *(*other) << " is "
             << *(*edge) << " mi" << endl;
    }
    cout << endl;
}

// Menu option 2: print a valid path as vertices only
static void optionFindPath(const Graph &graph)
{
    cout << "Enter start vertex: ";
    string start;
    cin >> start;
    cout << "Enter end vertex: ";
    string end;
    cin >> end;

    vector<Vertex *> path = graph.findPathAtLeastThree(start, end);
    if (path.empty())
    {
        cout << "No valid path found." << endl
             << endl;
        return;
    }

    for (size_t i = 0; i < path.size(); ++i)
    {
        // Vertices only, joined by the handout phrase " to " (no edge weights)
        cout << *(*path[i]);
        if (i + 1 < path.size())
            cout << " to ";
    }
    cout << endl
         << endl;
}

// Menu option 3: insert an edge with specific validation errors
static void optionInsertEdge(Graph &graph)
{
    cout << "Enter first vertex: ";
    string aName;
    cin >> aName;
    cout << "Enter second vertex: ";
    string bName;
    cin >> bName;
    cout << "Enter edge label (distance): ";
    double label;
    if (!(cin >> label))
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Error: Invalid edge label." << endl
             << endl;
        return;
    }

    Vertex *a = graph.findVertex(aName);
    Vertex *b = graph.findVertex(bName);
    Graph::InsertEdgeStatus status = graph.insertEdgeChecked(a, b, label);
    // Each case below maps one validation failure to a specific message
    if (status == Graph::InsertEdgeStatus::null_vertex)
    {
        cout << "Error: Both vertices must exist." << endl
             << endl;
        return;
    }
    if (status == Graph::InsertEdgeStatus::duplicate_edge)
    {
        cout << "Error: Duplicate edge already exists." << endl
             << endl;
        return;
    }
    if (status == Graph::InsertEdgeStatus::parallel_edge)
    {
        cout << "Error: Parallel edges are not allowed." << endl
             << endl;
        return;
    }

    graph.insertEdge(a, b, label);
    cout << "Edge inserted" << endl
         << endl;
}

// Menu option 4: erase a vertex and every incident edge
static void optionEraseVertex(Graph &graph)
{
    cout << "Enter vertex to erase: ";
    string name;
    cin >> name;

    Vertex *vertex = graph.findVertex(name);
    if (vertex == nullptr)
    {
        cout << "Error: Vertex not found." << endl
             << endl;
        return;
    }

    graph.eraseVertex(vertex);
    cout << "Vertex removed" << endl
         << endl;
}

// Program entry: load graph file, then repeat menu until the user types exit
int main()
{
    // Opening prompts required by the assignment handout
    cout << "Hello!" << endl;
    cout << "Enter the file name: ";
    string fileName;
    cin >> fileName;
    cout << endl;

    // Construct graph from disk (vertices on line 1; one edge per following line)
    Graph graph(fileName);
    if (graph.vertices().empty())
    {
        cout << "Failed to load graph." << endl;
        return 1;
    }

    cout << "Thank you. Your graph is ready." << endl;

    while (true)
    {
        // Each iteration shows dashes + prompt + options; user enters 1–4 or exit
        printMenu();
        string selection;
        cin >> selection;

        if (selection == "exit")
            break;

        if (selection == "1")
            optionIncidentEdges(graph);
        else if (selection == "2")
            optionFindPath(graph);
        else if (selection == "3")
            optionInsertEdge(graph);
        else if (selection == "4")
            optionEraseVertex(graph);
        else
            cout << "Error: Invalid menu option." << endl
                 << endl;
    }

    return 0;
}
