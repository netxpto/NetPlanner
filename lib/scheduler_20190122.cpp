# include "..\include\scheduler_20190122.h"
# include "..\include\header_netxpto.h"

using namespace std;


void Scheduler::initialize(void){

	numberOfOutputSignals = (int) outputSignals.size();

	for (auto i = 0; i < numberOfOutputSignals; ++i) {
		outputSignals[i]-> // ?
	};

	setNumberOfNodes(calculateNumberOfNodes());
	setNumberOfDemands(calculateNumberOfDemands());

}

bool Scheduler::runBlock(void) {

	long int space = outputSignals[0]->space();

	if (space == 0) return false; // If there is not space available in the buffer to process another demand

	int process = min(space, numberOfDemands);

	for (int k = 1; k <= process; k++)
	{
		Demand outputDemand = generateDemand(orderingRule);
		outputSignals[0]->putSignal(outputDemand);
		numberOfDemands--;
	}

	return true;
}

Demand generateDemand(t_integer orderingRule)
{
	t_integer linha { 0 };
	t_integer coluna{ 0 };

	if (orderingRule == 0) // ODU4 to ODU0
	{
		//############################ ODU4 ####################################
		for (linha = 0; linha < numberOfNodes; linha++)
		{
			for (coluna = 0; coluna < numberOfNodes; coluna++)
			{
				if (odu4.matrix[linha][coluna] != 0) // If there are demands to be processed between this pair of nodes
				{
					// Creates new a Demand signal
					Demand d;
					d.demandIndex = demandIndex;
					d.sourceNode = { linha + 1 };
					d.destinationNode = { coluna + 1 };
					d.oduType = { 4 };
					d.restorationMethod = { 0 }; // Always 0, meaning no protection

					odu4.matrix[linha][coluna]--; // A demand was processed
					demandIndex++;
				}
			}
		}
		// The same for ODUs 3,2,1 and 0
	}
	else if (orderingRule == 1) // ODU0 to ODU4
	{

		//############################ ODU4 ####################################
		for (linha = 0; linha < numberOfNodes; linha++)
		{
			for (coluna = 0; coluna < numberOfNodes; coluna++)
			{
				if (odu0.matrix[linha][coluna] != 0) // If there are demands to be processed between this pair of nodes
				{
					// Creates new a Demand signal
					Demand d;
					d.demandIndex = demandIndex;
					d.sourceNode = { linha + 1 };
					d.destinationNode = { coluna + 1 };
					d.oduType = { 0 };
					d.restorationMethod = { 0 }; // Always 0, meaning no protection

					odu4.matrix[linha][coluna]--; // A demand was processed
					demandIndex++;
				}
			}
		}
		// The same for ODUs 1,2,3 and 4
	}
	else // Other
	{
	};


	return true;
}


