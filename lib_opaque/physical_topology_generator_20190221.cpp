#include "..\include_opaque\physical_topology_generator_20190221.h"

using namespace std;

void PhysicalTopologyGenerator::initialize(void) {

	numberOfOutputSignals = (int)outputSignals.size();
	
	setNumberOfNodes(calculateNumberOfNodes());
	setNumberOfLinks(calculateNumberOfLinks());

	numberOfNodes = getNumberOfNodes();
	numberOfLinks = getNumberOfLinks();
	index = getIndex();
}

bool PhysicalTopologyGenerator::runBlock(void) {

	t_integer_long space = outputSignals[0]->space();		// Buffer free space 
	
	//t_integer_long z = (numberOfLinks * ( 1 + opticalChannels));
	//t_integer_long z = (numberOfLinks * opticalChannels);
	int process = std::min(space, numberOfLinks);

	if (process == 0) return false;						

	for (int k = 1; k <= process; k++) 
	{
		t_physical_link outputPhysicalLink;
		generatePhysicalLink(outputPhysicalLink);
		
		//t_optical_channel outputOpticalChannel;
		//generateOpticalChannel(outputOpticalChannel);

		//t_physical_topology outputPhysicalTopology;
		//outputPhysicalTopology.physicalLink = outputPhysicalLink;
		//outputPhysicalTopology.opticalLink = outputOpticalLink;

		//outputSignals[0]->bufferPut((t_physical_topology)outputPhysicalTopology);
		//outputSignals[0]->bufferPut((t_optical_channel)outputOpticalChannel);
		outputSignals[0]->bufferPut((t_physical_link)outputPhysicalLink);
		//z--;
		numberOfLinks--;
	}
	return true;
};

bool PhysicalTopologyGenerator::generatePhysicalLink(t_physical_link &pl) {

	bool findLink{ false };

	t_integer line{ 0 };
	while ((line < numberOfNodes) && (!findLink)) 
	{
		t_integer column{ 0 };
		while ((column < numberOfNodes) && (!findLink)) 
		{
			if (physicalTopology[line][column] == 1) 
			{
				findLink = true;

				pl.index = index;
				pl.linkSourceNode = { line + 1 };
				pl.linkDestinationNode = { column + 1 };
				pl.numberOfOpticalChannels = opticalChannels;

				physicalTopology[line][column]--;
				index++;
			}
			column++;
		}
		line++;
	}
	return findLink;
}

/*
bool PhysicalTopologyGenerator::generateOpticalChannel(t_optical_channel &och) {

	bool findOch{ false };

	t_integer linkIndex{ 0 };
	while ((linkIndex < numberOfLinks) && (!findOch)) 
	{
		int k = 0;
		while ((k < opticalChannels) && (!findOch)) 
		{
			findOch = true;

			och.linkIndex = linkIndex;
			och.opticalChannelNumber = { k + 1 };
			och.capacity = opticalChannelCapacity;
			och.wavelenght = { 0 };
			och.sourceNode = { 0 };
			och.destinationNode = { 0 };
			
			k++;
		}
		linkIndex++;
	}
	return findOch;
}
*/

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
