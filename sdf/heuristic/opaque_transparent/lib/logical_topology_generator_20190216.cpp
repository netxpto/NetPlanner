# include "..\include\logical_topology_generator_20190216.h"

//using namespace std;

void LogicalTopologyGenerator::initialize(void) {

}

bool LogicalTopologyGenerator::runBlock(void) {
	

	if (!generate)
		return false;
	else 
		generate = false;

	if (transportMode == transport_mode::opaque)
	{
		t_logical_topology Opaque;
		// The other structures of a LogicalTopology signal are sent void to the LogicalTopologyManager_ block
		 
		Opaque.logicalTopologyAdjacencyMatrix = physicalTopologyAdjacencyMatrix; 
		Opaque.transportMode = transport_mode::opaque;

		outputSignals[0]->bufferPut((t_logical_topology) Opaque);

	}	
	else if (transportMode == transport_mode::transparent)  // Considering all nodes transparent
	{
		t_logical_topology Transparent;

//######### CREATES A TRANSPARENT LOGICAL TOPOLOGY ADJACENCY MATRIX ########################

		Transparent.logicalTopologyAdjacencyMatrix.resize(physicalTopologyAdjacencyMatrix[0].size(), vector<int>(physicalTopologyAdjacencyMatrix[0].size(), 1));
		for (size_t k = 0; k < physicalTopologyAdjacencyMatrix[0].size(); k++) { Transparent.logicalTopologyAdjacencyMatrix[k][k] = 0; }

		Transparent.transportMode = transport_mode::transparent;

		outputSignals[0]->bufferPut((t_logical_topology) Transparent);
		
	}
	else
	{
		cout << "Error: logical_Top_Gen_20190216.h - Transport Mode not defined\n";
	}

	return true;
};