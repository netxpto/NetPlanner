#include "..\include_opaque\logical_topology_generator_20190216.h"

void LogicalTopologyGenerator::initialize(void) {}

bool LogicalTopologyGenerator::runBlock(void) 
{
	if (!generate)
		return false;
	else
		generate = false;

	t_logical_topology outputLogicalTopology;

	if (transportMode == "opaque") 
		outputLogicalTopology.logicalTopologyAdjacencyMatrix = physicalTopologyAdjacencyMatrix;
	
	
	else if (transportMode == "transparent")
	{
		outputLogicalTopology.logicalTopologyAdjacencyMatrix.resize(physicalTopologyAdjacencyMatrix[0].size(), vector<t_integer>(physicalTopologyAdjacencyMatrix[0].size(), 1));

		for (size_t i = 0; i < physicalTopologyAdjacencyMatrix[0].size(); i++) 
		{
			outputLogicalTopology.logicalTopologyAdjacencyMatrix[i][i] = 0; 
		}
	}
	
	else
		std::cout << "Error: logical_topology_generator_20190216.h - Transport Mode not defined\n";

	outputSignals[0]->bufferPut((t_logical_topology)outputLogicalTopology);

	return true;
};
