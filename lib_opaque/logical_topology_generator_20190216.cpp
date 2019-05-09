#include "..\include_opaque\logical_topology_generator_20190216.h"


void LogicalTopologyGenerator::initialize(void) 
{

}

bool LogicalTopologyGenerator::runBlock(void) 
{
	if (!generate)
		return false;
	else
		generate = false;

	t_logical_topology outputLogicalTopology;
	t_path path;
	t_light_path lightPath;
	t_optical_channel opticalChannel;

	if (transportMode == "opaque") {
		/*
		path.pathIndex = 0;
		path.sourceNode = 1;
		path.destinationNode = 3;
		path.capacity = 50;
		path.numberOfLightPaths = 2;
		path.lightPathsIndex.push_back(0);
		path.lightPathsIndex.push_back(1);
		outputLogicalTopology.paths.push_back(path);
		path.lightPathsIndex.clear();

		lightPath.lightPathIndex = 0;
		lightPath.sourceNode = 1;
		lightPath.destinationNode = 2;
		lightPath.capacity = 50;
		lightPath.numberOfOpticalChannels = 1;
		lightPath.opticalChannelsIndex.push_back(0);
		outputLogicalTopology.lightPaths.push_back(lightPath);
		lightPath.opticalChannelsIndex.clear();

		lightPath.lightPathIndex = 1;
		lightPath.sourceNode = 2;
		lightPath.destinationNode = 3;
		lightPath.capacity = 50;
		lightPath.numberOfOpticalChannels = 1;
		lightPath.opticalChannelsIndex.push_back(1);
		outputLogicalTopology.lightPaths.push_back(lightPath);
		lightPath.opticalChannelsIndex.clear();

		opticalChannel.opticalChannelIndex = 0;
		opticalChannel.sourceNode = 1;
		opticalChannel.destinationNode = 2;
		opticalChannel.capacity = 50;
		opticalChannel.wavelength = 1550;
		opticalChannel.numberOfDemands = 0;
		outputLogicalTopology.opticalChannels.push_back(opticalChannel);

		opticalChannel.opticalChannelIndex = 1;
		opticalChannel.sourceNode = 2;
		opticalChannel.destinationNode = 3;
		opticalChannel.capacity = 50;
		opticalChannel.wavelength = 1550;
		opticalChannel.numberOfDemands = 0;
		outputLogicalTopology.opticalChannels.push_back(opticalChannel);
		*/
		outputLogicalTopology.logicalTopologyAdjacencyMatrix = physicalTopologyAdjacencyMatrix;
	}
	
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
