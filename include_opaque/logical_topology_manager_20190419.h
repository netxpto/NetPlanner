#ifndef LOGICAL_TOPOLOGY_MANAGER_H_
#define LOGICAL_TOPOLOGY_MANAGER_H_

#include "netxpto_20190130.h"

class LogicalTopologyManager : public Block {

	//Input Parameters
	std::string routingCriterionLogicalTopology;
	t_integer blockingCriterionLogicalTopology;

	//State Variables
	t_logical_topology logicalTopology;
	t_demand_request demand;

	t_integer dijkstra(std::vector<vector<int>> graph, t_integer src, t_integer dst);

public:
	// Methods (Constructors)
	LogicalTopologyManager() {};
	LogicalTopologyManager(initializer_list<Signal *> inputSig, initializer_list<Signal *> outputSig) :Block(inputSig, outputSig) {};

	void initialize(void);
	bool runBlock(void);

	void setRoutingCriterionLogicalTopology(std::string rLogical) { routingCriterionLogicalTopology = rLogical; }
	std::string getRoutingCriterionLogicalTopology(void) { return routingCriterionLogicalTopology; };

	void setBlockingCriterionLogicalTopology(t_integer bLogical) { blockingCriterionLogicalTopology = bLogical; }
	t_integer getBlockingCriterionLogicalTopology(void) { return blockingCriterionLogicalTopology; };

};

# endif
