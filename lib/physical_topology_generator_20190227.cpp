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
	t_physical_link outputLinks;
	t_optical_channel outputChannels;
	t_integer k{ 0 };


	for (t_integer line=0; line < (t_integer) physicalTopology[0].size(); line++)
	{
		for (t_integer column=0; column < (t_integer) physicalTopology[0].size(); column++)
		{
//################ CREATES PHYSICAL LINKS #######################################################
			if (physicalTopology[line][column] == 1)
			{
				outputLinks.linkIndex = k;
				outputLinks.linkSourceNode = line + 1;
				outputLinks.linkDestinationNode = column + 1;
				outputLinks.numberOfOpticalChannels = getOpticalChannels();
				output.physicalLinks.push_back(outputLinks);

//#################### CREATES THE OPTICAL CHANNELS OF EACH PHYSICAL LINK #######################
				for (t_integer channel = 0; channel < opticalChannelsPerTransportSystem; channel++)
				{
					outputChannels.linkIndex = k;
					outputChannels.opticalChannelNumber = channel+1;
					outputChannels.capacity = opticalChannelCapacity;
					outputChannels.wavelengtht = 0; // "O" means its not being used any wavelength in this optical channel
					outputChannels.sourceNode = line + 1;
					outputChannels.destinationNode = column + 1;
					output.opticalChannels.push_back(outputChannels);
				}
				k++;
			}
		}
	}
	outputSignals[0]->bufferPut((t_physical_topology)output);
	return true;
}