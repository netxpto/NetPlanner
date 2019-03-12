# ifndef PHYSICAL_TOPOLOGY_GENERATOR_H_
# define PHYSICAL_TOPOLOGY_GENERATOR_H_

# include <vector>
# include <complex>
# include <fstream>
# include <iostream>
//# include <math.h>
//# include <stdio.h>
# include <string>
# include <strstream>
# include <vector>
# include <algorithm> 
# include <random>
# include <chrono>

# include "netxpto_20190130.h"


class PhysicalTopologyGenerator : public Block {

	// Input Parameters
	t_matrix physicalTopology;
	t_integer transmissionSystems{ 0 };
	t_integer opticalChannels{ 0 };
	t_integer opticalChannelCapacity{ 0 };
	
	// State Variables
	bool generate{ true };

public:

	// Methods (Constructors)
	PhysicalTopologyGenerator() {};
	PhysicalTopologyGenerator(initializer_list<Signal *> inputSig, initializer_list<Signal *> outputSig) :Block(inputSig, outputSig) {};

	void initialize(void);
	bool runBlock(void);

	void setPhysicalTopology(t_matrix pTopology) { physicalTopology = pTopology; }
	t_matrix getPhysicalTopology(void) { return physicalTopology; };

	void setTransmissionSystems(t_integer tSystems) { transmissionSystems = tSystems; }
	t_integer getTransmissionSystems(void) { return transmissionSystems; }; 
	
	void setOpticalChannels(t_integer oChannels) { opticalChannels = oChannels; }
	t_integer getOpticalChannels(void) { return opticalChannels; };

	void setOpticalChannelCapacity(t_integer oChannelCapacity) { opticalChannelCapacity = oChannelCapacity; }
	t_integer getOpticalChannelCapacity(void) { return opticalChannelCapacity; };

};

# endif

