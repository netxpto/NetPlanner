# ifndef LOGICAL_TOPOLOGY_GENERATOR_H_
# define LOGICAL_TOPOLOGY_GENERATOR_H_

# include "netxpto_20190130.h"

class LogicalTopologyGenerator : public Block {

	// Input Parameters
	std::string transportMode;
	t_matrix physicalTopologyAdjacencyMatrix;
	
	// State variables
	bool generate{ true };
	
public:

	// Methods (Constructors)
	LogicalTopologyGenerator() {};
	LogicalTopologyGenerator(initializer_list<Signal *> inputSig, initializer_list<Signal *> outputSig) :Block(inputSig, outputSig) {};

	void initialize(void);
	bool runBlock(void);

	void setTransportMode(std::string tMode) { transportMode = tMode; }
	std::string getTransportMode(void) { return transportMode; };

	void setPhysicalTopologyAdjacencyMatrix(t_matrix pTopology) { physicalTopologyAdjacencyMatrix = pTopology; }
	t_matrix getPhysicalTopologyAdjacencyMatrix(void) { return physicalTopologyAdjacencyMatrix; };

};

# endif