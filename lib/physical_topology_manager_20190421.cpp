# include "..\include\physical_topology_manager_20190421.h"


using namespace std;

/*// A directed graph using adjacency list representation 
class Graph
{
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

Graph::Graph(int V)
{
	this->V = V;
	adj = new list<int>[V];
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
		vector<int> newPath;

		for (int i = 0; i < path_index; i++)
		{
			//cout << path[i] << " ";
			newPath.push_back(path[i]);
		}
		listOfAllPaths.push_back(newPath);

		//cout << endl;

	}
	//}
	else // If current vertex is not destination 
	{
		// Recur for all the vertices adjacent to current vertex 
		list<int>::iterator i;
		for (i = adj[u].begin(); i != adj[u].end(); ++i)
			if (!visited[*i])
				printAllPathsUtil(*i, d, visited, path, path_index);
	}

	// Remove current vertex from path[] and mark it as unvisited 
	path_index--;
	visited[u] = false;


}

vector<vector<int>> Graph::printFinalPaths(int numberOfPaths)
{

	int hops;
	vector<int> temp;
	vector<vector<int>> finalPaths;


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
*/
void PhysicalTopologyManager::initialize(void) {

}

bool PhysicalTopologyManager::runBlock(void) {

	int process = inputSignals[0]->ready(); // number of samples in the buffer ready to be processed
	//int process2 = inputSignals[1]->ready(); // number of PathRequest signals in the buffer ready to be processed
	//t_path_request newPathRequest;

	if (process == 0 /*&& process2 == 0*/) return false;

	if (process != 0)
	{
		for (int i = 0; i < process; i++) {
			inputSignals[0]->bufferGet(&currentPhysicalTopology);
		}
		outputSignals[0]->bufferPut((t_physical_topology)currentPhysicalTopology);

		t_path_request_routed pathRequestRouted;
		t_light_paths_table newEntry;

		newEntry.sourceNode = 1;
		newEntry.destinationNode = 3;
		newEntry.numberOfIntermediateNodes = 1;
		newEntry.intermediateNodes = { 2 };
		newEntry.wavelenght = 1550;

		pathRequestRouted.pathInformation.requestIndex = 2;
		pathRequestRouted.pathInformation.demandIndex = 3;
		pathRequestRouted.pathInformation.oduType= 4;
		pathRequestRouted.pathInformation.numberOfLightPaths = 1;
		pathRequestRouted.pathInformation.routed = true;

		pathRequestRouted.lightPathsTable.push_back(newEntry);

		outputSignals[1]->bufferPut((t_path_request_routed) pathRequestRouted);
	}

	/*if (process2 != 0) // there are PathRequests in the buffer
	{
		for (int i = 0; i < process2; i++) {
			inputSignals[1]->bufferGet(&newPathRequest); // reads a PathRequest signal
			
			std::vector<std::vector<int>> listOfFinalPaths;

			// Generates shortest physical paths (Dijkstra algorithm)

			Graph g(currentPhysicalTopology.physicalTopologyAdjacencyMatrix[0].size());

			for (int line = 0; line < currentPhysicalTopology.physicalTopologyAdjacencyMatrix[0].size(); line++)
			{
				for (int column = 0; column < currentPhysicalTopology.physicalTopologyAdjacencyMatrix[0].size(); column++)
				{
					if (currentPhysicalTopology.physicalTopologyAdjacencyMatrix[line][column] == 1)
						g.addEdge(line, column);
				}
			}

			int sourceNode = newPathRequest.sourceNode;
			int destinationNode = newPathRequest.destinationNode;
			
			g.printAllPaths(sourceNode, destinationNode);
			listOfFinalPaths = g.printFinalPaths(blockingCriterionPhysicalTopology);

			std::vector<std::vector<double>> availableWavelenghtsIndexOfAllOMS;
			// Search through the vector that contains the shortest paths
			for (int i = 0; i < blockingCriterionPhysicalTopology; i++)
			{
				for (int j = 0; j < listOfFinalPaths[i].size()-1; j++)
				{
					for(double k = 0; k < currentPhysicalTopology.opticalMultiplexingSystems.size(); k++)
					{
						// Find an OMS with the same source and destination nodes
						if ((currentPhysicalTopology.opticalMultiplexingSystems[k].sourceNode == (listOfFinalPaths[i][j]+1) && currentPhysicalTopology.opticalMultiplexingSystems[k].destinationNode == (listOfFinalPaths[i][j + 1]+1)))
						{
							std::vector<double> OMSInformation;
							
							OMSInformation.push_back(k); // first element of the vector identifies the OMS analyzed

							for (int l = 0; l < currentPhysicalTopology.opticalMultiplexingSystems[k].availableWavelenghts.size(); l++)
							{
								if (currentPhysicalTopology.opticalMultiplexingSystems[k].availableWavelenghts[l] == 1)
									OMSInformation.push_back(currentPhysicalTopology.opticalMultiplexingSystems[k].wavelenghts[l]);
							}
							availableWavelenghtsIndexOfAllOMS.push_back(OMSInformation);
						}
					}
				}
			}

			// Analyze which wavelenghts are available in each OMS
			double wavelenghtSelected{0};
			bool wavelenghtFound{ false };

			for (int i = 1; i < availableWavelenghtsIndexOfAllOMS[0].size(); i++) // correr os elementos da primeira linha
			{
				for (int line = 1; line < availableWavelenghtsIndexOfAllOMS.size(); line++)
				{
					wavelenghtFound = false;

					for (int column = 1; column < availableWavelenghtsIndexOfAllOMS[line].size(); column++)
					{
						if (availableWavelenghtsIndexOfAllOMS[line][column] == availableWavelenghtsIndexOfAllOMS[0][i])
						{
							wavelenghtFound = true;
							wavelenghtSelected = availableWavelenghtsIndexOfAllOMS[line][column];
							break;
						}
					}

					if (wavelenghtFound = false) // if it did not found the wavelenght in one of the OMS
					{
						wavelenghtSelected = 0;
						break;
					}
				}
			}


			if (wavelenghtSelected == 0 && wavelenghtFound == false) // PathRequest was not successful
			{
				t_path_request_routed pathRequestRouted;
				pathRequestRouted.pathInformation.requestIndex = newPathRequest.requestIndex;
				pathRequestRouted.pathInformation.demandIndex = newPathRequest.demandIndex;
				pathRequestRouted.pathInformation.oduType = newPathRequest.oduType;
				pathRequestRouted.pathInformation.routed = false;
				pathRequestRouted.pathInformation.numberOfLightPaths = -1;

				outputSignals[1]->bufferPut((t_path_request_routed)pathRequestRouted);
			}
			else
			{
				// Update PhysicalTopologyManager_ block state variables
				for (int i = 0; i < availableWavelenghtsIndexOfAllOMS.size(); i++)
				{
					for (int j =0; j < currentPhysicalTopology.opticalMultiplexingSystems[availableWavelenghtsIndexOfAllOMS[i][0]].wavelenghts.size(); j++)
					{
						if (wavelenghtSelected == currentPhysicalTopology.opticalMultiplexingSystems[availableWavelenghtsIndexOfAllOMS[i][0]].wavelenghts[j])
							currentPhysicalTopology.opticalMultiplexingSystems[availableWavelenghtsIndexOfAllOMS[i][0]].availableWavelenghts[j] == 0;
					}
					
				}

				//Generate a PathRequestRouted signal
				t_path_request_routed pathRequestRouted;
				std::vector<int> intermeidateNodes;
				for (int i=1; i < availableWavelenghtsIndexOfAllOMS.size()-1; i++)
				{
					intermeidateNodes.push_back(availableWavelenghtsIndexOfAllOMS[i][0]);
				}

				pathRequestRouted.pathInformation.requestIndex = newPathRequest.requestIndex;
				pathRequestRouted.pathInformation.demandIndex = newPathRequest.demandIndex;
				pathRequestRouted.pathInformation.oduType = newPathRequest.oduType;
				pathRequestRouted.pathInformation.routed = true;
				pathRequestRouted.pathInformation.numberOfLightPaths = 1;

				pathRequestRouted.lightPathsTable[0].sourceNode = newPathRequest.sourceNode;
				pathRequestRouted.lightPathsTable[0].destinationNode = newPathRequest.destinationNode;
				pathRequestRouted.lightPathsTable[0].numberOfIntermediateNodes = availableWavelenghtsIndexOfAllOMS.size()-2;
				pathRequestRouted.lightPathsTable[0].intermediateNodes = intermeidateNodes;
				pathRequestRouted.lightPathsTable[0].wavelenght = wavelenghtSelected;

				outputSignals[1]->bufferPut((t_path_request_routed)pathRequestRouted);

			}
		}
	}*/
	return true;
};

