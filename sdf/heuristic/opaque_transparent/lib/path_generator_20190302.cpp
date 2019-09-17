# include "..\include\path_generator_20190302.h"
# include "..\include\Dijkstra_20190309.h"


using namespace std;

void PathGenerator::initialize(void) {

	numberOfOutputSignals = (int)outputSignals.size();
	numberOfOutputSignals = (int)inputSignals.size();
}

bool PathGenerator::runBlock(void) {

	// Components of a "DemandListOfPaths" signal
	t_demand demand;
	//std::vector<t_path> Paths;
	t_demand_list_of_paths demandListOfPaths;

	// Logical topology
	t_logical_topology logicalTopology;

	t_integer pathIndex{ 0 };

	t_integer process1 = inputSignals[0]->ready(); // Number of demands in the buffer ready to be processed
	t_integer process2 = inputSignals[1]->ready(); // Logical topology available

	if (process1 == 0 || process2 == 0) return false;

	inputSignals[1]->bufferGet(&logicalTopology); // Logical topology signal

//#################### SAVES STATE VARIABLES ########################################################################

	currentLogicalTopology.logicalTopology = logicalTopology.logicalTopology; // Saves initial logical topology matrix
	currentLogicalTopology.logicalLinks = logicalTopology.logicalLinks; // Saves logical links

//#################### CREATE A GRAPH ACCORDING TO OUR LOGICAL TOPOLOGY FOR DIJKSTRA ALGORITHM ######################

	Graph g(logicalTopology.logicalTopology[0].size()); // Creates a graph with a certain number of nodes

	for (t_integer line=0; line<logicalTopology.logicalTopology[0].size(); line++)
	{
		for (t_integer column = 0; column < logicalTopology.logicalTopology[0].size(); column++)
		{
			if (logicalTopology.logicalTopology[line][column] == 1)
			{
				g.addEdge(line, column); // creates a logical connection
			}
		}
	}

//################### PROCESS A DEMAND ###############################################################################

	for (t_integer j = 0; j < process1; j++) // Processes demands one by one
	{
		t_integer source{0};
		t_integer destination{0};
		std::vector<std::vector<t_integer>> shortestPaths{0}; // Shortest paths given by Dijkstra 

		inputSignals[0]->bufferGet(&demand);

		source = demand.sourceNode;				// Demand source node
		destination = demand.destinationNode;	// Demand destination node

		g.printAllPaths(source, destination);
		//cout << "Shortest  " << numberOfPaths << "  paths " << endl;
		//cout << endl;
		shortestPaths = g.printFinalPaths(numberOfPaths);	// Vector with the shortest paths selected

//################### CREATES Path SIGNALS ############################################################################
		std::vector<t_path> Paths;
		//t_path Path;

		for (t_integer k = 0; k < numberOfPaths; k++)		// All paths (one by one)
		{
			t_path Path;
			Path.pathIndex = pathIndex;			// Path index
			Path.sourceNode = source;			// Path source node
			Path.destinationNode = destination;	// Path destination node
			// Creates a vector of logicalLinks (in the begining all to 0)
			std::vector<t_integer> LogicalLinks(logicalTopology.logicalLinks.size() / 4, 0);
//################# ANALYZE INDIVIDUALLY EACH OF THE SELECTED SHORTEST PATHS ###################################

			for (t_integer i = 0; i < shortestPaths[k].size()-1; i++) // Analyze each individual path of final selected shortest paths
			{
				t_integer src;
				t_integer dst;
				// Nodes go through 1 to 6
				src = (shortestPaths[k][i])+1;		// Path's LINK source NODE
				dst = (shortestPaths[k][i + 1])+1;	// Path's LINK destination NODE
				std::vector<t_integer> index;	// Contains the used logical links index


				t_integer l{ 0 };
				bool indexFound{ false };

				while (l < (logicalTopology.logicalLinks.size() / 4) && !indexFound) // Iterates through all rows of logical links to find one with the same source and destination nodes
				{
					if (src == logicalTopology.logicalLinks[l].linkSourceNode && dst == logicalTopology.logicalLinks[l].linkDestinationNode)
					{
						indexFound = true;
						index.push_back(logicalTopology.logicalLinks[l].linkIndex);
					}
					l++;
				}
	
				// Preencher matriz de logical links do path com 0s e 1s
				for (t_integer x = 0; x < LogicalLinks.size(); x++)
				{
					t_integer d{ 0 };
					bool found{ false };
					while ( d<index.size() && !found)
					{
						if (x == index[d])
						{
							LogicalLinks[x] = 1;
							found = true;
						}
						d++;
					}
					if (!found)
					{
						LogicalLinks[x] = 0;
					}
				}
				
			};
			Path.logicalLinks = LogicalLinks;

//######################## CALCULATE NUMBER OF HOPS ##################################################################
			t_integer sum{ 0 };
			
			for (t_integer j = 0; j < Path.logicalLinks.size(); j++)
			{
				sum = sum + Path.logicalLinks[j];
			}
			Path.hops = sum;
			Paths.push_back(Path);
			pathIndex++;
		}

//################### CREATE A DemandListOfPaths SIGNAL ##############################################################

		demandListOfPaths.demand = demand;
		demandListOfPaths.selectedPaths=Paths;

		outputSignals[0]->bufferPut((t_demand_list_of_paths)demandListOfPaths);
 	}
	
	
	//else if (demandsReady == 0) return false;

	/*for (int i = 0; i < process; i++) {
		inputSignals[0]->bufferGet();
		++processedSamples;
	}

	if (numberOfSamples >= 0) numberOfSamples -= process;

	if (displayNumberOfSamples) {
		cout << "Samples to be processed now: " << process << "\n";
		cout << "Samples to be processed later: " << numberOfSamples << "\n";
		cout << "Samples already processed: " << processedSamples << "\n";
	}
	*/
	return true;
};
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
	
////////////////////////////////////////////////////////////////////////////////////////////
}

void Graph::addEdge(int u, int v)
{
	adj[u].push_back(v); // Add v to u’s list. 
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
		list<int>::iterator i = adj[u].begin();
		for (i ; i != adj[u].end(); ++i)
			if (!visited[*i])
				printAllPathsUtil(*i, d, visited, path, path_index);
	}

	// Remove current vertex from path[] and mark it as unvisited 
	path_index--;
	visited[u] = false;


}

vector<vector<int>> Graph::printFinalPaths(int numberOfPaths)
{

	//int hops;
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

	/*for (int a = 0; a < finalPaths.size(); a++)
	{
		hops = finalPaths[a].size() - 1;
		for (int b = 0; b < finalPaths[a].size(); b++)
		{
			cout << finalPaths[a][b] << " ";
		}
		cout << "    " << hops << endl;
	}
	cout << endl;*/
	return  finalPaths;
}
Graph::Graph(int V)
{
	this->V = V;
	adj = new list<int>[V];
}

