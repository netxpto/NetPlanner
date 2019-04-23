# ifndef LOGICAL_TOPOLOGY_MANAGER_H
# define LOGICAL_TOPOLOGY_MANAGER_H

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

class LogicalTopologyManager : public Block {
	
	// State variables
	t_logical_topology currentLogicalTopology;
	t_demand demand;

	// Input parameters
	routing_criterion_logical_topology routingCriterionLogicalTopology{ routing_criterion_logical_topology::hops };
	t_integer blockingCriterionLogicalTopology{ 1 }; // Number of paths to try before blocking a demand


public:

	// Methods (Constructors)
	LogicalTopologyManager() {};
	LogicalTopologyManager(initializer_list<Signal *> inputSig, initializer_list<Signal *> outputSig) :Block(inputSig, outputSig) {};
	
	void initialize(void);
	bool runBlock(void);

	void setRoutingCriterionLogicalTopology(routing_criterion_logical_topology crt) { routingCriterionLogicalTopology = crt; };
	routing_criterion_logical_topology getRoutingCriterionLogicalTopology(void) { return routingCriterionLogicalTopology; };

	void setBlockingCriterionLogicalTopology(t_integer crt) { blockingCriterionLogicalTopology = crt; };
	t_integer getBlockingCriterionLogicalTopology(void) { return blockingCriterionLogicalTopology; };

};

#endif 

