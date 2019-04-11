#include "..\include_opaque\physical_topology_generator_20190221.h"


void PhysicalTopologyGenerator::initialize(void) {}


bool PhysicalTopologyGenerator::runBlock(void) {

	if (!generate)
		return false;
	else
		generate = false;

	t_physical_topology outputPhysicalTopology;
	outputPhysicalTopology.physicalTopology = adjacencyMatrix;
	t_optical_multiplexing_system outputOpticalMultiplexingSystem;
	
	t_integer index{ 0 };
	
	t_integer line{ 0 };
	t_integer nodes = adjacencyMatrix[0].size();

	for (t_integer w = 0; w < opticalChannels; w++) {
		outputOpticalMultiplexingSystem.wavelengths.push_back(1);
	}

	while (line < nodes) {

		t_integer column{ 0 };
		while (column < nodes) {

			if (adjacencyMatrix[line][column] == 1) {

				outputOpticalMultiplexingSystem.OMSIndex = index;
				outputOpticalMultiplexingSystem.sourceNode = { line + 1 };
				outputOpticalMultiplexingSystem.destinationNode = { column + 1 };
				outputOpticalMultiplexingSystem.maximumNumberOfWavelengths = opticalChannels;
				outputOpticalMultiplexingSystem.wavelengths;
				
				outputPhysicalTopology.OMS.push_back(outputOpticalMultiplexingSystem);

				adjacencyMatrix[line][column]--;
				index++;
			}
			column++;
		}
		line++;
	}
	
	outputSignals[0]->bufferPut((t_physical_topology)outputPhysicalTopology);	
		
	return true;
};


