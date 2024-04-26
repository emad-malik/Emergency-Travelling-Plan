#include <iostream>
#include <climits>
using namespace std;
int NO_PARENT = -1;
// template for queue implementation
template<typename T>
class Queue
{
private:
    // Node structure definition
    struct Node
	 {
        int data;
        Node* next;
        Node(int value) : data(value), next(nullptr) {}
    };
public:
    Node* front;
    Node* rear;
    // Constructor
    Queue() : front(nullptr), rear(nullptr) {}
    // function to check if the queue is empty
    bool empty()
	{
        return front == nullptr;
    }
    // function to insert an element at the rear of the queue
    void enqueue(int value) 
	{
        Node* newNode = new Node(value);
        if (empty()) 
		{
            front = rear = newNode;
        } 
		else
		{
            rear->next = newNode;
            rear = newNode;
        }
    
    }
    // function to remove an element from the front of the queue
    void dequeue()
	 {
        if (empty())
		{
			return;
        }
        Node* temp = front;
        front = front->next;
        if (front == nullptr)
		{
            rear = nullptr; // Queue is now empty
        }
    }
    // function to get the value of the element at the front of the queue
    int Front() 
	{
        if (empty()) 
		{
            return -1;
        }
        return front->data;
    }
};
// travel plan class
class TravelPlan 
{
// private data members
private:
    int numCities;
    int** travel; // travel plan or graph
public:
    // constructor
    TravelPlan(int number_ofCities) : numCities(number_ofCities) 
    {
        InitializePlan();
    }
    // destructor
    ~TravelPlan() 
    {
        DeallocatePlan();
    }
    // function to initialize travel plan
    void InitializePlan() 
    {
        // allocate memory for the 2D array
        travel = new int*[numCities];
        for (int i = 0; i < numCities; ++i) 
        {
            travel[i] = new int[numCities];
        }
        // graph initialization
        for (int i = 0; i < numCities; ++i) 
        {
            for (int j = 0; j < numCities; ++j) 
            {
                travel[i][j] = 0;
            }
        }
        // setting road connections between cities
        for (int i = 0; i < numCities - 1; ++i) 
        {
            travel[i][i + 1] = 1;
        }
    }
    // getter for travel plan
    int** getGraph()
    {
        return travel;
    }
    // function to read user input
    void readInput(int& numCities)
    {
        //reads the number of aerial routes in input
        int numAerialRoutes;
        cin >> numAerialRoutes;
        for (int j = 0; j < numAerialRoutes; ++j) 
        {
            int source, destination;
            cin >> source >> destination;
            AddAerialConnection(source - 1,destination - 1) ;
        }
        for (int i = 0; i < numCities; ++i) 
        {
            for (int j = 0; j < numCities; ++j) 
            {
                if (travel[i][j] == 2) 
                {
                    cout << "Aerial Route: " << i + 1 << " -> " << j + 1 << endl;
                }
            }
        }
    }
    // function to deallocate travel plan
    void DeallocatePlan() 
    {
        // deallocate memory for the 2D array
        for (int i = 0; i < numCities; ++i) 
        {
            delete[] travel[i];
        }
        delete[] travel;
    }
    bool IsValidCity(int city) 
    {
        return (city >= 0 && city < numCities); // validating range of cities
    }
    // function to add road connection between cities(connecting edges to vertices)
    void AddRoadConnection(int source_city, int destination_city) 
    {
        if (IsValidCity(source_city) && IsValidCity(destination_city)) // validate cities and add road connection
        {
            travel[source_city][destination_city] = 1;
        } 
        else // invalid city case
        {
            cout << "Road to this city is under construction at the moment." << endl;
        }
    }
    // function to add aerial connection between cities
    void AddAerialConnection(int source_city, int destination_city) 
    {
        if (IsValidCity(source_city) && IsValidCity(destination_city)) 
        {
            travel[source_city][destination_city] = 2;
        } 
        else // invalid city case
        {
            cout << "Planes don't fly to this city." << endl;
        }
    }
    // function to print path
    void printPath(int currentVertex, int* parents, Queue<int>& q_structure ) 
    {
        if (currentVertex == NO_PARENT) // return if vertex is not connected to any city
        {
            return;
        }
        printPath(parents[currentVertex], parents, q_structure); // recursive call to print path 
        cout << currentVertex << " ";
        // storing the optimum vertices in our queue
        q_structure.enqueue(currentVertex);
    }
    // function to print shortest path 
    void printShortestPath(int startVertex, int destinationVertex, int* parents, int** travel) 
    {
        Queue<int> q_structure;
        int roadsTaken = 0;
        bool aerialTaken = false;
        int dayCount = 1;
        cout << "Shortest Path from " << startVertex << " to " << destinationVertex << ": ";
        printPath(destinationVertex, parents, q_structure); // recursively calling print path function
        while (!q_structure.empty()) 
        {
            int i = q_structure.Front();
            q_structure.dequeue();
            if (!q_structure.empty()) 
            {
                int j = q_structure.Front();
                if (travel[i][j] == 1 && roadsTaken != 6) // if user has not taken 6 roads in one day yet
                {
                    roadsTaken++;
                    aerialTaken = false;  // reset aerial count
                } 
                else if (travel[i][j] == 2) 
                {
                    aerialTaken = true;
                }
                if ((aerialTaken || (roadsTaken == 6)) && !q_structure.empty()) // if roads taken in one day is 6 and aerial route is taken again
                {
                    Queue<int> copy_q = q_structure;
                    copy_q.dequeue();
                    if (travel[j][copy_q.Front()] != 2) 
                    { 
                        roadsTaken = 0; // reset road count
                        dayCount++; // increment day count
                        cout << "day " << dayCount << ": ";
                        while (!copy_q.empty()) 
                        {
                            cout << copy_q.Front() << "->";
                            copy_q.dequeue();
                        }
                        cout << endl;
                        
                    }
                   
                }
            }
            
        }
        cout << "Total Days: " << dayCount << endl; // number of days to reach destination
    }
    // function to print cities reachable from a city
    void printSolution(int startVertex, int* distances, int* parents, int nVertices) 
    {
        cout << "Vertex\t Distance\tPath\n";
        for (int vertexIndex = 0; vertexIndex < nVertices; vertexIndex++) 
        {
            if (vertexIndex != startVertex) 
            {
                cout << startVertex << " -> " << vertexIndex << " \t\t " << distances[vertexIndex] << "\t\t";
            }
        }
    }
	// using bfs traversal to give minimum numbers of days to assist the traveler
  	void SolverAlgorithm(int** adjacencyMatrix, int startVertex, int nVertices) 
  	{
	    // arrays to store shortest distances, visited vertices, and parents
	    int* shortestDistances = new int[nVertices];
	    bool* added = new bool[nVertices];
	    int* parents = new int[nVertices];
	    // initialize arrays
	    for (int i = 0; i < nVertices; i++) 
	    {
	        shortestDistances[i] = INT_MAX;
	        added[i] = false;
	        parents[i] = NO_PARENT;
	    }
	    // set distance from startVertex to itself as 0
	    shortestDistances[startVertex] = 0;
	    // iterate over all vertices except the start vertex
	    for (int i = 1; i < nVertices; i++) 
	    {
	        // find the nearest vertex with the shortest distance
	        int nearestVertex = -1;
	        int shortestDistance = INT_MAX;
	        // iterate over all vertices
	        for (int vertexIndex = 0; vertexIndex < nVertices; vertexIndex++) 
	        {
	            // check if the vertex is not added and has a shorter distance
	            if (!added[vertexIndex] && shortestDistances[vertexIndex] < shortestDistance) 
	            {
	                nearestVertex = vertexIndex;
	                shortestDistance = shortestDistances[vertexIndex];
	            }
	        }
	        // mark the nearest vertex as added
	        added[nearestVertex] = true;
	        // update the distances to adjacent vertices or neighbours
	        for (int vertexIndex = 0; vertexIndex < nVertices; vertexIndex++) 
	        {
	            int edgeDistance = adjacencyMatrix[nearestVertex][vertexIndex];
	            // check if there is a valid edge and if the new distance is shorter
	            if (edgeDistance > 0 && (shortestDistance + edgeDistance) < shortestDistances[vertexIndex]) 
	            {
					// update parent and distance
	                parents[vertexIndex] = nearestVertex;
	                shortestDistances[vertexIndex] = shortestDistance + edgeDistance;
	           	 }
	        }
	    }
	
	    // print the shortest path from startVertex to the last vertex
	   	printShortestPath(0, nVertices-1, parents, adjacencyMatrix);
	    // deallocate memory
	    delete[] shortestDistances;
	    delete[] added;
	    delete[] parents;
	}
};
int main() 
{
    int numCases= 0;
    cin >> numCases;
	if(numCases <= 0) // test cases validation
	{
		cout<<"Enter valid number of test cases."<<endl;
	}
    for (int i = 0; i < numCases; ++i) 
    {
        int numCities= 0;
        cin>> numCities;
		if(numCities <= 0) // cities validation
		{
			cout<<"Enter valid number of cities."<<endl;
		}
        // Initialize the adjacency matrix
        TravelPlan graph(numCities);
        graph.InitializePlan();
        // Read aerial route inputs
        graph.readInput(numCities);
        graph.SolverAlgorithm(graph.getGraph(), 0, numCities);
    }
    return 0;
}