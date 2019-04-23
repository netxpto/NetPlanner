#include "..\include_opaque\scheduler_20190122.h"


// Global variables needed to generate a demand signal

void Scheduler::initialize(void) {

	numberOfOutputSignals = (int) outputSignals.size();

	setNumberOfNodes(calculateNumberOfNodes());
	setNumberOfDemands(calculateNumberOfDemands());
	
	numberOfNodes = getNumberOfNodes();				// Returns the number of nodes
	numberOfDemands = getNumberOfDemands();			// Returns the total number of existent demands
	demandIndex = getDemandIndex();					// Returns demandIndex value
	orderingRule = getDemandOrderingRule();			// Returns orderingRule value

}

bool Scheduler::runBlock(void) {

	t_integer_long space = outputSignals[0]->space();	// Buffer free space 

	int process = std::min(space, numberOfDemands);

	if (process == 0) return false;						// When 0 all demands were processed

	for (int k = 1; k <= process; k++) {

		t_demand_request outputDemand;
		generateDemand(orderingRule, outputDemand);
		outputSignals[0]->bufferPut((t_demand_request) outputDemand);
		numberOfDemands--;

	}

	return true;

}

bool Scheduler::generateDemand(t_integer orderingRule, t_demand_request &dem) {

	bool findDemand{ false };

	if (orderingRule == 0) { // ODU4 to ODU0
	
		//############################ ODU4 ####################################
		t_integer line{ 0 };
		while ((line < numberOfNodes) && (!findDemand)) {

			t_integer column{ 0 };
			while ((column < numberOfNodes) && (!findDemand)) {

				if (odu4[line][column] != 0) { // If there are demands to be processed between this pair of nodes
					
					findDemand = true;

					dem.demandIndex = demandIndex;
					dem.sourceNode = { line + 1 };
					dem.destinationNode = { column + 1 };
					dem.oduType = { 4 };
					dem.survivabilityMethod = { survivability_method::none }; // No protection

					odu4[line][column]--; // A demand was processed
					demandIndex++;
				}
				column++;
			}
			line++;
		}

		//############################ ODU3 ####################################
		line = 0 ;
		while (line < numberOfNodes && !findDemand) {

			t_integer column{ 0 };
			while (column < numberOfNodes && !findDemand) {

				if (odu3[line][column] != 0) { // If there are demands to be processed between this pair of nodes
				
					findDemand = true;

					dem.demandIndex = demandIndex;
					dem.sourceNode = { line + 1 };
					dem.destinationNode = { column + 1 };
					dem.oduType = { 3 };
					dem.survivabilityMethod = { survivability_method::none }; // No protection

					odu3[line][column]--; // A demand was processed
					demandIndex++;
				}
				column++;
			}
			line++;
		}

		//############################ ODU2 ####################################
		line = 0;
		while (line < numberOfNodes && !findDemand) {

			t_integer column{ 0 };
			while (column < numberOfNodes && !findDemand) {

				if (odu2[line][column] != 0) { // If there are demands to be processed between this pair of nodes
				
					findDemand = true;

					dem.demandIndex = demandIndex;
					dem.sourceNode = { line + 1 };
					dem.destinationNode = { column + 1 };
					dem.oduType = { 2 };
					dem.survivabilityMethod = { survivability_method::none }; // No protection

					odu2[line][column]--; // A demand was processed
					demandIndex++;
				}
				column++;
			}
			line++;
		}

		//############################ ODU1 ####################################
		line = 0;
		while (line < numberOfNodes && !findDemand) {

			t_integer column{ 0 };
			while (column < numberOfNodes && !findDemand) {

				if (odu1[line][column] != 0) { // If there are demands to be processed between this pair of nodes
				
					findDemand = true;

					dem.demandIndex = demandIndex;
					dem.sourceNode = { line + 1 };
					dem.destinationNode = { column + 1 };
					dem.oduType = { 1 };
					dem.survivabilityMethod = { survivability_method::none }; // No protection

					odu1[line][column]--; // A demand was processed
					demandIndex++;
				}
				column++;
			}
			line++;

		}

		//############################ ODU0 ####################################

		line = 0;
		while (line < numberOfNodes && !findDemand) {

			t_integer column{ 0 };
			while (column < numberOfNodes && !findDemand) {

				if (odu0[line][column] != 0) { // If there are demands to be processed between this pair of nodes
				
					findDemand = true;

					dem.demandIndex = demandIndex;
					dem.sourceNode = { line + 1 };
					dem.destinationNode = { column + 1 };
					dem.oduType = { 0 };
					dem.survivabilityMethod = { survivability_method::none }; // No protection

					odu0[line][column]--; // A demand was processed
					demandIndex++;
				}
				column++;
			}
			line++;
		}
	}

	else if (orderingRule == 1) { // ODU0 to ODU4
	
		//############################ ODU0 ####################################
		t_integer line{ 0 };
		while (line < numberOfNodes && !findDemand) {

			t_integer column{ 0 };
			while (column < numberOfNodes && !findDemand) {

				if (odu0[line][column] != 0) { // If there are demands to be processed between this pair of nodes
			
					findDemand = true;

					dem.demandIndex = demandIndex;
					dem.sourceNode = { line + 1 };
					dem.destinationNode = { column + 1 };
					dem.oduType = { 0 };
					dem.survivabilityMethod = { survivability_method::none }; // No protection

					odu0[line][column]--; // A demand was processed
					demandIndex++;
				}
				column++;
			}
			line++;
		}

		//############################ ODU1 ####################################
		line = 0;
		while (line < numberOfNodes && !findDemand) {

			t_integer column{ 0 };
			while (column < numberOfNodes && !findDemand) {

				if (odu1[line][column] != 0) { // If there are demands to be processed between this pair of nodes
			
					findDemand = true;

					dem.demandIndex = demandIndex;
					dem.sourceNode = { line + 1 };
					dem.destinationNode = { column + 1 };
					dem.oduType = { 1 };
					dem.survivabilityMethod = { survivability_method::none }; // No protection

					odu1[line][column]--; // A demand was processed
					demandIndex++;
				}
				column++;
			}
			line++;
		}

		//############################ ODU2 ####################################
		line = 0;
		while (line < numberOfNodes && !findDemand) {

			t_integer column{ 0 };
			while (column < numberOfNodes && !findDemand) {

				if (odu2[line][column] != 0) { // If there are demands to be processed between this pair of nodes
			
					findDemand = true;

					dem.demandIndex = demandIndex;
					dem.sourceNode = { line + 1 };
					dem.destinationNode = { column + 1 };
					dem.oduType = { 2 };
					dem.survivabilityMethod = { survivability_method::none }; // No protection

					odu2[line][column]--; // A demand was processed
					demandIndex++;
				}
				column++;

			}
			line++;
		}

		//############################ ODU3 ####################################
		line = 0;
		while (line < numberOfNodes && !findDemand) {

			t_integer column{ 0 };
			while (column < numberOfNodes && !findDemand) {

				if (odu3[line][column] != 0) { // If there are demands to be processed between this pair of nodes
			
					findDemand = true;

					dem.demandIndex = demandIndex;
					dem.sourceNode = { line + 1 };
					dem.destinationNode = { column + 1 };
					dem.oduType = { 3 };
					dem.survivabilityMethod = { survivability_method::none }; // No protection

					odu3[line][column]--; // A demand was processed
					demandIndex++;
				}
				column++;
			}
			line++;
		}

		//############################ ODU4 ####################################
		line=0;
		while ((line < numberOfNodes) && (!findDemand)) {

			t_integer column{ 0 };
			while ((column < numberOfNodes) && (!findDemand)) {

				if (odu4[line][column] != 0) { // If there are demands to be processed between this pair of nodes
			
					findDemand = true;

					dem.demandIndex = demandIndex;
					dem.sourceNode = { line + 1 };
					dem.destinationNode = { column + 1 };
					dem.oduType = { 4 };
					dem.survivabilityMethod = { survivability_method::none }; // No protection

					odu4[line][column]--; // A demand was processed
					demandIndex++;
				}
				column++;
			}
			line++;
		}
	}
	
	return findDemand;

}

t_integer Scheduler::calculateNumberOfNodes() {

	t_integer nodes = odu0[0].size() ;
	return nodes;

};

t_integer Scheduler::calculateNumberOfDemands() {

	t_integer demands{ 0 };
	t_integer line{ 0 };
	t_integer column{ 0 };

	//##################### ODU0 ###############################################
	for (line = 0; line < numberOfNodes; line++) {

		for (column = 0; column < numberOfNodes; column++) {

				demands+= odu0[line][column];
		}
	}

	//##################### ODU1 ###############################################
	for (line = 0; line < numberOfNodes; line++) {

		for (column = 0; column < numberOfNodes; column++) {

			demands += odu1[line][column];
		}
	}

	//##################### ODU2 ###############################################
	for (line = 0; line < numberOfNodes; line++) {

		for (column = 0; column < numberOfNodes; column++) {

			demands += odu2[line][column];
		}
	}
	
	//##################### ODU3 ###############################################
	for (line = 0; line < numberOfNodes; line++) {

		for (column = 0; column < numberOfNodes; column++) {

			demands += odu3[line][column];
		}
	}

	//##################### ODU4 ###############################################
	for (line = 0; line < numberOfNodes; line++) {

		for (column = 0; column < numberOfNodes; column++) {

			demands += odu4[line][column];
		}
	}

	return demands;

};







