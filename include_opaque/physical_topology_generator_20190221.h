# ifndef PHYSICAL_TOPOLOGY_GENERATOR_H_
# define PHYSICAL_TOPOLOGY_GENERATOR_H_


# include "netxpto_20190130.h"


class PhysicalTopologyGenerator : public Block {

	// Input Parameters
	t_matrix physicalTopologyAdjacencyMatrix;
	t_integer numberOfOMSPerLink{ 0 };
	t_integer numberOfOpticalChannelsPerOMS{ 0 };
	t_integer opticalChannelCapacity{ 0 };
	
	// State Variables
	bool generate{ true };

public:

	// Methods (Constructors)
	PhysicalTopologyGenerator() {};
	PhysicalTopologyGenerator(initializer_list<Signal *> inputSig, initializer_list<Signal *> outputSig) :Block(inputSig, outputSig) {};

	void initialize(void);
	bool runBlock(void);

	void setPhysicalTopologyAdjacencyMatrix(t_matrix aMatrix) { physicalTopologyAdjacencyMatrix = aMatrix; }
	t_matrix getPhysicalTopologyAdjacencyMatrix(void) { return physicalTopologyAdjacencyMatrix; };

	void setNumberOfOMSPerLink(t_integer tSystems) { numberOfOMSPerLink = tSystems; }
	t_integer getNumberOfOMSPerLink(void) { return numberOfOMSPerLink; };
	
	void setNumberOfOpticalChannelsPerOMS(t_integer oChannels) { numberOfOpticalChannelsPerOMS = oChannels; }
	t_integer getNumberOfOpticalChannelsPerOMS(void) { return numberOfOpticalChannelsPerOMS; };

	void setOpticalChannelCapacity(t_integer oChannelCapacity) { opticalChannelCapacity = oChannelCapacity; }
	t_integer getOpticalChannelCapacity(void) { return opticalChannelCapacity; };

};

# endif

