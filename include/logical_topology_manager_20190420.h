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
	
	//t_demand demand;
	//t_path_request_routed pathRequestRouted;

	// State variables
	t_logical_topology currentLogicalTopology;
	t_integer requestIndex{ 0 };
	t_matrix logicalTopology_odu0;	// Logical topology matrix for odu0 type demands
	t_matrix logicalTopology_odu1;	// Logical topology matrix for odu1 type demands
	t_matrix logicalTopology_odu2;	// Logical topology matrix for odu2 type demands
	t_matrix logicalTopology_odu3;	// Logical topology matrix for odu3 type demands
	t_matrix logicalTopology_odu4;	// Logical topology matrix for odu4 type demands

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

	t_logical_topology getLogicalTopology(void) { return currentLogicalTopology; };
};

#endif 

