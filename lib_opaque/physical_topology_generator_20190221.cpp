#include "..\include_opaque\physical_topology_generator_20190221.h"

void PhysicalTopologyGenerator::initialize(void)
{
	numberOfOutputSignals = (int)outputSignals.size();

	physicalTopologyAdjacencyMatrix = getPhysicalTopologyAdjacencyMatrix();
	distanceMatrix = getDistanceMatrix();
	numberOfOpticalChannelsPerOMS = getNumberOfOpticalChannelsPerOMS();
	span = getSpan();
}

bool PhysicalTopologyGenerator::runBlock(void)
{
	if (!generate)
		return false;
	else
		generate = false;

	t_physical_topology physicalTopology;
	t_optical_multiplexing_system opticalMultiplexingSystem;

	physicalTopology.physicalTopologyAdjacencyMatrix = physicalTopologyAdjacencyMatrix;
	physicalTopology.distanceMatrix = distanceMatrix;

	t_integer index = 0;

	for (size_t i = 0; i < physicalTopologyAdjacencyMatrix[0].size(); i++)
	{
		for (size_t j = 0; j < physicalTopologyAdjacencyMatrix[0].size(); j++)
		{
			double wavelength = initialWavelength;

			if (physicalTopologyAdjacencyMatrix[i][j] == 1)
			{
				opticalMultiplexingSystem.OMSIndex = index;
				opticalMultiplexingSystem.sourceNode = i + 1;
				opticalMultiplexingSystem.destinationNode = j + 1;
				opticalMultiplexingSystem.maximumNumberOfWavelengths = numberOfOpticalChannelsPerOMS;

				for (t_integer k = 0; k < numberOfOpticalChannelsPerOMS; k++)
				{
					opticalMultiplexingSystem.wavelengths.push_back(wavelength);
					if (k < numberOfOpticalChannelsPerOMS - 1)
						wavelength += wavelengthSpacing;

					opticalMultiplexingSystem.availableWavelengths.push_back(1);
				}

				if (distanceMatrix[i][j] % span != 0)
					opticalMultiplexingSystem.numberOfAmplifiers = distanceMatrix[i][j] / span;
				else 
					opticalMultiplexingSystem.numberOfAmplifiers = distanceMatrix[i][j] / span - 1;

				physicalTopology.OMS.push_back(opticalMultiplexingSystem);

				opticalMultiplexingSystem.wavelengths.clear();
				opticalMultiplexingSystem.availableWavelengths.clear();
				index++;
			}
		}
	}

	outputSignals[0]->bufferPut((t_physical_topology)physicalTopology);

	return true;
};


