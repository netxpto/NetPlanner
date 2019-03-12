# include "..\include_opaque\logical_topology_generator_20190216.h"


void LogicalTopologyGenerator::initialize(void) {

	setNumberOfNodes(calculateNumberOfNodes());
	setNumberOfLinks(calculateNumberOfLinks());

	numberOfNodes = getNumberOfNodes();
	numberOfLinks = getNumberOfLinks();
	index = getIndex();

}

bool LogicalTopologyGenerator::runBlock(void) {
	
	if (!generate)
		return false;
	else
		generate = false;

	if (transportMode == "opaque")
	{
		t_logical_topology outputLogicalTopologyOpaque;
		outputLogicalTopologyOpaque.logicalTopology = physicalTopology;

		t_integer_long space = outputSignals[0]->space();	// Buffer free space 

		int process = std::min(space, numberOfLinks);

		if (process == 0) return false;						

		for (int k = 1; k <= process; k++)
		{
			t_logical_link outputLogicalLinkOpaque;
			generateLogicalLinkOpaque(outputLogicalLinkOpaque);

			outputLogicalTopologyOpaque.logicalLinks.push_back(outputLogicalLinkOpaque);
		}

		outputSignals[0]->bufferPut((t_logical_topology)outputLogicalTopologyOpaque);
	}

	else {

		std::cout << "Error: logical_topology_generator_20190216.h - Transport Mode not opaque\n";
	}
	
	return true;
};

bool LogicalTopologyGenerator::generateLogicalLinkOpaque(t_logical_link &lLink) {

	bool findLogicalLink{ false };

	t_integer line{ 0 };
	while ((line < numberOfNodes) && (!findLogicalLink))
	{
		t_integer column{ 0 };
		while ((column < numberOfNodes) && (!findLogicalLink))
		{
			if (physicalTopology[line][column] == 1)
			{
				findLogicalLink = true;

				lLink.index = index;
				lLink.linkSourceNode = { line + 1 };
				lLink.linkDestinationNode = { column + 1 };
				lLink.numberOfLightPaths = 0;

				physicalTopology[line][column]--;
				index++;
			}
			column++;
		}
		line++;
	}
	return findLogicalLink;
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