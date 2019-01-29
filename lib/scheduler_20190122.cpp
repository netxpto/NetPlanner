# include "..\include\scheduler_20190122.h"

using namespace std;

void Scheduler::initialize(void){

	numberOfOutputSignals = (int) outputSignals.size();

	for (auto i = 0; i < numberOfOutputSignals; ++i) {
		outputSignals[i]->
	};

	setNumberOfNodes(calculateNumberOfNodes());
	setNumberOfDemands(calculateNumberOfDemands());

}

bool Scheduler::runBlock(void) {

	long int space = outputSignals[0]->space();

	if (space == 0) return false;

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
	if (orderingRule == 0) // ODU4 to ODU0
	{
		//############################ ODU4 ####################################
		for (int linha = 0; linha < numberOfNodes; linha++)
		{
			for (int coluna = 0; coluna < numberOfNodes; coluna++)
			{
				while (odu4.matrix[linha][coluna]>0) // If there are demands to be processed between this pair of nodes
				{
					// Creates new a Demand signal
					// Class demand has to be included here ?

					demandIndex = demandIndex;
					sourceNode = { linha + 1 };
					destinationNode = { coluna + 1 };
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


	return true;
}

}



void Scheduler :: calculaNumberOfNOdes(){
};

void Scheduler::calculaNumberOfDemands() {
};
