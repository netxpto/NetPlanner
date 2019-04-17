# ifndef LOGICAL_TOPOLOGY_GENERATOR_H_
# define LOGICAL_TOPOLOGY_GENERATOR_H_

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


using namespace std;

class LogicalTopologyGenerator : public Block {

	// Input Parameters
	transport_mode transportMode{ transport_mode::transparent }; // Transport mode default value
	t_matrix physicalTopologyAdjacencyMatrix{0};

	// State variables
	bool generate{ true };

public:

	// Methods (Constructors)
	LogicalTopologyGenerator() {};
	LogicalTopologyGenerator(initializer_list<Signal *> inputSig, initializer_list<Signal *> outputSig) :Block(inputSig, outputSig) {};


	void initialize(void);
	bool runBlock(void);

	void setTransportMode(transport_mode tMode) { transportMode = tMode; }
	transport_mode getTransportMode(void) { return transportMode; };

	void setPhysicalTopologyAjacencyMatrix(t_matrix pTopology) { physicalTopologyAdjacencyMatrix = pTopology; }
	t_matrix getPhysicalTopologyAdjacencyMatrix(void) { return physicalTopologyAdjacencyMatrix; };
};

# endif