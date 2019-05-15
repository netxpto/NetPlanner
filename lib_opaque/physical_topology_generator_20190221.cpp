#include "..\include_opaque\physical_topology_generator_20190221.h"

void PhysicalTopologyGenerator::initialize(void) {}

bool PhysicalTopologyGenerator::runBlock(void)
{
	if (!generate)
		return false;
	else
		generate = false;

	t_physical_topology outputPhysicalTopology;
	t_optical_multiplexing_system outputOpticalMultiplexingSystem;

	outputPhysicalTopology.physicalTopologyAdjacencyMatrix = physicalTopologyAdjacencyMatrix;

	t_integer index = 0;

	for (size_t i = 0; i < physicalTopologyAdjacencyMatrix[0].size(); i++)
	{
		for (size_t j = 0; j < physicalTopologyAdjacencyMatrix[0].size(); j++)
		{
			double wavelength = initialWavelength;

			if (physicalTopologyAdjacencyMatrix[i][j] == 1)
			{
				outputOpticalMultiplexingSystem.OMSIndex = index;
				outputOpticalMultiplexingSystem.sourceNode = i + 1;
				outputOpticalMultiplexingSystem.destinationNode = j + 1;
				outputOpticalMultiplexingSystem.maximumNumberOfWavelengths = numberOfOpticalChannelsPerOMS;

				for (t_integer k = 0; k < numberOfOpticalChannelsPerOMS; k++)
				{
					outputOpticalMultiplexingSystem.wavelengths.push_back(wavelength);
					if (k < numberOfOpticalChannelsPerOMS - 1)
						wavelength += wavelengthSpacing;

					outputOpticalMultiplexingSystem.availableWavelengths.push_back(1);
				}

				outputPhysicalTopology.OMS.push_back(outputOpticalMultiplexingSystem);

				outputOpticalMultiplexingSystem.wavelengths.clear();
				outputOpticalMultiplexingSystem.availableWavelengths.clear();
				index++;
			}
		}
	}

	outputSignals[0]->bufferPut((t_physical_topology)outputPhysicalTopology);

	return true;
};


