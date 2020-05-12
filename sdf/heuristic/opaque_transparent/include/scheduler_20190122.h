# ifndef SCHEDULER_H_
# define SCHEDULER_H_

# include "..\include\netxpto_20190130.h"


//using namespace std;    // (commented by Romil 08/05/2020)


 
class Scheduler : public Block {

	// State variables + Input Parameters
	t_integer_long demandIndex{ 0 };
	t_integer_long numberOfDemands{ 0 };

	t_matrix odu0{ 0 };		// ODUO demands matrix default value
	t_matrix odu1{ 0 };		// ODU1 demands matrix default value
	t_matrix odu2{ 0 };		// ODU2 demands matrix default value
	t_matrix odu3{ 0 };		// ODU3 demands matrix default value
	t_matrix odu4{ 0 };		// ODU4 demands matrix default value

	// Copy of the demands matrices for report purposes
	t_matrix odu0Copy{ 0 };
	t_matrix odu1Copy{ 0 };
	t_matrix odu2Copy{ 0 };
	t_matrix odu3Copy{ 0 };
	t_matrix odu4Copy{ 0 };

	// Input Parameters
	ordering_rule orderingRule{ ordering_rule::ascendingOrder };	// By default descending ordering rule (ODU4 ... ODU0)
	t_integer numberOfNodes{ 0 }; // Deafult value

	// Functions that need access to this class variables values in the main cpp file
	bool generateDemand(ordering_rule orderingRule, t_demand &dem);
	t_integer calculateNumberOfDemands();

public:

	// Methods (Constructors)
	Scheduler() {};
	Scheduler(std::initializer_list<Signal *> inputSig, std::initializer_list<Signal *> outputSig) :Block(inputSig, outputSig) {};

	void initialize(void);
	bool runBlock(void); 


	void setNumberOfNodes(t_integer nodes) { numberOfNodes = nodes; }
	t_integer getNumberOfNodes(void) { return numberOfNodes; };

	void setODU0(t_matrix od0) { odu0 = od0; }
	t_matrix const getODU0(void) { return odu0; };

	void setODU0Copy() { odu0Copy=odu0; }
	t_matrix const getODU0Copy(void) { return odu0Copy; };

	void setODU1(t_matrix od1) { odu1 = od1; }
	t_matrix const getODU1(void) { return odu1; };

	void setODU1Copy() { odu1Copy = odu1; }
	t_matrix const getODU1Copy(void) { return odu1Copy; };

	void setODU2(t_matrix od2) { odu2 = od2; }
	t_matrix const getODU2(void) { return odu2; };

	void setODU2Copy() { odu2Copy = odu2; }
	t_matrix const getODU2Copy(void) { return odu2Copy; };

	void setODU3(t_matrix od3) { odu3 = od3; }
	t_matrix const getODU3(void) { return odu3; };

	void setODU3Copy() { odu3Copy = odu3; }
	t_matrix const getODU3Copy(void) { return odu3Copy; };

	void setODU4(t_matrix od4) { odu4 = od4; }
	t_matrix const getODU4(void) { return odu4; };

	void setODU4Copy() { odu4Copy = odu4; }
	t_matrix const getODU4Copy(void) { return odu4Copy; };

	void setOrderingRule(ordering_rule rule) { orderingRule = rule; }
	ordering_rule getOrderingRule(void) { return orderingRule; };

	void setNumberOfDemands(t_integer number) { numberOfDemands = number; }

	
	t_integer getNumberOfDemands() { return numberOfDemands; };
	t_integer getDemandIndex() { return demandIndex; };	

};

# endif


