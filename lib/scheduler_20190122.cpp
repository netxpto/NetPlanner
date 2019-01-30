# include "..\include\scheduler_20190122.h"


using namespace std;

t_demand generateDemand(t_integer orderingRule);

void Scheduler::initialize(void){

	numberOfOutputSignals = (int) outputSignals.size();

	setNumberOfNodes(calculateNumberOfNodes());
	setNumberOfDemands(calculateNumberOfDemands());

}

bool Scheduler::runBlock(void) {

	t_integer_long space = outputSignals[0]->space();

	if (space == 0) return false; // If there is not space available in the buffer to process another demand

	int process = std::min(space, numberOfDemands);

	for (int k = 1; k <= process; k++)
	{
		t_demand outputDemand = generateDemand(orderingRule);
		outputSignals[0]->bufferPut((t_demand) outputDemand);
		numberOfDemands--;
	}

	return true;
}

t_demand generateDemand(t_integer orderingRule)
{

	t_demand dem;
	/*
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

	*/
	return dem;
	
}


