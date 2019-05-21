#include "..\include_opaque\logical_topology_generator_20190216.h"

void LogicalTopologyGenerator::initialize(void)
{
	numberOfOutputSignals = (int)outputSignals.size();

	transportMode = getTransportMode();
	physicalTopologyAdjacencyMatrix = getPhysicalTopologyAdjacencyMatrix();
	distanceMatrix = getDistanceMatrix();
}

bool LogicalTopologyGenerator::runBlock(void) 
{
	if (!generate)
		return false;
	else
		generate = false;

	t_logical_topology logicalTopology;

	logicalTopology.distanceMatrix = distanceMatrix;

	if (transportMode == t_transport_mode::opaque) 
		logicalTopology.logicalTopologyAdjacencyMatrix = physicalTopologyAdjacencyMatrix;
	
	else
		std::cout << "Error: logical_topology_generator_20190216.h - Transport Mode not opaque\n";

	outputSignals[0]->bufferPut((t_logical_topology)logicalTopology);

	return true;
};
