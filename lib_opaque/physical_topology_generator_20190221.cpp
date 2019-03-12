#include "..\include_opaque\physical_topology_generator_20190221.h"

void PhysicalTopologyGenerator::initialize(void) {

}

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

	while (line < physicalTopology[0].size())
	{
		t_integer column{ 0 };
		while (column < physicalTopology[0].size())
		{
			if (physicalTopology[line][column] == 1)
			{
				outputPhysicalLink.index = index;
				outputPhysicalLink.linkSourceNode = { line + 1 };
				outputPhysicalLink.linkDestinationNode = { column + 1 };
				outputPhysicalLink.numberOfOpticalChannels = opticalChannels;
				outputPhysicalTopology.physicalLinks.push_back(outputPhysicalLink);

				physicalTopology[line][column]--;
				index++;

				t_integer channel{ 0 };
				while (channel < opticalChannels)
				{
					outputOpticalChannel.linkIndex = { index - 1 };
					outputOpticalChannel.opticalChannelNumber = { channel + 1 };
					outputOpticalChannel.capacity = opticalChannelCapacity;
					outputOpticalChannel.wavelenght = { 0 };
					outputOpticalChannel.sourceNode = { line + 1 };
					outputOpticalChannel.destinationNode = { column + 1 };
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

/*
bool PhysicalTopologyGenerator::generatePhysicalLink(t_physical_link &pLink) {

	bool findPhysicalLink{ false };

	t_integer line{ 0 };
	while ((line < numberOfNodes) && (!findPhysicalLink)) 
	{
		t_integer column{ 0 };
		while ((column < numberOfNodes) && (!findPhysicalLink)) 
		{
			if (physicalTopology[line][column] == 1) 
			{
				findPhysicalLink = true;

				pLink.index = index;
				pLink.linkSourceNode = { line + 1 };
				pLink.linkDestinationNode = { column + 1 };
				pLink.numberOfOpticalChannels = opticalChannels;

				physicalTopology[line][column]--;
				index++;
			}
			column++;
		}
		line++;
	}
	return findPhysicalLink;
}

bool PhysicalTopologyGenerator::generateOpticalChannel(t_optical_channel &oChannel) {

	bool findOpticalChannel{ false };

	t_integer linkIndex{ 0 };
	while ((linkIndex < numberOfLinks) && (!findOpticalChannel)) 
	{
		int channel = 0;
		while ((channel < opticalChannels) && (!findOpticalChannel)) 
		{
			findOpticalChannel = true;

			oChannel.linkIndex = linkIndex;
			oChannel.opticalChannelNumber = { channel + 1 };
			oChannel.capacity = opticalChannelCapacity;
			oChannel.wavelenght = { 0 };
			oChannel.sourceNode = { 0 };
			oChannel.destinationNode = { 0 };
			
			channel++;
		}
		linkIndex++;
	}
	return findOpticalChannel;
}


t_integer PhysicalTopologyGenerator::calculateNumberOfNodes() {

	t_integer nodes = physicalTopology[0].size();
	
	return nodes;
};

t_integer PhysicalTopologyGenerator::calculateNumberOfLinks() {

	t_integer links{ 0 };

	for (t_integer line = 0; line < numberOfNodes; line++) 
	{
		for (t_integer column = 0; column < numberOfNodes ; column++) 
		{
			links += physicalTopology[line][column];
		}

	}
	return links;
};
*/