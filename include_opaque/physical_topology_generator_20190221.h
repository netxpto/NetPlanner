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

using namespace std;

class PhysicalTopologyGenerator : public Block {

	// State variables + Input Parameters
	t_integer index{ 0 };
	t_integer_long numberOfLinks{ 0 };
	t_integer opticalChannels{ 0 };
	
	// Input Parameters
	t_matrix physicalTopology;
	t_integer transmissionSystems{ 0 };
	t_integer opticalChannelCapacity{ 0 };
	t_integer numberOfNodes{ 0 };

	bool generatePhysicalLink(t_physical_link &pl);
	//bool generateOpticalChannel(t_optical_channel &och);
	t_integer calculateNumberOfNodes();
	t_integer calculateNumberOfLinks();

public:

	// Methods (Constructors)
	PhysicalTopologyGenerator() {};
	PhysicalTopologyGenerator(initializer_list<Signal *> inputSig, initializer_list<Signal *> outputSig) :Block(inputSig, outputSig) {};

	void initialize(void);
	bool runBlock(void);

	void setPhysicalTopology(t_matrix pTopology) { physicalTopology = pTopology; }
	t_matrix getPhysicalTopology(void) { return physicalTopology; };

	void setOpticalChannels(t_integer oChannels) { opticalChannels = oChannels; }
	t_integer getOpticalChannels(void) { return opticalChannels; };

	void setTransmissionSystems(t_integer tSystems) { transmissionSystems = tSystems; }
	t_integer getTransmissionSystems(void) { return transmissionSystems; };

	void setOpticalChannelCapacity(t_integer chCapacity) { opticalChannelCapacity = chCapacity; }
	t_integer getOpticalChannelCapacity(void) { return opticalChannelCapacity; };

	void setNumberOfNodes(t_integer nNodes) { numberOfNodes = nNodes; }
	void setNumberOfLinks(t_integer nLinks) { numberOfLinks = nLinks; }
	
	t_integer getNumberOfNodes() { return numberOfNodes; };
	t_integer getNumberOfLinks() { return numberOfLinks; };
	t_integer getIndex() { return index; };
};

# endif

