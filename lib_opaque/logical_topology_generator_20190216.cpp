# include "..\include_opaque\logical_topology_generator_20190216.h"


void LogicalTopologyGenerator::initialize(void) {}

bool LogicalTopologyGenerator::runBlock(void) {
	
	if (!generate)
		return false;
	else
		generate = false;

	if (transportMode == "opaque") {

		t_logical_topology outputLogicalTopology;

		outputLogicalTopology.logicalTopologyAdjacencyMatrix = physicalTopologyAdjacencyMatrix;

		outputSignals[0]->bufferPut((t_logical_topology) outputLogicalTopology);
	}

	else 
		std::cout << "Error: logical_topology_generator_20190216.h - Transport Mode not opaque\n";

	
	return true;

};
