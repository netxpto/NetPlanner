# ifndef PHYSICAL_TOPOLOGY_GENERATOR_H_
# define PHYSICAL_TOPOLOGY_GENERATOR_H_

# include <vector>
# include <complex>
# include <fstream>
# include <iostream>
# include <math.h>
# include <stdio.h>
# include <string>
# include <strstream>
# include <vector>
# include <algorithm> 
# include <random>
# include <chrono>


# include "..\include\netxpto_20190130.h"

class PhysicalTopologyGenerator : public Block {

	// Input Parameters
	t_matrix physicalTopology{ {0} };
	t_integer opticalChannelsPerTransportSystem{ 0 };
	t_integer transportSystems{ 0 };		// Number of transport systems between each pair of nodes
	t_integer opticalChannelCapacity{ 0 };  // In terms of ODU0s

	// State variables
	bool generate{ true };

public:

	// Methods (Constructors)
	PhysicalTopologyGenerator() {};
	PhysicalTopologyGenerator(initializer_list<Signal *> inputSig, initializer_list<Signal *> outputSig) :Block(inputSig, outputSig) {};


	void initialize(void);
	bool runBlock(void);


	void setOpticalChannels(t_integer channels) { opticalChannelsPerTransportSystem = channels; }; // Number of optical channels per transport system
	t_integer getOpticalChannels(void) { return opticalChannelsPerTransportSystem; };

	void setPhysicalTopology(t_matrix pTopology) { physicalTopology = pTopology; } // Physical topology
	t_matrix getPhysicalTopology(void) { return physicalTopology; };

	void setTransportSystems(t_integer tSystems) { transportSystems = tSystems; };
	t_integer getTransportSystems(void) { return transportSystems; };

	void setOpticalChannelCapacity(t_integer optChCapacity) { opticalChannelCapacity = optChCapacity; };
	t_integer getOpticalChannelCapacity(void) { return opticalChannelCapacity; };
};

# endif