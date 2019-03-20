#include "..\include_opaque\physical_topology_generator_20190221.h"


void PhysicalTopologyGenerator::initialize(void) {}


bool PhysicalTopologyGenerator::runBlock(void) {

	if (!generate)
		return false;
	else
		generate = false;

	t_physical_topology outputPhysicalTopology;
	t_physical_link outputPhysicalLink;
	t_optical_channel outputOpticalChannel;
	
	t_integer index{ 0 };
	
	t_integer line{ 0 };
	t_integer nodes = adjacencyMatrix[0].size();

	while (line < nodes) {

		t_integer column{ 0 };
		while (column < nodes) {

			if (adjacencyMatrix[line][column] == 1) {

				outputPhysicalLink.index = index;
				outputPhysicalLink.linkSourceNode = { line + 1 };
				outputPhysicalLink.linkDestinationNode = { column + 1 };
				outputPhysicalLink.numberOfOpticalChannels = opticalChannels;
				outputPhysicalTopology.physicalLinks.push_back(outputPhysicalLink);

				adjacencyMatrix[line][column]--;
				index++;

				t_integer channel{ 0 };
				while (channel < opticalChannels) {

					outputOpticalChannel.linkIndex = { index - 1 };
					outputOpticalChannel.opticalChannelNumber = { channel + 1 };
					outputOpticalChannel.capacity = opticalChannelCapacity;
					outputOpticalChannel.wavelenght = { 0 };
					outputPhysicalTopology.opticalChannels.push_back(outputOpticalChannel);

					channel++;
				}
			}
			column++;
		}
		line++;
	}
	
	outputSignals[0]->bufferPut((t_physical_topology)outputPhysicalTopology);	
		
	return true;
};


