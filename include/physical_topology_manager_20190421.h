# ifndef PHYSICAL_TOPOLOGY_MANAGER_H
# define PHYSICAL_TOPOLOGY_MANAGER_H

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

class PhysicalTopologyManager : public Block {

	// State variables
	t_physical_topology currentPhysicalTopology;

	// Input parameters
	routing_criterion_physical_topology routingCriterionPhysicalTopology{ routing_criterion_physical_topology::hops };
	t_integer blockingCriterionPhysicalTopology{ 1 }; // Number of paths to try before blocking a demand


public:

	// Methods (Constructors)
	PhysicalTopologyManager() {};
	PhysicalTopologyManager(initializer_list<Signal *> inputSig, initializer_list<Signal *> outputSig) :Block(inputSig, outputSig) {};

	void initialize(void);
	bool runBlock(void);

	void setRoutingCriterionPhysicalTopology(routing_criterion_physical_topology crt) { routingCriterionPhysicalTopology = crt; };
	routing_criterion_physical_topology getRoutingCriterionPhysicalTopology(void) { return routingCriterionPhysicalTopology; };

	void setBlockingCriterionPhysicalTopology(t_integer crt) { blockingCriterionPhysicalTopology = crt; };
	t_integer getBlockingCriterionPhysicalTopology(void) { return blockingCriterionPhysicalTopology; };

	t_physical_topology getPhysicalTopology() { return currentPhysicalTopology; };
};

#endif 

