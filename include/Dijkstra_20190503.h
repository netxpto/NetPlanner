# ifndef DIJKSTRA_H
# define DIJKSTRA_H

#include<iostream> 
#include <list> 
#include <vector>
//# include <fstream>
//# include <string>
//# include <algorithm> 
//# include <chrono>

//using namespace std;    (commented by Romil 08/05/2020)

class Graph
{
	int V; // No. of vertices in graph 
	std::list<int> *adj; // Pointer to an array containing adjacency lists 
	std::vector<std::vector<int>> listOfAllPaths; // List containing all possible paths

	// A recursive function used by printAllPaths() 
	void printAllPathsUtil(int, int, bool[], int[], int &);

public:
	Graph(int V); // Constructor 
	void addEdge(int u, int v);
	void printAllPaths(int s, int d);
	std::vector<std::vector<int>> printFinalPaths(int numberOfPaths);
};

Graph::Graph(int V)
{
	this->V = V;
	adj = new std::list<int>[V];
}

void Graph::addEdge(int u, int v)
{
	adj[u].push_back(v); // Add v to u’s list. 
}

// Prints all paths from 's' to 'd' 
void Graph::printAllPaths(int s, int d)
{
	// Mark all the vertices as not visited 
	bool *visited = new bool[V];

	// Create an array to store paths 
	int *path = new int[V];
	int path_index = 0; // Initialize path[] as empty 

	// Initialize all vertices as not visited 
	for (int i = 0; i < V; i++)
		visited[i] = false;

	// Call the recursive helper function to print all paths 
	printAllPathsUtil(s, d, visited, path, path_index);
}

// A recursive function to print all paths from 'u' to 'd'. 
// visited[] keeps track of vertices in current path. 
// path[] stores actual vertices and path_index is current 
// index in path[] 
void Graph::printAllPathsUtil(int u, int d, bool visited[],
	int path[], int &path_index)
{
	visited[u] = true;
	path[path_index] = u;
	path_index++;
	int pathMaxSize;
	// If current vertex is same as destination, then print 
	// current path[] 
	if (u == d)
	{
		std::vector<int> newPath;

		for (int i = 0; i < path_index; i++)
		{
			//cout << path[i] << " ";
			newPath.push_back(path[i]);
		}
		listOfAllPaths.push_back(newPath);

		//cout << std::endl;

	}
	//}
	else // If current vertex is not destination 
	{
		// Recur for all the vertices adjacent to current vertex 
		std::list<int>::iterator i;
		for (i = adj[u].begin(); i != adj[u].end(); ++i)
			if (!visited[*i])
				printAllPathsUtil(*i, d, visited, path, path_index);
	}

	// Remove current vertex from path[] and mark it as unvisited 
	path_index--;
	visited[u] = false;


}

std::vector<std::vector<int>> Graph::printFinalPaths(int numberOfPaths)
{

	int hops;
	std::vector<int> temp;
	std::vector<std::vector<int>> finalPaths;


	//sorting paths in ASCENDING ORDER of hops
	for (int i = 0; i < listOfAllPaths.size(); i++) // number of paths
	{
		for (int j = i + 1; j < listOfAllPaths.size(); j++)
		{
			if (listOfAllPaths[i].size() > listOfAllPaths[j].size())
			{
				temp = listOfAllPaths[i];
				listOfAllPaths[i] = listOfAllPaths[j];
				listOfAllPaths[j] = temp;
			}
		}
	}

	for (int k = 0; k < numberOfPaths; k++)
	{
		finalPaths.push_back(listOfAllPaths[k]);
	}

	return  finalPaths;
}
// Driver program 
#endif 