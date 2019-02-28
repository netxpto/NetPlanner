# include "..\include\logical_topology_generator_20190216.h"

//using namespace std;

void LogicalTopologyGenerator::initialize(void) {

}

bool LogicalTopologyGenerator::runBlock(void) {
	

	if (!generate)
		return false;
	else 
		generate = false;

	if (transportMode == transport_mode::opaque)
	{
		t_logical_topology outputOpaque;
		t_logical_links outputLinks;
		t_light_paths outputLights;
		t_integer k{ 0 };
		 

		outputOpaque.logicalTopology = physicalTopology;

// For each existent logical connection it creates a logical link structure 

		for (t_integer line = 0; line < outputOpaque.logicalTopology[0].size(); line++)
		{
			for (t_integer column = 0; column < outputOpaque.logicalTopology[0].size(); column++)
			{
				if (outputOpaque.logicalTopology[line][column] == 1)
				{
					outputLinks.linkIndex = k;
					outputLinks.linkSourceNode = line+1;
					outputLinks.linkDestinationNode = column+1;
					outputLinks.numberOfLightPaths = 0;
					k++;
					outputOpaque.logicalLinks.push_back(outputLinks);
				}
				// Light Paths stay void ??
				// They should be a state variable of path generator/tester
			}
		}
		outputSignals[0]->bufferPut((t_logical_topology) outputOpaque);

	}	
	else if (transportMode == transport_mode::transparent)  // Considering all nodes transparent
	{
		t_logical_topology outputTransparent;
		t_logical_links outputLinks;
		t_light_paths outputLights;
		t_integer k{ 0 };

//######### CREATES A TRANSPARENT LOGICAL TOPOLOGY ########################

		outputTransparent.logicalTopology.resize(physicalTopology[0].size(), vector<int>(physicalTopology[0].size(), 1));
		for (size_t k = 0; k < physicalTopology[0].size(); k++) { outputTransparent.logicalTopology[k][k] = 0; }

//######### CREATES LOGICAL LINKS ########################

		for (t_integer line = 0; line < outputTransparent.logicalTopology[0].size(); line++)
		{
			for (t_integer column = 0; column < outputTransparent.logicalTopology[0].size(); column++)
			{
				if (outputTransparent.logicalTopology[line][column] == 1)
				{
					outputLinks.linkIndex = k;
					outputLinks.linkSourceNode = line+1;
					outputLinks.linkDestinationNode = column+1;
					outputLinks.numberOfLightPaths = 0;
					k++;
					outputTransparent.logicalLinks.push_back(outputLinks);
				}
				// Light Paths stay void ??
				// They should be a state variable of path generator/tester
			}
		}


		outputSignals[0]->bufferPut((t_logical_topology) outputTransparent);
		
	}
	else
	{
		cout << "Error: logical_Top_Gen_20190216.h - Transport Mode not defined\n";
	}

	return true;
};