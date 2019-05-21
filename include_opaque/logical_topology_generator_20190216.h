# ifndef LOGICAL_TOPOLOGY_GENERATOR_H_
# define LOGICAL_TOPOLOGY_GENERATOR_H_

# include "netxpto_20190130.h"

class LogicalTopologyGenerator : public Block {

	// Input Parameters
	t_transport_mode transportMode;
	t_matrix physicalTopologyAdjacencyMatrix;
	t_matrix distanceMatrix;

	// State variables
	bool generate{ true };
	
public:

	// Methods (Constructors)
	LogicalTopologyGenerator() {};
	LogicalTopologyGenerator(initializer_list<Signal *> inputSig, initializer_list<Signal *> outputSig) :Block(inputSig, outputSig) {};

	void initialize(void);
	bool runBlock(void);

	void setTransportMode(t_transport_mode tMode) { transportMode = tMode; }
	t_transport_mode getTransportMode(void) { return transportMode; };

	void setPhysicalTopologyAdjacencyMatrix(t_matrix pTopology) { physicalTopologyAdjacencyMatrix = pTopology; }
	t_matrix getPhysicalTopologyAdjacencyMatrix(void) { return physicalTopologyAdjacencyMatrix; };

	void setDistanceMatrix(t_matrix dMatrix) { distanceMatrix = dMatrix; }
	t_matrix getDistanceMatrix(void) { return distanceMatrix; };

};

# endif