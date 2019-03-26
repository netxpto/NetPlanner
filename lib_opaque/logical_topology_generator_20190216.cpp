# include "..\include_opaque\logical_topology_generator_20190216.h"


void LogicalTopologyGenerator::initialize(void) {}

bool LogicalTopologyGenerator::runBlock(void) {
	
	if (!generate)
		return false;
	else
		generate = false;

	if (transportMode == "opaque") 

		outputSignals[0]->bufferPut((t_matrix)adjacencyMatrix);

	else {

		std::cout << "Error: logical_topology_generator_20190216.h - Transport Mode not opaque\n";

	}
	
	return true;

};
