# ifndef LOGICAL_TOPOLOGY_GENERATOR_H_
# define LOGICAL_TOPOLOGY_GENERATOR_H_


# include "netxpto_20190130.h"


class LogicalTopologyGenerator : public Block {

	// State variables + Input Parameters
	t_integer index{ 0 };
	t_integer_long numberOfLinks{ 0 };
	
	// Input Parameters
	std::string transportMode;
	t_matrix adjacencyMatrix;
	t_integer numberOfNodes{ 0 };
	
	// State variables
	bool generate{ true };
	bool generateLogicalLinkOpaque(t_logical_link &lLink);

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

	void setAdjacencyMatrix(t_matrix aMatrix) { adjacencyMatrix = aMatrix; }
	t_matrix getAdjacencyMatrix(void) { return adjacencyMatrix; };

	void setNumberOfNodes(t_integer nNodes) { numberOfNodes = nNodes; }
	void setNumberOfLinks(t_integer nLinks) { numberOfLinks = nLinks; }

	t_integer getNumberOfNodes() { return numberOfNodes; };
	t_integer getNumberOfLinks() { return numberOfLinks; };
	t_integer getIndex() { return index; };

};

# endif