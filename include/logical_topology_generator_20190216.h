# ifndef LOGICAL_TOPOLOGY_GENERATOR_H_
# define LOGICAL_TOPOLOGY_GENERATOR_H_

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


# include "..\include\netxpto_20190130.h"


using namespace std;

class LogicalTopologyGenerator : public Block {

	// Input Parameters
	std::string transportMode;
	t_matrix physicalTopology;

	// State variables
	t_logical_topolgy logicalTopology;
	bool change{ false }; // When Path tester bock is implemented there is a possibility of occuring changes in the logical topology 



public:

	// Methods (Constructors)
	LogicalTopologyGenerator() {};
	LogicalTopologyGenerator(initializer_list<Signal *> inputSig, initializer_list<Signal *> outputSig) :Block(inputSig, outputSig) {};


	void initialize(void);
	bool runBlock(void);

	void setTransportMode(std::string str) { transportMode = str; }
	std::string getTransportMode(void) { return transportMode; };

	void setPhysicalTopology(t_matrix phy) { physicalTopology = phy; }
	t_matrix getPhysicalTopology(void) { return physicalTopology; };
};

# endif