# include "..\include\scheduler_20190122.h"


using namespace std;

//	Scheduler scheduler;
t_demand generateDemand(t_integer orderingRule);

//	Global variables needed to generate a demand signal
t_matrix odu0;
t_matrix odu1;
t_matrix odu2;
t_matrix odu3;
t_matrix odu4;
t_integer numberOfNodes;
t_integer numberOfDemands;
t_integer demandIndex;
t_integer orderingRule;

void Scheduler::initialize(void){

	numberOfOutputSignals = (int) outputSignals.size();

	setNumberOfNodes(calculateNumberOfNodes());
	setNumberOfDemands(calculateNumberOfDemands());

	odu0 = getODU0();								// Returns matrix of ODU0 demands
	odu1 = getODU1();								// Returns matrix of ODU1 demands
	odu2 = getODU2();								// Returns matrix of ODU2 demands
	odu3 = getODU3();								// Returns matrix of ODU3 demands	
	odu4 = getODU4();								// Returns matrix of ODU4 demands
	numberOfNodes = getNumberOfNodes();				// Returns the number of nodes
	numberOfDemands = getNumberOfDemands();			// Returns the total number of existent demands
	demandIndex = getDemandIndex();					// Returns demandIndex value
	orderingRule = getDemandOrderingRule();			// Returns orderingRule value

}

bool Scheduler::runBlock(void) {

	t_integer_long space = outputSignals[0]->space(); // signal buffer space 

	if (space == 0) return false; // If there is no more space available in the buffer to process another demand

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

	t_demand dem; // Creates new a demand signal
	t_integer line{ 0 };
	t_integer column{ 0 };

	if (orderingRule == 0) // ODU4 to ODU0
	{
		//############################ ODU4 ####################################
		for (line = 0; line < numberOfNodes; line++)
		{
			for (column = 0; column < numberOfNodes; column++)
			{
				if (odu4[line][column] != 0) // If there are demands to be processed between this pair of nodes
				{
					dem.demandIndex = demandIndex;
					dem.sourceNode = { line + 1 };
					dem.destinationNode = { column + 1 };
					dem.oduType = { 4 };
					dem.restorationMethod = { 0 }; // Always 0, meaning no protection

					odu4[line][column]--; // A demand was processed
					demandIndex++;
					return dem;
				}
			}
		}
		//############################ ODU3 ####################################
		for (line = 0; line < numberOfNodes; line++)
		{
			for (column = 0; column < numberOfNodes; column++)
			{
				if (odu3[line][column] != 0) // If there are demands to be processed between this pair of nodes
				{
					dem.demandIndex = demandIndex;
					dem.sourceNode = { line + 1 };
					dem.destinationNode = { column + 1 };
					dem.oduType = { 3 };
					dem.restorationMethod = { 0 }; // Always 0, meaning no protection

					odu3[line][column]--; // A demand was processed
					demandIndex++;
					return dem;
				}
			}
		}
		//############################ ODU2 ####################################
		for (line = 0; line < numberOfNodes; line++)
		{
			for (column = 0; column < numberOfNodes; column++)
			{
				if (odu2[line][column] != 0) // If there are demands to be processed between this pair of nodes
				{
					dem.demandIndex = demandIndex;
					dem.sourceNode = { line + 1 };
					dem.destinationNode = { column + 1 };
					dem.oduType = { 2 };
					dem.restorationMethod = { 0 }; // Always 0, meaning no protection

					odu2[line][column]--; // A demand was processed
					demandIndex++;
					return dem;
				}
			}
		}
		//############################ ODU1 ####################################
		for (line = 0; line < numberOfNodes; line++)
		{
			for (column = 0; column < numberOfNodes; column++)
			{
				if (odu1[line][column] != 0) // If there are demands to be processed between this pair of nodes
				{
					dem.demandIndex = demandIndex;
					dem.sourceNode = { line + 1 };
					dem.destinationNode = { column + 1 };
					dem.oduType = { 1 };
					dem.restorationMethod = { 0 }; // Always 0, meaning no protection

					odu1[line][column]--; // A demand was processed
					demandIndex++;
					return dem;
				}
			}
		}
		//############################ ODU0 ####################################
		for (line = 0; line < numberOfNodes; line++)
		{
			for (column = 0; column < numberOfNodes; column++)
			{
				if (odu0[line][column] != 0) // If there are demands to be processed between this pair of nodes
				{
					dem.demandIndex = demandIndex;
					dem.sourceNode = { line + 1 };
					dem.destinationNode = { column + 1 };
					dem.oduType = { 0 };
					dem.restorationMethod = { 0 }; // Always 0, meaning no protection

					odu0[line][column]--; // A demand was processed
					demandIndex++;
					return dem;
				}
			}
		}
		
	}
	else if (orderingRule == 1) // ODU0 to ODU4
	{

		//############################ ODU0 ####################################
		for (line = 0; line < numberOfNodes; line++)
		{
			for (column = 0; column < numberOfNodes; column++)
			{
				if (odu0[line][column] != 0) // If there are demands to be processed between this pair of nodes
				{
					dem.demandIndex = demandIndex;
					dem.sourceNode = { line + 1 };
					dem.destinationNode = { column + 1 };
					dem.oduType = { 0 };
					dem.restorationMethod = { 0 }; // Always 0, meaning no protection

					odu0[line][column]--; // A demand was processed
					demandIndex++;
					return dem;
				}
			}
		}
		//############################ ODU1 ####################################
		for (line = 0; line < numberOfNodes; line++)
		{
			for (column = 0; column < numberOfNodes; column++)
			{
				if (odu1[line][column] != 0) // If there are demands to be processed between this pair of nodes
				{
					dem.demandIndex = demandIndex;
					dem.sourceNode = { line + 1 };
					dem.destinationNode = { column + 1 };
					dem.oduType = { 1 };
					dem.restorationMethod = { 0 }; // Always 0, meaning no protection

					odu1[line][column]--; // A demand was processed
					demandIndex++;
					return dem;
				}
			}
		}
		//############################ ODU2 ####################################
		for (line = 0; line < numberOfNodes; line++)
		{
			for (column = 0; column < numberOfNodes; column++)
			{
				if (odu2[line][column] != 0) // If there are demands to be processed between this pair of nodes
				{
					dem.demandIndex = demandIndex;
					dem.sourceNode = { line + 1 };
					dem.destinationNode = { column + 1 };
					dem.oduType = { 2 };
					dem.restorationMethod = { 0 }; // Always 0, meaning no protection

					odu2[line][column]--; // A demand was processed
					demandIndex++;
					return dem;
				}
			}
		}
		//############################ ODU3 ####################################
		for (line = 0; line < numberOfNodes; line++)
		{
			for (column = 0; column < numberOfNodes; column++)
			{
				if (odu3[line][column] != 0) // If there are demands to be processed between this pair of nodes
				{
					dem.demandIndex = demandIndex;
					dem.sourceNode = { line + 1 };
					dem.destinationNode = { column + 1 };
					dem.oduType = { 3 };
					dem.restorationMethod = { 0 }; // Always 0, meaning no protection

					odu3[line][column]--; // A demand was processed
					demandIndex++;
					return dem;
				}
			}
		}
		//############################ ODU4 ####################################
		for (line = 0; line < numberOfNodes; line++)
		{
			for (column = 0; column < numberOfNodes; column++)
			{
				if (odu4[line][column] != 0) // If there are demands to be processed between this pair of nodes
				{
					dem.demandIndex = demandIndex;
					dem.sourceNode = { line + 1 };
					dem.destinationNode = { column + 1 };
					dem.oduType = { 4 };
					dem.restorationMethod = { 0 }; // Always 0, meaning no protection

					odu4[line][column]--; // A demand was processed
					demandIndex++;
					return dem;
				}
			}
		}
	}
}

t_integer calculateNumberOfNodes()	{ return sizeof(odu0); };

t_integer calculateNumberOfDemands()
{
	t_integer demands{ 0 };
	t_integer line{ 0 };
	t_integer column{ 0 };

	//##################### ODU0 ###############################################
	for (line = 0; line < numberOfNodes; line++)
	{
		for (column = 0; column < numberOfNodes; column++)
		{
			while (odu0[line][column] > 0) // If there are demands to be processed between this pair of nodes
			{
				demands++;
			}
		}
	}
	//##################### ODU1 ###############################################
	for (line = 0; line < numberOfNodes; line++)
	{
		for (column = 0; column < numberOfNodes; column++)
		{
			while (odu1[line][column] > 0) // If there are demands to be processed between this pair of nodes
			{
				demands++;
			}
		}
	}
	//##################### ODU2 ###############################################
	for (line = 0; line < numberOfNodes; line++)
	{
		for (column = 0; column < numberOfNodes; column++)
		{
			while (odu2[line][column] > 0) // If there are demands to be processed between this pair of nodes
			{
				demands++;
			}
		}
	}
	//##################### ODU3 ###############################################
	for (line = 0; line < numberOfNodes; line++)
	{
		for (column = 0; column < numberOfNodes; column++)
		{
			while (odu3[line][column] > 0) // If there are demands to be processed between this pair of nodes
			{
				demands++;
			}
		}
	}
	//##################### ODU4 ###############################################
	for (line = 0; line < numberOfNodes; line++)
	{
		for (column = 0; column < numberOfNodes; column++)
		{
			while (odu4[line][column] > 0) // If there are demands to be processed between this pair of nodes
			{
				demands++;
			}
		}
	}
	return demands;
};







