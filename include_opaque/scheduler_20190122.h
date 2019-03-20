#ifndef SCHEDULER_H_
#define SCHEDULER_H_


#include "netxpto_20190130.h"


class Scheduler : public Block {

	// State variables + Input Parameters
	t_integer_long demandIndex{ 0 };
	t_integer_long numberOfDemands{ 0 };

	t_matrix odu0, odu1, odu2, odu3, odu4;

	// Input Parameters
	t_integer orderingRule{ 0 }; // By default descending ordering rule (ODU4 ... ODU0)
	t_integer numberOfNodes{ 0 };

	// Functions that need access to this class variables values in the main cpp file
	bool generateDemand(t_integer orderingRule, t_demand &dem);
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

	void setODU1(t_matrix od1) { odu1 = od1; }
	t_matrix const getODU1(void) { return odu1; };

	void setODU2(t_matrix od2) { odu2 = od2; }
	t_matrix const getODU2(void) { return odu2; };

	void setODU3(t_matrix od3) { odu3 = od3; }
	t_matrix const getODU3(void) { return odu3; };

	void setODU4(t_matrix od4) { odu4 = od4; }
	t_matrix const getODU4(void) { return odu4; };

	void setDemandsOrderingRule(t_integer rule) { orderingRule = rule; }
	t_integer const setDemandsOrderingRule(void) { return orderingRule; };

	void setNumberOfNodes(t_integer nodes) { numberOfNodes = nodes; }
	void setNumberOfDemands(t_integer number) { numberOfDemands = number; }

	t_integer getNumberOfNodes() { return numberOfNodes; };
	t_integer getNumberOfDemands() { return numberOfDemands; };
	t_integer getDemandIndex() { return demandIndex; };
	t_integer getDemandOrderingRule() { return orderingRule; };

};

# endif


