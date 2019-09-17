# ifndef PHYSICAL_TOPOLOGY_GENERATOR_H_
# define PHYSICAL_TOPOLOGY_GENERATOR_H_


# include "..\include\netxpto_20190130.h"

class PhysicalTopologyGenerator : public Block {

	// Input Parameters
	t_matrix physicalTopologyAdjacencyMatrix;
	transport_mode transportMode;
	t_matrix distancesBetweenNodes;
	t_integer span;
	t_integer numberOfOMSPerLink;
	t_integer numberOfOpticalChannelsPerOMS;
	double initialWavelenght;
	double wavelenghtSpacing;
	t_integer opticalChannelCapacity;

	// State variables
	bool generate{ true }; 
	

public:

	// Methods (Constructors)
	PhysicalTopologyGenerator() {};
	PhysicalTopologyGenerator(initializer_list<Signal *> inputSig, initializer_list<Signal *> outputSig) :Block(inputSig, outputSig) {};


	void initialize(void);
	bool runBlock(void);


	void setNumberOfOpticalChannelsPerOMS(t_integer channels) { numberOfOpticalChannelsPerOMS = channels; };	// Number of optical channels per OMS
	t_integer getNumberOfOpticalChannelsPerOMS(void) { return numberOfOpticalChannelsPerOMS; };

	void setInitialWavelenght(double wavelenght) { initialWavelenght = wavelenght; };						// Initial wavelenght value (nm)
	double getInitialWavelenght(void) { return initialWavelenght; };

	void setWavelenghtSpacing(double spacing) { wavelenghtSpacing = spacing; };									// Spacing between wavlenghts used
	double getWavelenghtSpacing(void) { return wavelenghtSpacing; };

	void setPhysicalTopologyAdjacencyMatrix(t_matrix pTopology) { physicalTopologyAdjacencyMatrix = pTopology; } // Physical topology
	t_matrix getPhysicalTopologyAdjacencyMatrix(void) { return physicalTopologyAdjacencyMatrix; };

	void setDistancesBetweenNodes(t_matrix distances) { distancesBetweenNodes = distances; } // Distances between node in kilometers (km)
	t_matrix getDistancesBetweenNodes(void) { return distancesBetweenNodes; };
	
	void setSpan(int s) { span = s; } // maximum distance between amplifiers
	t_integer getSpan(void) { return span; };

	void setNumberOfOMSPerLink(t_integer tSystems) { numberOfOMSPerLink = tSystems; };
	t_integer getnumberOfOMSPerLink(void) { return numberOfOMSPerLink; };

	void setOpticalChannelCapacity(t_integer optChCapacity) { opticalChannelCapacity = optChCapacity; };
	t_integer getOpticalChannelCapacity(void) { return opticalChannelCapacity; };

	void setTransportMode(transport_mode tMode) { transportMode = tMode; }
	transport_mode getTransportMode(void) { return transportMode; };
};

# endif