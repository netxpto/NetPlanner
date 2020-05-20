# ifndef PHYSICAL_TOPOLOGY_MANAGER_H
# define PHYSICAL_TOPOLOGY_MANAGER_H


# include "..\include\netxpto_20190130.h"

//using namespace std;    // (commented by Romil 08/05/2020)

class PhysicalTopologyManager : public Block {

	// State variables
	t_physical_topology currentPhysicalTopology;

	// Input parameters
	t_routing_criterion_physical_topology routingCriterionPhysicalTopology{ t_routing_criterion_physical_topology::hops };
	t_integer blockingCriterionPhysicalTopology{ 1 }; // Number of paths to try before blocking a demand


public:

	// Methods (Constructors)
	PhysicalTopologyManager() {};
	PhysicalTopologyManager(std::initializer_list<Signal *> inputSig, std::initializer_list<Signal *> outputSig) :Block(inputSig, outputSig) {};

	void initialize(void);
	bool runBlock(void);

	void setRoutingCriterionPhysicalTopology(t_routing_criterion_physical_topology crt) { routingCriterionPhysicalTopology = crt; };
	t_routing_criterion_physical_topology getRoutingCriterionPhysicalTopology(void) { return routingCriterionPhysicalTopology; };

	void setBlockingCriterionPhysicalTopology(t_integer crt) { blockingCriterionPhysicalTopology = crt; };
	t_integer getBlockingCriterionPhysicalTopology(void) { return blockingCriterionPhysicalTopology; };

	t_physical_topology getPhysicalTopology() { return currentPhysicalTopology; };
};

#endif 

