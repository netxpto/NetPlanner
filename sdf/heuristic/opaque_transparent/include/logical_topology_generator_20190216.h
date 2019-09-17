# ifndef LOGICAL_TOPOLOGY_GENERATOR_H_
# define LOGICAL_TOPOLOGY_GENERATOR_H_


# include "..\include\netxpto_20190130.h"


using namespace std;

class LogicalTopologyGenerator : public Block {

	// Input Parameters
	transport_mode transportMode; // Transport mode default value
	t_matrix physicalTopologyAdjacencyMatrix;
	t_matrix distanceBetweenNodes;

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

	void setDistanceMatrix(t_matrix dMatrix) { distanceBetweenNodes = dMatrix; }
	t_matrix getDistanceMatrix(void) { return distanceBetweenNodes; };

};

# endif