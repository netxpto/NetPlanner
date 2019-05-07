# include "..\include\physical_topology_generator_20190227.h"

//using namespace std;

void PhysicalTopologyGenerator::initialize(void) {
	//numberOfOutputSignals = (int)outputSignals.size();
}

bool PhysicalTopologyGenerator::runBlock(void) {

	if (!generate)
		return false;
	else
		generate = false;

		t_physical_topology output;
		t_optical_multiplexing_systems oms; // Optical Multiplexing Systems
		t_integer k{ 0 };


		output.physicalTopologyAdjacencyMatrix = getPhysicalTopologyAdjacencyMatrix();

		for (t_integer line = 0; line < (t_integer)physicalTopologyAdjacencyMatrix[0].size(); line++)
		{
			for (t_integer column = 0; column < (t_integer)physicalTopologyAdjacencyMatrix[0].size(); column++)
			{
				//################ CREATES OPTICAL MULTIPLEXING SYSTEMS #######################################################

				if (physicalTopologyAdjacencyMatrix[line][column] == 1)
				{
					double addWavelenght{ 0 };

					oms.opticalMultiplexingSystemIndex = k;
					oms.sourceNode = line + 1;
					oms.destinationNode = column + 1;
					oms.numberOfWavelenghts = getNumberOfOpticalChannelsPerOMS();

					// wavelenghts vector
					for (auto i = 0; i < numberOfOpticalChannelsPerOMS; i++)
					{
						if (i == 0)	// In the beggining it adds our initial wavelenght to the wavelnghts vector
						{
							addWavelenght = getInitialWavelenght();
							oms.wavelenghts.push_back(addWavelenght);
						}
						else
						{
							oms.wavelenghts.push_back(addWavelenght);
						}
						addWavelenght = addWavelenght + wavelenghtSpacing;
						oms.availableWavelenghts.push_back(1);
					}

					output.opticalMultiplexingSystems.push_back(oms);
					oms.wavelenghts.clear();
					oms.availableWavelenghts.clear();
					k++;
				}
			}
		}

		outputSignals[0]->bufferPut((t_physical_topology)output);
		return true;
};