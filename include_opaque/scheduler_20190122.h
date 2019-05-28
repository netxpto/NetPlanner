#ifndef SCHEDULER_H_
#define SCHEDULER_H_


#include "netxpto_20190130.h"


class Scheduler : public Block {

	// State variables + Input Parameters
	t_integer_long demandIndex{ 0 };
	t_integer_long numberOfDemands{ 0 };

	t_matrix odu0, odu1, odu2, odu3, odu4;

	//t_matrix odu0Copy{ 0 };
	//t_matrix odu1Copy{ 0 };
	//t_matrix odu2Copy{ 0 };
	//t_matrix odu3Copy{ 0 };
	//t_matrix odu4Copy{ 0 };

	// Input Parameters
	t_ordering_rule orderingRule{ t_ordering_rule::descendingOrder }; // By default descending ordering rule (ODU4 ... ODU0)
	t_integer numberOfNodes{ 0 };

	// Functions that need access to this class variables values in the main cpp file
	bool generateDemand(t_ordering_rule orderingRule, t_demand_request &dem);
	t_integer calculateNumberOfNodes();
	t_integer calculateNumberOfDemands();

public:

	// Methods (Constructors)
	Scheduler() {};
	Scheduler(initializer_list<Signal *> inputSig, initializer_list<Signal *> outputSig) :Block(inputSig, outputSig) {};

	void initialize(void);
	bool runBlock(void); 

	void setODU0(t_matrix od0) { odu0 = od0; }
	t_matrix const getODU0(void) { return odu0; };

	//void setODU0Copy() { odu0Copy = odu0; }
	//t_matrix const getODU0Copy(void) { return odu0Copy; };

	void setODU1(t_matrix od1) { odu1 = od1; }
	t_matrix const getODU1(void) { return odu1; };

	//void setODU1Copy() { odu1Copy = odu1; }
	//t_matrix const getODU1Copy(void) { return odu1Copy; };

	void setODU2(t_matrix od2) { odu2 = od2; }
	t_matrix const getODU2(void) { return odu2; };

	//void setODU2Copy() { odu2Copy = odu2; }
	//t_matrix const getODU2Copy(void) { return odu2Copy; };

	void setODU3(t_matrix od3) { odu3 = od3; }
	t_matrix const getODU3(void) { return odu3; };

	//void setODU3Copy() { odu3Copy = odu3; }
	//t_matrix const getODU3Copy(void) { return odu3Copy; };

	void setODU4(t_matrix od4) { odu4 = od4; }
	t_matrix const getODU4(void) { return odu4; };

	//void setODU4Copy() { odu4Copy = odu4; }
	//t_matrix const getODU4Copy(void) { return odu4Copy; };

	void setOrderingRule(t_ordering_rule oRule) { orderingRule = oRule; }
	t_ordering_rule const getOrderingRule(void) { return orderingRule; };

	void setNumberOfNodes(t_integer nodes) { numberOfNodes = nodes; }
	void setNumberOfDemands(t_integer number) { numberOfDemands = number; }

	t_integer getNumberOfNodes() { return numberOfNodes; };
	t_integer getNumberOfDemands() { return numberOfDemands; };
	t_integer getDemandIndex() { return demandIndex; };
	t_ordering_rule getDemandOrderingRule() { return orderingRule; };

};

# endif


