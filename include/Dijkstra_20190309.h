#pragma once
# ifndef DIJKSTRA_H_
# define DIJKSTRA_H_



# include <vector>
# include <complex>
# include <fstream>
# include <iostream>
# include <math.h>
# include <stdio.h>
# include <string>
# include <strstream>
# include <vector>
# include <algorithm> 
# include <random>
# include <chrono>


# include "..\include\netxpto_20190130.h"
using namespace std;



class Graph : public Block {

	int V; // No. of vertices in graph 
	list<int> *adj; // Pointer to an array containing adjacency lists 
	vector<vector<int>> listOfAllPaths; // List containing all possible paths

	// A recursive function used by printAllPaths() 
	void printAllPathsUtil(int, int, bool[], int[], int &);

public:

	Graph(int V); // Constructor 
	void addEdge(int u, int v);
	void printAllPaths(int s, int d);
	vector<vector<int>> printFinalPaths(int numberOfPaths);
};

# endif
