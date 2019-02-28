# include "..\include_opaque\logical_topology_generator_20190216.h"

using namespace std;

void LogicalTopologyGenerator::initialize(void) {

	setNumberOfNodes(calculateNumberOfNodes());
	setNumberOfLinks(calculateNumberOfLinks());

	numberOfNodes = getNumberOfNodes();
	numberOfLinks = getNumberOfLinks();
	index = getIndex();

}

bool LogicalTopologyGenerator::runBlock(void) {
	/*
	if (transportMode == "opaque" || transportMode == "OPAQUE") {
		
		t_logical_link outputLogicalLinkOpaque;
		generateLogicalLinkOpaque(outputLogicalLinkOpaque);

		outputSignals[0]->bufferPut((t_logical_link) outputLogicalLinkOpaque);
	}	
	
	else {
		
		cout << "Error: logical_topology_generator_20190216.h - Transport Mode not defined\n";
	}
	*/
	return true;
};

bool LogicalTopologyGenerator::generateLogicalLinkOpaque(t_logical_link &lLO) {

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

				lLO.index = index;
				lLO.linkSourceNode = { line + 1 };
				lLO.linkDestinationNode = { column + 1 };
				lLO.numberOfLightPaths = numberOfLightPaths;

				physicalTopology[line][column]--;
				index++;
			}
			column++;
		}
		line++;
	}
	return findLink;
}

t_integer LogicalTopologyGenerator::calculateNumberOfNodes() {

	t_integer nodes = physicalTopology[0].size();

	return nodes;
};

t_integer LogicalTopologyGenerator::calculateNumberOfLinks() {

	t_integer links{ 0 };

	for (t_integer line = 0; line < numberOfNodes; line++)
	{
		for (t_integer column = 0; column < numberOfNodes; column++)
		{
			links += physicalTopology[line][column];
		}

	}
	return links;
};