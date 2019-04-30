#include "..\include_opaque\logical_topology_manager_20190419.h"

void LogicalTopologyManager::initialize(void){

}

bool LogicalTopologyManager::runBlock(void) {

	t_integer readyLogicalTopology = inputSignals[0]->ready();
	t_integer readyDemands = inputSignals[1]->ready();
	
	if (readyDemands == 0 || readyLogicalTopology == 0) return false;

	t_path_request pathRequest;
	t_path_request_routed pathRequestRouted;
	t_path_routed pathRouted;
	t_light_paths_table lightPathsTable;

	t_demand_request_routed processedDemands;

	t_path path;
	t_light_path lightPath;
	t_optical_channel opticalChannel;

	inputSignals[0]->bufferGet(&logicalTopology);

	//paths, lightPaths e opticalChannels para teste
	/*
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
	path.destinationNode = 6;
	path.capacity = 80;
	path.numberOfLightPaths = 1;
	path.lightPathsIndex.push_back(3);
	logicalTopology.paths.push_back(path);
	path.lightPathsIndex.clear();


	lightPath.lightPathIndex = 0;
	lightPath.sourceNode = 1;
	lightPath.destinationNode = 2;
	lightPath.capacity = 80;
	lightPath.numberOfOpticalChannels = 1;
	lightPath.opticalChannelsIndex.push_back(0);
	logicalTopology.lightPaths.push_back(lightPath);
	lightPath.opticalChannelsIndex.clear();

	lightPath.lightPathIndex = 1;
	lightPath.sourceNode = 1;
	lightPath.destinationNode = 2;
	lightPath.capacity = 80;
	lightPath.numberOfOpticalChannels = 1;
	lightPath.opticalChannelsIndex.push_back(1);
	logicalTopology.lightPaths.push_back(lightPath);
	lightPath.opticalChannelsIndex.clear();

	lightPath.lightPathIndex = 2;
	lightPath.sourceNode = 2;
	lightPath.destinationNode = 3;
	lightPath.capacity = 80;
	lightPath.numberOfOpticalChannels = 1;
	lightPath.opticalChannelsIndex.push_back(2);
	logicalTopology.lightPaths.push_back(lightPath);
	lightPath.opticalChannelsIndex.clear();

	lightPath.lightPathIndex = 3;
	lightPath.sourceNode = 1;
	lightPath.destinationNode = 6;
	lightPath.capacity = 80;
	lightPath.numberOfOpticalChannels = 1;
	lightPath.opticalChannelsIndex.push_back(3);
	logicalTopology.lightPaths.push_back(lightPath);
	lightPath.opticalChannelsIndex.clear();

	t_optical_channel opticalChannel;

	opticalChannel.opticalChannelIndex = 0;
	opticalChannel.sourceNode = 1;
	opticalChannel.destinationNode = 2;
	opticalChannel.capacity = 80;
	opticalChannel.wavelength = 1550;
	opticalChannel.numberOfDemands = 0;
	logicalTopology.opticalChannels.push_back(opticalChannel);

	opticalChannel.opticalChannelIndex = 1;
	opticalChannel.sourceNode = 1;
	opticalChannel.destinationNode = 2;
	opticalChannel.capacity = 80;
	opticalChannel.wavelength = 1550;
	opticalChannel.numberOfDemands = 0;
	logicalTopology.opticalChannels.push_back(opticalChannel);

	opticalChannel.opticalChannelIndex = 2;
	opticalChannel.sourceNode = 2;
	opticalChannel.destinationNode = 3;
	opticalChannel.capacity = 80;
	opticalChannel.wavelength = 1550;
	opticalChannel.numberOfDemands = 0;
	logicalTopology.opticalChannels.push_back(opticalChannel);

	opticalChannel.opticalChannelIndex = 3;
	opticalChannel.sourceNode = 1;
	opticalChannel.destinationNode = 6;
	opticalChannel.capacity = 80;
	opticalChannel.wavelength = 1550;
	opticalChannel.numberOfDemands = 0;
	logicalTopology.opticalChannels.push_back(opticalChannel);
	*/

	for (t_integer d = 0; d < readyDemands; d++) {

		inputSignals[1]->bufferGet(&demand);

		t_integer odu0s;
		switch (demand.oduType) {
		case 4: odu0s = 80; //odu4 = 80 odu0s
			break;
		case 3: odu0s = 32; //odu3 = 32 odu0s
			break;
		case 2: odu0s = 8;  //odu2 = 8 odu0s
			break;
		case 1: odu0s = 2;  //odu1 = 2 odu0s
			break;
		case 0: odu0s = 1;  //odu0 
			break;
		}

		bool pathFound{ false };

		t_integer i{ 0 };
		while ((i < (t_integer)logicalTopology.paths.size()) && (!pathFound)) {

			if (demand.sourceNode == logicalTopology.paths[i].sourceNode
				&& demand.destinationNode == logicalTopology.paths[i].destinationNode
				&& odu0s <= logicalTopology.paths[i].capacity) {

				logicalTopology.paths[i].capacity -= odu0s;

				std::vector<t_integer> lightPathsIndex = logicalTopology.paths[i].lightPathsIndex;

				for (t_integer j = 0; j < (t_integer)lightPathsIndex.size(); j++) {

					t_integer lightPathIndex = lightPathsIndex[j];
					logicalTopology.lightPaths[lightPathIndex].capacity -= odu0s;

					std::vector<t_integer> opticalChannelsIndex = logicalTopology.lightPaths[lightPathIndex].opticalChannelsIndex;

					for (t_integer k = 0; k < (t_integer)opticalChannelsIndex.size(); k++) {

						t_integer opticalChannelIndex = opticalChannelsIndex[k];
						logicalTopology.opticalChannels[opticalChannelIndex].capacity -= odu0s;
						logicalTopology.opticalChannels[opticalChannelIndex].numberOfDemands++;

						if (logicalTopology.opticalChannels[opticalChannelIndex].numberOfDemands == 1)
							logicalTopology.opticalChannels[opticalChannelIndex].demandsIndex.push_back(demand.demandIndex);
						else {
							logicalTopology.opticalChannels[opticalChannelIndex].demandsIndex.push_back(demand.demandIndex); // corrigir para quando já existiam demands
						}

					}
				}

				pathFound = true;
				processedDemands.routed = true;
				processedDemands.pathIndex = logicalTopology.paths[i].pathIndex;

			}
			i++;
		}


		if (pathFound == false) { // se não existir o path

			//1º verificar se já foi feito o dijkstra com o nó origem, portanto se já foi encontrado um caminho curto
			//se não, fazer o dijkstra (demand.sourceNode, demand.destinationNode) ex: dijkstra(1,5)
			// 1->6->5
			//resultado lightpaths
			//lp1 => sourceNode = 1, destinationNode = 6;
			//lp2 => sourceNode = 6, destinationNode = 5;
			
			//dijkstra(logicalTopology.logicalTopologyAdjacencyMatrix, demand.sourceNode, demand.destinationNode);
			
			//procurar se ja existem os lightPaths, se existirem e com capacidade não necessita de ir à topologia física
			bool lightPathCapacityEnough{ true };
			std::vector<int> lightPathsIndex;

			for (int i = 0; i < (t_integer)logicalTopology.lightPaths.size; i++) {

				if (src == logicalTopology.lightPaths[lp].sourceNode && dst == logicalTopology.lightPaths[lp].destinationNode) {

					if (logicalTopology.lightPaths[lp].capacity < odu0s) {
						lightPathCapacityEnough = false;
						break;
					}
					else if (logicalTopology.lightPaths[lp].capacity >= odu0s) {
						lightPathsIndex.push_back(lp);
					}
				}
			}

			if (lightPathCapacityEnough == true) {

				for (int i = 0; i < lightPathsIndex.size(); i++) {

					logicalTopology.lightPaths[i].capacity -= odu0s;
					logicalTopology.opticalChannels[i].capacity -= odu0s;
					logicalTopology.opticalChannels[i].numberOfDemands++;
					logicalTopology.opticalChannels[i].demandsIndex.push_back(demand.demandIndex);
					logicalTopology.paths[logicalTopology.paths.size()].sourceNode = demand.sourceNode;
					logicalTopology.paths[logicalTopology.paths.size()].destinationNode = demand.destinationNode;
					logicalTopology.paths[logicalTopology.paths.size()].capacity = 80 - odu0s;
					logicalTopology.paths[logicalTopology.paths.size()].numberOfLightPaths = lightPathsIndex.size();
					logicalTopology.paths[logicalTopology.paths.size()].lightPathsIndex = lightPathsIndex;
				}
			}

			else if (lightPathCapacityEnough == false) {
				//lightPathRequest
			}
		}

			

		//simular pathRequests
		if (demand.demandIndex == 0) {

			pathRequest.requestIndex = 0;
			pathRequest.sourceNode = 1;
			pathRequest.destinationNode = 3;
			pathRequest.numberOfIntermediateNodes = 1;
			pathRequest.intermediateNodes.push_back(2);
		}

		else if (demand.demandIndex == 4) {

			pathRequest.requestIndex = 0;
			pathRequest.sourceNode = 1;
			pathRequest.destinationNode = 3;
			pathRequest.numberOfIntermediateNodes = 1;
			pathRequest.intermediateNodes.clear();
			//pathRequest.intermediateNodes.push_back(6);
			pathRequest.intermediateNodes.push_back(2);

		}

		else if (demand.demandIndex == 2) {

			pathRequest.requestIndex = 2;
			pathRequest.sourceNode = 1;
			pathRequest.destinationNode = 3;
			pathRequest.numberOfIntermediateNodes = 2;
			pathRequest.intermediateNodes.clear();
			pathRequest.intermediateNodes.push_back(6);
			pathRequest.intermediateNodes.push_back(2);

		}

		outputSignals[0]->bufferPut((t_path_request)pathRequest);
			
		t_integer readyPathRequestRouted = inputSignals[2]->ready();

		inputSignals[2]->bufferGet(&pathRequestRouted);

			
		/*
		// pathRequestRouted simulado 
		pathRouted.requestIndex = 0;
		pathRouted.routed = true;
		pathRouted.numberOfLightPaths = 2;
		pathRequestRouted.pathRouted.push_back(pathRouted);

		lightPathsTable.sourceNode = 1;
		lightPathsTable.destinationNode = 2;
		lightPathsTable.numberOfIntermediateNodes = 0;
		lightPathsTable.wavelength = 1550;
		pathRequestRouted.lightPathsTable.clear();
		pathRequestRouted.lightPathsTable.push_back(lightPathsTable);

		lightPathsTable.sourceNode = 2;
		lightPathsTable.destinationNode = 3;
		lightPathsTable.numberOfIntermediateNodes = 0;
		lightPathsTable.wavelength = 1550;
		pathRequestRouted.lightPathsTable.push_back(lightPathsTable);

			
		if (pathRequestRouted.pathRouted[0].routed == true) {
			
			for (int l = 0; l < pathRequestRouted.lightPathsTable.size(); l++) {

				opticalChannel.opticalChannelIndex = logicalTopology.opticalChannels.size();
				opticalChannel.sourceNode = pathRequestRouted.lightPathsTable[l].sourceNode;
				opticalChannel.destinationNode = pathRequestRouted.lightPathsTable[l].destinationNode;
				opticalChannel.wavelength = pathRequestRouted.lightPathsTable[l].wavelength;
				opticalChannel.capacity = 80 - odu0s; // corrigir os 80
				opticalChannel.numberOfDemands = 1;
				opticalChannel.demandsIndex.push_back(demand.demandIndex);
				logicalTopology.opticalChannels.push_back(opticalChannel);
				opticalChannel.demandsIndex.clear();

				lightPath.lightPathIndex = logicalTopology.lightPaths.size();
				lightPath.sourceNode = pathRequestRouted.lightPathsTable[l].sourceNode;
				lightPath.destinationNode = pathRequestRouted.lightPathsTable[l].destinationNode;
				lightPath.capacity = 80 - odu0s; // corrigir os 80
				lightPath.numberOfOpticalChannels = 1;
				lightPath.opticalChannelsIndex.push_back(opticalChannel.opticalChannelIndex);
				logicalTopology.lightPaths.push_back(lightPath);
				lightPath.opticalChannelsIndex.clear();
			}

			path.pathIndex = logicalTopology.paths.size();
			path.sourceNode = pathRequest.sourceNode;
			path.destinationNode = pathRequest.destinationNode;
			path.capacity = 80 - odu0s; // corrigir os 80
			path.numberOfLightPaths = pathRequestRouted.pathRouted[0].numberOfLightPaths;	
			path.lightPathsIndex.push_back(0);
			path.lightPathsIndex.push_back(1);
			logicalTopology.paths.push_back(path);
			
			*/

			//processedDemands.pathIndex = logicalTopology.paths[0].pathIndex;
		//}

			//else if (pathRequestRouted.pathRouted[0].routed == false) {

				//processedDemands.routed = false;

			//}

	}

	//processedDemands.demandIndex = demand.demandIndex;
	//outputSignals[2]->bufferPut((t_demand_request_routed)processedDemands);

	outputSignals[1]->bufferPut((t_logical_topology)logicalTopology);

	return true;
};

t_integer LogicalTopologyManager::dijkstra(std::vector<vector<int>> graph, t_integer src, t_integer dst) {

	t_integer n = graph.size();
	std::vector<t_integer> dist(n);
	std::vector<bool> vis(n);
	std::vector<t_integer> prev;

	for (t_integer i = 0; i < n; i++)
		dist[i] = INT_MAX;

	dist[src] = 0;

	for (t_integer i = 0; i < n; i++) {
		t_integer u = -1;
		for (t_integer j = 0; j < n; j++) {
			if (vis[j]) continue;
			if (u == -1 || dist[j] < dist[u]) 
				u = j;
		}

		vis[u] = true;
		for (t_integer j = 0; j < n; j++) {
			t_integer path = dist[u] + graph[u][j];
			if (path < dist[j]){
				dist[j] = path;
				prev[j] = u;
			}
		}
	}

	return dist[dst], prev[dst];
}