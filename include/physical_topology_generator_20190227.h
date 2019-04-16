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
	t_matrix physicalTopologyAdjacencyMatrix{0};
	t_integer numberOfOMSPerLink{ 0 };
	t_integer numberOfOpticalChannelsPerOMS{0};
	double initialWavelenght{ 0 };
	double wavelenghtSpacing{ 0 };
	t_integer opticalChannelCapacity{ 0 };

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

	void setNumberOfOMSPerLink(t_integer tSystems) { numberOfOMSPerLink = tSystems; };
	t_integer getnumberOfOMSPerLink(void) { return numberOfOMSPerLink; };

	void setOpticalChannelCapacity(t_integer optChCapacity) { opticalChannelCapacity = optChCapacity; };
	t_integer getOpticalChannelCapacity(void) { return opticalChannelCapacity; };
};

# endif