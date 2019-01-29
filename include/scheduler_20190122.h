# ifndef SCHEDULER_H_
# define SCHEDULER_H_


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
const int NUMBER_OF_NODES = 6;


# include "..\include\netxpto_20180815.h"

using namespace std;

 
class Scheduler : public Block {

	// State variables
	t_integer demandIndex = { 0 };
	t_integer sourceNode = { 0 };
	t_integer destinationNode = { 0 };
	t_integer oduType = { 0 };
	t_integer restorationMethod = { 0 };

	 // Input parameters
	t_matrix odu0{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} };
	t_matrix odu1{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} };
	t_matrix odu2{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} };
	t_matrix odu3{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} };
	t_matrix odu4{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} };
	t_integer orderingRule{ 0 };


 public:

	// Methods (Constructors)
	Scheduler() {};
	Scheduler(initializer_list<Signal *> inputSig, initializer_list<Signal *> outputSig) :Block(inputSig, outputSig){};

	void initialize(void);
	bool runBlock(void);

	void setODU0(t_matrix od0) {odu0 = od0;}
	t_matrix const getODU0(void) { return odu0; };

	void setODU1(t_matrix od1) { odu1 = od1; }
	t_matrix const getODU1(void) { return odu1; };
	
	void setODU2(t_matrix od2) { odu2 = od2; }
	t_matrix const getODU2(void) { return odu2; };

	void setODU3(t_matrix od3) { odu3 = od3; }
	t_matrix const getODU3(void) { return odu3; };

	void setODU4(t_matrix od4) { odu4 = od4; }
	t_matrix const getODU4(void) { return odu4; };

	void setDemandsOrderingRule(t_integer rule) { orderingRule= rule; }
	t_integer const setDemandsOrderingRule(void) { return orderingRule; };

private:
	// LÓGICA DO BLOCO SCHEDULER
	if (orderingRule == 0) // ODU4 to ODU0
	{
		//############################ ODU4 ####################################

		for (int linha = 0; linha < NUMBER_OF_NODES; linha++)
		{
			for (int coluna = 0; j < NUMBER_OF_NODES; cooluna++)
			{
				while (odu4.matrix[linha][coluna]>0) // If there are demands to be processed between this pair of nodes
				{
					// Creates new a Demand signal
					// Class demand has to be included here ?

					demandIndex = demandIndex;
					sourceNode = { linha+1 };
					destinationNode = { coluna+1 };
					oduType = { 4 };
					restorationMethod = { 0 }; // Always 0, meaning no protection

					odu4.matrix[linha][coluna]--; // A demand was processed
					demandIndex++;
				}
			}
		}
		// The same for ODUs 3,2,1 and 0
	}
	else if (orderingRule == 1) // ODU0 to ODU4
	{

	}
	else // Other
	{

	}
};

# endif


