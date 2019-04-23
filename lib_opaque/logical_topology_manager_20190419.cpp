#include "..\include_opaque\logical_topology_manager_20190419.h"

void LogicalTopologyManager::initialize(void){

	numberOfOutputSignals = (int)outputSignals.size();
}

bool LogicalTopologyManager::runBlock(void) {

	t_integer readyLogicalTopology = inputSignals[0]->ready();
	t_integer readyDemands = inputSignals[1]->ready();
	
	if (readyDemands == 0 || readyLogicalTopology == 0) return false;

	t_logical_topology logicalTopology;
	t_demand_request demand;
	t_path_request pathRequest;
	t_path_request_routed pathRequestRouted;
	t_demand_request_routed processedDemands;

	t_path path;
	t_light_path lightPath;

	inputSignals[0]->bufferGet(&logicalTopology);

	
	// paths e lightPaths para teste

	path.pathIndex = 0;
	path.sourceNode = 1;
	path.destinationNode = 2;
	path.capacity = 80;
	path.numberOfLightPaths = 1;
	path.lightPathsIndex.push_back(0);
	logicalTopology.paths.push_back(path);
	path.lightPathsIndex.clear();
	
	path.pathIndex = 1;
	path.sourceNode = 1;
	path.destinationNode = 3;
	path.capacity = 80;
	path.numberOfLightPaths = 2;
	path.lightPathsIndex.push_back(1);
	path.lightPathsIndex.push_back(2);
	logicalTopology.paths.push_back(path);
	path.lightPathsIndex.clear();

	path.pathIndex = 2;
	path.sourceNode = 1;
	path.destinationNode = 3;
	path.capacity = 80;
	path.numberOfLightPaths = 3;
	path.lightPathsIndex.push_back(3);
	path.lightPathsIndex.push_back(4);
	path.lightPathsIndex.push_back(5);
	logicalTopology.paths.push_back(path);
	path.lightPathsIndex.clear();

	path.pathIndex = 3;
	path.sourceNode = 1;
	path.destinationNode = 3;
	path.capacity = 80;
	path.numberOfLightPaths = 3;
	path.lightPathsIndex.push_back(6);
	path.lightPathsIndex.push_back(7);
	path.lightPathsIndex.push_back(8);
	logicalTopology.paths.push_back(path);
	path.lightPathsIndex.clear();

	path.pathIndex = 4;
	path.sourceNode = 1;
	path.destinationNode = 6;
	path.capacity = 80;
	path.numberOfLightPaths = 1;
	path.lightPathsIndex.push_back(6);
	logicalTopology.paths.push_back(path);
	path.lightPathsIndex.clear();


	lightPath.lightPathIndex = 0;
	lightPath.sourceNode = 1;
	lightPath.destinationNode = 2;
	lightPath.capacity = 80;
	lightPath.numberOfOpticalChannels = 1;
	logicalTopology.lightPaths.push_back(lightPath);

	lightPath.lightPathIndex = 1;
	lightPath.sourceNode = 1;
	lightPath.destinationNode = 2;
	lightPath.capacity = 80;
	lightPath.numberOfOpticalChannels = 1;
	logicalTopology.lightPaths.push_back(lightPath);

	lightPath.lightPathIndex = 2;
	lightPath.sourceNode = 2;
	lightPath.destinationNode = 3;
	lightPath.capacity = 80;
	lightPath.numberOfOpticalChannels = 1;
	logicalTopology.lightPaths.push_back(lightPath);

	lightPath.lightPathIndex = 3;
	lightPath.sourceNode = 1;
	lightPath.destinationNode = 6;
	lightPath.capacity = 80;
	lightPath.numberOfOpticalChannels = 1;
	logicalTopology.lightPaths.push_back(lightPath);

	lightPath.lightPathIndex = 4;
	lightPath.sourceNode = 6;
	lightPath.destinationNode = 2;
	lightPath.capacity = 80;
	lightPath.numberOfOpticalChannels = 1;
	logicalTopology.lightPaths.push_back(lightPath);

	lightPath.lightPathIndex = 5;
	lightPath.sourceNode = 2;
	lightPath.destinationNode = 3;
	lightPath.capacity = 80;
	lightPath.numberOfOpticalChannels = 1;
	logicalTopology.lightPaths.push_back(lightPath);

	lightPath.lightPathIndex = 6;
	lightPath.sourceNode = 1;
	lightPath.destinationNode = 6;
	lightPath.capacity = 80;
	lightPath.numberOfOpticalChannels = 1;
	logicalTopology.lightPaths.push_back(lightPath);

	lightPath.lightPathIndex = 7;
	lightPath.sourceNode = 6;
	lightPath.destinationNode = 5;
	lightPath.capacity = 80;
	lightPath.numberOfOpticalChannels = 1;
	logicalTopology.lightPaths.push_back(lightPath);

	lightPath.lightPathIndex = 8;
	lightPath.sourceNode = 5;
	lightPath.destinationNode = 3;
	lightPath.capacity = 80;
	lightPath.numberOfOpticalChannels = 1;
	logicalTopology.lightPaths.push_back(lightPath);
	
	
	for (t_integer d = 0; d < readyDemands; d++) {

		inputSignals[1]->bufferGet(&demand);

		t_integer odu0s;
		switch (demand.oduType) {
			case 4: odu0s = 80;
				break;
			case 3: odu0s = 32;
				break;
			case 2: odu0s = 8;
				break;
			case 1: odu0s = 2;
				break;
			case 0: odu0s = 1;
				break;
		}

		for (int i = 0; i < (int)logicalTopology.paths.size(); i++) {

			if (demand.sourceNode == logicalTopology.paths[i].sourceNode
				&& demand.destinationNode == logicalTopology.paths[i].destinationNode
				&& odu0s <= logicalTopology.paths[i].capacity) {

				logicalTopology.paths[i].capacity -= odu0s;

				std::vector<t_integer> lightPathsIndex = logicalTopology.paths[i].lightPathsIndex;

				for (int j = 0; j < (int)lightPathsIndex.size(); j++) {

					t_integer lightPathIndex = lightPathsIndex[j];
					logicalTopology.lightPaths[lightPathIndex].capacity -= odu0s;
				}

				processedDemands.pathIndex = logicalTopology.paths[i].pathIndex;

			}
		}


		//simular pathRequests
		if (d == 0) {

			pathRequest.requestIndex = 0;
			pathRequest.sourceNode = 1;
			pathRequest.destinationNode = 2;
			pathRequest.numberOfIntermediateNodes = 0;

		}

		else if (d == 1) {

			pathRequest.requestIndex = 1;
			pathRequest.sourceNode = 1;
			pathRequest.destinationNode = 3;
			pathRequest.numberOfIntermediateNodes = 1;
			pathRequest.intermediateNodes.clear();
			pathRequest.intermediateNodes.push_back(2);

		}

		else if (d == 2) {

			pathRequest.requestIndex = 2;
			pathRequest.sourceNode = 1;
			pathRequest.destinationNode = 3;
			pathRequest.numberOfIntermediateNodes = 2;
			pathRequest.intermediateNodes.clear();
			pathRequest.intermediateNodes.push_back(6);
			pathRequest.intermediateNodes.push_back(2);

		}

		else if (d == 3) {

			pathRequest.requestIndex = 3;
			pathRequest.sourceNode = 1;
			pathRequest.destinationNode = 3;
			pathRequest.numberOfIntermediateNodes = 2;
			pathRequest.intermediateNodes.clear();
			pathRequest.intermediateNodes.push_back(6);
			pathRequest.intermediateNodes.push_back(5);

		}

		else if (d == 5) {

			pathRequest.requestIndex = 4;
			pathRequest.sourceNode = 1;
			pathRequest.destinationNode = 6;
			pathRequest.numberOfIntermediateNodes = 0;
			pathRequest.intermediateNodes.clear();
		}

		outputSignals[0]->bufferPut((t_path_request)pathRequest);

		t_integer readyPathRequestRouted = inputSignals[2]->ready();

		inputSignals[2]->bufferGet(&pathRequestRouted);

		/*
		//pathIndex
		path.sourceNode = pathRequest.sourceNode;
		path.destinationNode = pathRequest.destinationNode;
		path.capacity = 80 - odu0s; // corrigir os 80
		path.numberOfLightPaths = pathRequest.numberOfIntermediateNodes+1;
		//path.lightPathsIndex.push_back(0);
		logicalTopology.paths.push_back(path);

		for (int l = 0; l < pathRequestRouted.lightPathsTable.size(); l++) {

			lightPath.sourceNode = pathRequestRouted.lightPathsTable[l].sourceNode;
			lightPath.destinationNode = pathRequestRouted.lightPathsTable[l].destinationNode;
			logicalTopology.lightPaths.push_back(lightPath);
		}
		*/

		processedDemands.demandIndex = d;
		processedDemands.routed = true;
		outputSignals[2]->bufferPut((t_demand_request_routed)processedDemands);

	}	
			
	outputSignals[1]->bufferPut((t_logical_topology)logicalTopology);
		
	return true;
};
