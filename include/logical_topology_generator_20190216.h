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
	transport_mode transportMode{ transport_mode::opaque };
	t_matrix physicalTopology{ {0} };

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

	void setPhysicalTopology(t_matrix pTopology) { physicalTopology = pTopology; }
	t_matrix getPhysicalTopology(void) { return physicalTopology; };
};

# endif