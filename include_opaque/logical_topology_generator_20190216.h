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

# include "netxpto_20190130.h"

//using namespace std;

class LogicalTopologyGenerator : public Block {

	// State variables + Input Parameters
	t_integer index{ 0 };
	t_integer_long numberOfLinks{ 0 };
	t_integer numberOfLightPaths{ 0 };
	
	// Input Parameters
	std::string transportMode;
	t_matrix physicalTopology;
	t_integer numberOfNodes{ 0 };
	
	bool generateLogicalLinkOpaque(t_logical_link & lLO);
	t_integer calculateNumberOfNodes();
	t_integer calculateNumberOfLinks();

public:

	// Methods (Constructors)
	LogicalTopologyGenerator() {};
	LogicalTopologyGenerator(initializer_list<Signal *> inputSig, initializer_list<Signal *> outputSig) :Block(inputSig, outputSig) {};

	void initialize(void);
	bool runBlock(void);

	void setTransportMode(std::string tMode) { transportMode = tMode; }
	std::string getTransportMode(void) { return transportMode; };

	void setPhysicalTopology(t_matrix pTopology) { physicalTopology = pTopology; }
	t_matrix getPhysicalTopology(void) { return physicalTopology; };

	void setNumberOfNodes(t_integer nNodes) { numberOfNodes = nNodes; }
	void setNumberOfLinks(t_integer nLinks) { numberOfLinks = nLinks; }

	t_integer getNumberOfNodes() { return numberOfNodes; };
	t_integer getNumberOfLinks() { return numberOfLinks; };
	t_integer getIndex() { return index; };
};

# endif