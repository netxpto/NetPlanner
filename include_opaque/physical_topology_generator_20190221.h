# ifndef PHYSICAL_TOPOLOGY_GENERATOR_H_
# define PHYSICAL_TOPOLOGY_GENERATOR_H_


# include "netxpto_20190130.h"


class PhysicalTopologyGenerator : public Block {

	// Input Parameters
	t_matrix physicalTopologyAdjacencyMatrix;
	t_matrix distanceMatrix;
	t_integer span;
	t_integer numberOfOMSPerLink{ 0 };
	t_integer numberOfOpticalChannelsPerOMS{ 0 };
	double initialWavelength{ 0 };
	double wavelengthSpacing{ 0 };
	t_integer opticalChannelCapacity{ 0 };
	
	// State Variables
	bool generate{ true };

public:

	// Methods (Constructors)
	PhysicalTopologyGenerator() {};
	PhysicalTopologyGenerator(std::initializer_list<Signal *> inputSig, std::initializer_list<Signal *> outputSig) :Block(inputSig, outputSig) {};

	void initialize(void);
	bool runBlock(void);

	void setPhysicalTopologyAdjacencyMatrix(t_matrix pTopology) { physicalTopologyAdjacencyMatrix = pTopology; }
	t_matrix getPhysicalTopologyAdjacencyMatrix(void) { return physicalTopologyAdjacencyMatrix; };

	void setDistanceMatrix(t_matrix dMatrix) { distanceMatrix = dMatrix; }
	t_matrix getDistanceMatrix(void) { return distanceMatrix; };

	void setSpan(t_integer sp) { span = sp; }
	t_integer getSpan(void) { return span; };

	void setNumberOfOMSPerLink(t_integer nOMS) { numberOfOMSPerLink = nOMS; }
	t_integer getNumberOfOMSPerLink(void) { return numberOfOMSPerLink; };
	
	void setNumberOfOpticalChannelsPerOMS(t_integer nChannels) { numberOfOpticalChannelsPerOMS = nChannels; }
	t_integer getNumberOfOpticalChannelsPerOMS(void) { return numberOfOpticalChannelsPerOMS; };

	void setInitialWavelength(double iWavelength) { initialWavelength = iWavelength; }
	double getInitialWavelength(void) { return initialWavelength; };

	void setWavelengthSpacing(double wSpacing) { wavelengthSpacing = wSpacing; }
	double getWavelengthSpacing(void) { return wavelengthSpacing; };

	void setOpticalChannelCapacity(t_integer cCapacity) { opticalChannelCapacity = cCapacity; }
	t_integer getOpticalChannelCapacity(void) { return opticalChannelCapacity; };

};

# endif