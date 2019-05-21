# ifndef PHYSICAL_TOPOLOGY_MANAGER_H_
# define PHYSICAL_TOPOLOGY_MANAGER_H_

# include "netxpto_20190130.h"

class PhysicalTopologyManager : public Block {

	//Input Parameters
	t_routing_criterion_physical_topology routingCriterionPhysicalTopology;
	t_integer blockingCriterionPhysicalTopology;

	//State Variables
	t_physical_topology physicalTopology;

public:
	// Methods (Constructors)
	PhysicalTopologyManager() {};
	PhysicalTopologyManager(initializer_list<Signal *> inputSig, initializer_list<Signal *> outputSig) :Block(inputSig, outputSig) {};

	void initialize(void);
	bool runBlock(void);

	void setRoutingCriterionPhysicalTopology(t_routing_criterion_physical_topology rPhysical) { routingCriterionPhysicalTopology = rPhysical; }
	t_routing_criterion_physical_topology getRoutingCriterionPhysicalTopology(void) { return routingCriterionPhysicalTopology; };

	void setBlockingCriterionPhysicalTopology(t_integer bPhysical) { blockingCriterionPhysicalTopology = bPhysical; }
	t_integer getBlockingCriterionPhysicalTopology(void) { return blockingCriterionPhysicalTopology; };

};

# endif
