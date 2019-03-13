#include "..\include_opaque\path_generator_20190307.h"

#define numberOfNodes 6

void PathGenerator::initialize(void) {

	
}

bool PathGenerator::runBlock(void) {

	t_integer readyDemands = inputSignals[0]->ready();
	t_integer readyLogicalTopology = inputSignals[1]->ready();
	//t_integer readyRemovedPaths = inputSignals[2]->ready();

	int ready = min(readyDemands, readyLogicalTopology);

	int space = outputSignals[0]->space();

	int length = min(ready, space);

	if (length <= 0) return false;

	t_demand D;
	t_logical_topology L;
	t_demand_list_of_paths outputDemandListOfPaths;

	for (int i = 0; i < length; i++) {
		
		inputSignals[0]->bufferGet(&D);
		inputSignals[1]->bufferGet(&L);

		outputDemandListOfPaths.demand.demandIndex = D.demandIndex;
		outputDemandListOfPaths.demand.sourceNode = D.sourceNode;
		outputDemandListOfPaths.demand.destinationNode = D.destinationNode;
		outputDemandListOfPaths.demand.restorationMethod = D.restorationMethod;

		outputSignals[0]->bufferPut((t_demand_list_of_paths)outputDemandListOfPaths);
	}

	/*
	if (shortestPathType == "hops") {

		
	}
	*/
	
	return true;
};

t_integer PathGenerator::minDistance(t_integer dist[], bool prev[])
{
	t_integer min = INT_MAX;
	t_integer min_index;

	for (int node = 0; node < numberOfNodes; node++)
		
		if (prev[node] == false && dist[node] <= min) {

			min = dist[node];
			min_index = node;
		}
	return min_index;
};

t_integer PathGenerator::dijkstra(t_matrix logicalTopology , t_integer sourceNode) {
	
	t_integer dist[numberOfNodes];	 

	bool prev[numberOfNodes];  

	for (t_integer i = 0; i < numberOfNodes; i++) {
		
		dist[i] = INT_MAX; // Initialize all distances as INFINITE
		prev[i] = false;
	}
		
	dist[sourceNode] = 0; // Distance of source node from itself is always 0 
 
	for (t_integer count = 0; count < numberOfNodes - 1; count++) { // Find shortest path for all nodes 

		t_integer u = minDistance(dist, prev);

		prev[u] = true; // node processed 

		for (t_integer node = 0; node < numberOfNodes; node++) {

			if (!prev[node] && logicalTopology[u][node] && dist[u] != INT_MAX && dist[u] + logicalTopology[u][node] < dist[node])
				dist[node] = dist[u] + logicalTopology[u][node];
		}
	}	
	return dist[];
	return prev[];
};

/*
// Pseudocode
function YenKSP(Graph, source, sink, K) :

	// Determine the shortest path from the source to the sink.
	A[0] = Dijkstra(Graph, source, sink);

	// Initialize the set to store the potential kth shortest path.
	B = [];

	for k from 1 to K :
		
		// The spur node ranges from the first node to the next to last node in the previous k-shortest path.
		for i from 0 to size(A[k − 1]) − 2 :

			// Spur node is retrieved from the previous k-shortest path, k − 1.
			spurNode = A[k - 1].node(i);
			// The sequence of nodes from the source to the spur node of the previous k-shortest path.
			rootPath = A[k - 1].nodes(0, i);

			for each path p in A:
				if rootPath == p.nodes(0, i) :
					// Remove the links that are part of the previous shortest paths which share the same root path.
					remove p.edge(i, i + 1) from Graph;

			for each node rootPathNode in rootPath except spurNode :
				remove rootPathNode from Graph;

			// Calculate the spur path from the spur node to the sink.
			spurPath = Dijkstra(Graph, spurNode, sink);

			// Entire path is made up of the root path and spur path.
			totalPath = rootPath + spurPath;
			// Add the potential k-shortest path to the heap.
			B.append(totalPath);

			// Add back the edges and nodes that were removed from the graph.
			restore edges to Graph;
			restore nodes in rootPath to Graph;

	if B is empty :
		// This handles the case of there being no spur paths, or no spur paths left.
		// This could happen if the spur paths have already been exhausted (added to A), 
		// or there are no spur paths at all - such as when both the source and sink vertices 
		// lie along a "dead end".
		break;
	// Sort the potential k-shortest paths by cost.
	B.sort();
	// Add the lowest cost path becomes the k-shortest path.
	A[k] = B[0];
	B.pop();

return A;
*/