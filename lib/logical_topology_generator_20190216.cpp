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

		outputSignals[0]->bufferPut((t_logical_topology) Opaque);

	}	
	else if (transportMode == transport_mode::transparent)  // Considering all nodes transparent
	{
		t_logical_topology Transparent;

//######### CREATES A TRANSPARENT LOGICAL TOPOLOGY ADJACENCY MATRIX ########################

		Transparent.logicalTopologyAdjacencyMatrix.resize(physicalTopologyAdjacencyMatrix[0].size(), vector<int>(physicalTopologyAdjacencyMatrix[0].size(), 1));
		for (size_t k = 0; k < physicalTopologyAdjacencyMatrix[0].size(); k++) { Transparent.logicalTopologyAdjacencyMatrix[k][k] = 0; }

// CREATE A PATH, LIGHTPATH AND OPTICAL CHANNEL FOR TEST PURPOSES
	/*	t_paths test1;
		test1.pathIndex = 20;
		test1.sourceNode = 1;
		test1.destinationNode = 5;
		test1.capacity = 48;
		test1.numberOfLightPaths = 1;
		test1.lightPathsIndex = {0};
		t_light_paths test2;
		test2.lightPathIndex = 0;
		test2.sourceNode = 1;
		test2.destinationNode = 5;
		test2.capacity = test1.capacity;
		test2.numberOfOpticalChannels = 2;
		test2.opticalChannelsIndex = { 0,1 };
		t_optical_channels test3;
		t_optical_channels test4;
		test3.opticalChannelIndex = 0;
		test4.opticalChannelIndex = 1;
		test3.sourceNode = 1;
		test4.sourceNode = 6;
		test3.destinationNode = 6;
		test4.destinationNode = 5;
		test3.capacity = test1.capacity;
		test4.capacity = test1.capacity;
		test3.wavelenght = 1550;
		test4.wavelenght = 1550;
		test3.numberOfDemands = 1;
		test4.numberOfDemands = 1;
		test3.demandsIndex = { 1 };
		test4.demandsIndex = { 1 };
		Transparent.paths.push_back(test1);
		Transparent.lightPaths.push_back(test2);
		Transparent.opticalChannels.push_back(test3);
		Transparent.opticalChannels.push_back(test4);*/

		outputSignals[0]->bufferPut((t_logical_topology) Transparent);
		
	}
	else
	{
		cout << "Error: logical_Top_Gen_20190216.h - Transport Mode not defined\n";
	}

	return true;
};