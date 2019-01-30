# ifndef SCHEDULER_H_
# define SCHEDULER_H_


/*
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
*/

# include "..\include\netxpto_20190130.h"


using namespace std;


 
class Scheduler : public Block {

	// State variables + Input Parameters
	t_integer_long demandIndex{ 0 };
	t_integer_long numberOfDemands{ 0 };
	t_matrix odu0{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} };
	t_matrix odu1{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} };
	t_matrix odu2{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} };
	t_matrix odu3{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} };
	t_matrix odu4{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} };

	// Input Parameters
	t_integer orderingRule{ 0 };
	t_integer numberOfNodes{ 0 };

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

	void setNumberOfNodes(t_integer nodes) {numberOfNodes = nodes;}
	void setNumberOfDemands(t_integer number) { numberOfDemands = number; }

	t_integer calculateNumberOfNodes() 
	{
		t_integer nodes = sizeof(odu0);
		return nodes;
	};

	t_integer calculateNumberOfDemands()
	{
		/*		t_integer demands { 0 };
				t_integer linha{ 0 };
				t_integer coluna{ 0 };

				//##################### ODU0 ###############################################
				for (linha = 0; linha < numberOfNodes; linha++)
				{
					for (coluna = 0; coluna < numberOfNodes; coluna++)
					{
						while (odu0.matrix[linha][coluna] > 0) // If there are demands to be processed between this pair of nodes
						{
							demands++;
						}
					}
				}
				//##################### ODU1 ###############################################
				for (linha = 0; linha < numberOfNodes; linha++)
				{
					for (coluna = 0; coluna < numberOfNodes; coluna++)
					{
						while (odu1.matrix[linha][coluna] > 0) // If there are demands to be processed between this pair of nodes
						{
							demands++;
						}
					}
				}
				//##################### ODU2 ###############################################
				for (linha = 0; linha < numberOfNodes; linha++)
				{
					for (coluna = 0; coluna < numberOfNodes; coluna++)
					{
						while (odu2.matrix[linha][coluna] > 0) // If there are demands to be processed between this pair of nodes
						{
							demands++;
						}
					}
				}
				//##################### ODU3 ###############################################
				for (linha = 0; linha < numberOfNodes; linha++)
				{
					for (coluna = 0; coluna < numberOfNodes; coluna++)
					{
						while (odu3.matrix[linha][coluna] > 0) // If there are demands to be processed between this pair of nodes
						{
							demands++;
						}
					}
				}
				//##################### ODU4 ###############################################
				for (linha = 0; linha < numberOfNodes; linha++)
				{
					for (coluna = 0; coluna < numberOfNodes; coluna++)
					{
						while (odu4.matrix[linha][coluna] > 0) // If there are demands to be processed between this pair of nodes
						{
							demands++;
						}
					}
				}
				return demands;
			};
		*/
		return 0;
	};
	
};

# endif


