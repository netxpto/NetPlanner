#include "..\include_opaque\logical_topology_manager_20190419.h"

void LogicalTopologyManager::initialize(void)
{

}

bool LogicalTopologyManager::runBlock(void)
{
	t_integer readyLogicalTopology = inputSignals[0]->ready();
	t_integer readyDemands = inputSignals[1]->ready();
	t_integer readyPathRequestRouted = inputSignals[2]->ready();

	if (readyDemands == 0 && readyLogicalTopology == 0 && readyPathRequestRouted == 0) return false;

	t_demand_request demand;
	t_path_request pathRequest;
	t_path_request_routed pathRequestRouted;
	t_light_paths_table lightPathsTable;
	t_demand_request_routed processedDemands;
	t_path path;
	t_light_path lightPath;
	t_optical_channel opticalChannel;

	if (readyLogicalTopology != 0)
	{
		for (t_integer i = 0; i < readyLogicalTopology; i++)
			inputSignals[0]->bufferGet(&logicalTopology);
	}
		
	if (readyPathRequestRouted != 0)
	{
		for (t_integer pR = 0; pR < readyPathRequestRouted; pR++)
		{
			inputSignals[2]->bufferGet(&pathRequestRouted);

			if (pathRequestRouted.pathInformation.routed == true)
			{
				std::vector<t_integer> lightPathsIndex;

				t_integer odu0s;
				switch (pathRequestRouted.pathInformation.oduType)
				{
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

				if (pathRequestRouted.pathInformation.pathTotal == 1)
				{
					for (size_t l = 0; l < pathRequestRouted.lightPathsTable.size(); l++)
					{
						opticalChannel.opticalChannelIndex = logicalTopology.opticalChannels.size();
						opticalChannel.sourceNode = pathRequestRouted.lightPathsTable[l].sourceNode;
						opticalChannel.destinationNode = pathRequestRouted.lightPathsTable[l].destinationNode;
						opticalChannel.wavelength = pathRequestRouted.lightPathsTable[l].wavelength;
						opticalChannel.capacity = 80 - odu0s;
						opticalChannel.numberOfDemands = 1;
						opticalChannel.demandsIndex.push_back(pathRequestRouted.pathInformation.demandIndex);
						logicalTopology.opticalChannels.push_back(opticalChannel);
						opticalChannel.demandsIndex.clear();

						lightPath.lightPathIndex = logicalTopology.lightPaths.size();
						lightPath.sourceNode = pathRequestRouted.lightPathsTable[l].sourceNode;
						lightPath.destinationNode = pathRequestRouted.lightPathsTable[l].destinationNode;
						lightPath.capacity = 80 - odu0s;
						lightPath.numberOfOpticalChannels = 1;
						lightPath.opticalChannelsIndex.push_back(opticalChannel.opticalChannelIndex);
						logicalTopology.lightPaths.push_back(lightPath);
						lightPath.opticalChannelsIndex.clear();

						lightPathsIndex.push_back(lightPath.lightPathIndex);
					}
					path.pathIndex = logicalTopology.paths.size();
					path.sourceNode = pathRequestRouted.lightPathsTable[0].sourceNode;
					path.destinationNode = pathRequestRouted.lightPathsTable[pathRequestRouted.lightPathsTable.size() - 1].destinationNode;
					path.capacity = 80 - odu0s;
					path.numberOfLightPaths = pathRequestRouted.pathInformation.numberOfLightPaths;
					path.lightPathsIndex = lightPathsIndex;
					logicalTopology.paths.push_back(path);

					processedDemands.routed = true;
					processedDemands.pathIndex = path.pathIndex;
				}

				else if (pathRequestRouted.pathInformation.pathTotal == 0)
				{
					for (size_t l = 0; l < pathRequestRouted.lightPathsTable.size(); l++)
					{
						opticalChannel.opticalChannelIndex = logicalTopology.opticalChannels.size();
						opticalChannel.sourceNode = pathRequestRouted.lightPathsTable[l].sourceNode;
						opticalChannel.destinationNode = pathRequestRouted.lightPathsTable[l].destinationNode;
						opticalChannel.wavelength = pathRequestRouted.lightPathsTable[l].wavelength;
						opticalChannel.capacity = 80;
						opticalChannel.numberOfDemands = 0;
						//opticalChannel.demandsIndex.push_back(pathRequestRouted.pathInformation.demandIndex);
						logicalTopology.opticalChannels.push_back(opticalChannel);
						//opticalChannel.demandsIndex.clear();

						lightPath.lightPathIndex = logicalTopology.lightPaths.size();
						lightPath.sourceNode = pathRequestRouted.lightPathsTable[l].sourceNode;
						lightPath.destinationNode = pathRequestRouted.lightPathsTable[l].destinationNode;
						lightPath.capacity = 80;
						lightPath.numberOfOpticalChannels = 1;
						lightPath.opticalChannelsIndex.push_back(opticalChannel.opticalChannelIndex);
						logicalTopology.lightPaths.push_back(lightPath);
						lightPath.opticalChannelsIndex.clear();

						lightPathsIndex.push_back(lightPath.lightPathIndex);
					}
					adjacency_list_t adjacency_list(logicalTopology.logicalTopologyAdjacencyMatrix.size());

					for (size_t i = 0; i < logicalTopology.logicalTopologyAdjacencyMatrix.size(); i++)
					{
						for (size_t j = 0; j < logicalTopology.logicalTopologyAdjacencyMatrix.size(); j++)
						{
							if (logicalTopology.logicalTopologyAdjacencyMatrix[i][j] != 0)
							{
								//if (routingCriterionLogicalTopology == "hops")
								adjacency_list[i].push_back(neighbor(j, 1));
							}
						}
					}
					
					std::vector<weight_t> min_distance;
					std::vector<vertex_t> previous;
					dijkstraComputePaths(pathRequestRouted.pathInformation.demandSourceNode - 1, adjacency_list, min_distance, previous);
					std::list<vertex_t> pathDijkstra = dijkstraGetShortestPathTo(pathRequestRouted.pathInformation.demandDestinationNode - 1, previous);
					std::vector<vertex_t> myPath(pathDijkstra.begin(), pathDijkstra.end());
					
					std::vector<t_integer> lightPathsIndex;
					std::vector<t_integer> capacityLightPaths;

					for (size_t i = 0; i < myPath.size() - 1; i++)
					{
						t_integer src = myPath[i] + 1;
						t_integer dst = myPath[i + 1] + 1;

						size_t j = 0;
						while (j < logicalTopology.lightPaths.size())
						{
							if (src == logicalTopology.lightPaths[j].sourceNode && dst == logicalTopology.lightPaths[j].destinationNode)
							{
								if (logicalTopology.lightPaths[j].capacity >= odu0s)
								{
									logicalTopology.lightPaths[j].capacity -= odu0s;
									logicalTopology.opticalChannels[j].capacity -= odu0s;
									logicalTopology.opticalChannels[j].numberOfDemands++;
									logicalTopology.opticalChannels[j].demandsIndex.push_back(pathRequestRouted.pathInformation.demandIndex);
									
									for (size_t l = 0; l < logicalTopology.paths.size(); l++)
									{
										for (size_t m = 0; m < logicalTopology.paths[l].lightPathsIndex.size(); m++)
										{
											if (logicalTopology.paths[l].lightPathsIndex[m] == j)
											{
												if (logicalTopology.paths[l].capacity > logicalTopology.lightPaths[j].capacity)
													logicalTopology.paths[l].capacity = logicalTopology.lightPaths[j].capacity;
											}
										}
									}

									capacityLightPaths.push_back(logicalTopology.lightPaths[j].capacity);
									lightPathsIndex.push_back(j);
								}		
							}
							j++;
						}
					}
					
					path.pathIndex = logicalTopology.paths.size();
					path.sourceNode = pathRequestRouted.pathInformation.demandSourceNode;
					path.destinationNode = pathRequestRouted.pathInformation.demandDestinationNode;
					t_integer minCapacity = capacityLightPaths[0];
					for (size_t i = 0; i < capacityLightPaths.size(); i++)
					{
						if (capacityLightPaths[i] < minCapacity)
							minCapacity = capacityLightPaths[i];
					}
					path.capacity = minCapacity; 
					path.numberOfLightPaths = lightPathsIndex.size();
					path.lightPathsIndex = lightPathsIndex;
					logicalTopology.paths.push_back(path);

					processedDemands.routed = true;
					processedDemands.pathIndex = path.pathIndex;
				}
			}
			else
			{
				processedDemands.routed = false;
				processedDemands.pathIndex = -1;
				
			}
			processedDemands.demandIndex = pathRequestRouted.pathInformation.demandIndex;
			outputSignals[2]->bufferPut((t_demand_request_routed)processedDemands);
		}
	}
	
	if (readyDemands != 0)
	{
		for (t_integer d = 0; d < readyDemands; d++)
		{
			inputSignals[1]->bufferGet(&demand);

			t_integer odu0s;
			switch (demand.oduType)
			{
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

			std::vector<t_integer> lightPathsIndex;
			bool pathFound = false;
			size_t i = 0;

			while ((i < logicalTopology.paths.size()) && (!pathFound))
			{
				if (demand.sourceNode == logicalTopology.paths[i].sourceNode && demand.destinationNode == logicalTopology.paths[i].destinationNode)
				{
					if (odu0s <= logicalTopology.paths[i].capacity)
					{
						logicalTopology.paths[i].capacity -= odu0s;

						 lightPathsIndex = logicalTopology.paths[i].lightPathsIndex;

						for (size_t j = 0; j < lightPathsIndex.size(); j++)
						{
							t_integer lightPathIndex = lightPathsIndex[j];
							logicalTopology.lightPaths[lightPathIndex].capacity -= odu0s;

							std::vector<t_integer> opticalChannelsIndex = logicalTopology.lightPaths[lightPathIndex].opticalChannelsIndex;

							for (size_t k = 0; k < opticalChannelsIndex.size(); k++)
							{
								t_integer opticalChannelIndex = opticalChannelsIndex[k];
								logicalTopology.opticalChannels[opticalChannelIndex].capacity -= odu0s;
								logicalTopology.opticalChannels[opticalChannelIndex].numberOfDemands++;
								logicalTopology.opticalChannels[opticalChannelIndex].demandsIndex.push_back(demand.demandIndex);
							}
						}
						pathFound = true;
						processedDemands.demandIndex = demand.demandIndex;
						processedDemands.routed = true;
						processedDemands.pathIndex = logicalTopology.paths[i].pathIndex;
						outputSignals[2]->bufferPut((t_demand_request_routed)processedDemands);
					}
				}
				i++;
			}
			
			if (pathFound == true) // verficar se outros paths utilizam os lightPaths que foram atualizados, de forma a atualizar a sua capacidade 
			{
				for (size_t k = 0; k < lightPathsIndex.size(); k++)
				{
					for (size_t l = 0; l < logicalTopology.paths.size(); l++)
					{
						for (size_t m = 0; m < logicalTopology.paths[l].lightPathsIndex.size(); m++)
						{
							if (logicalTopology.paths[l].lightPathsIndex[m] == lightPathsIndex[k])
							{
								if (logicalTopology.paths[l].capacity > logicalTopology.lightPaths[lightPathsIndex[k]].capacity)
									logicalTopology.paths[l].capacity = logicalTopology.lightPaths[lightPathsIndex[k]].capacity;
							}
						}
					}
				}
			}
			
			if (pathFound == false) 
			{
				adjacency_list_t adjacency_list(logicalTopology.logicalTopologyAdjacencyMatrix.size());

				for (size_t i = 0; i < logicalTopology.logicalTopologyAdjacencyMatrix.size(); i++)
				{
					for (size_t j = 0; j < logicalTopology.logicalTopologyAdjacencyMatrix.size(); j++)
					{
						if (logicalTopology.logicalTopologyAdjacencyMatrix[i][j] != 0)
						{
							//if (routingCriterionLogicalTopology == "hops")
								adjacency_list[i].push_back(neighbor(j, 1));
						}
					}
				}
				std::vector<weight_t> min_distance;
				std::vector<vertex_t> previous;
				dijkstraComputePaths(demand.sourceNode - 1, adjacency_list, min_distance, previous);
				std::list<vertex_t> pathDijkstra = dijkstraGetShortestPathTo(demand.destinationNode - 1, previous);
				std::vector<vertex_t> myPath(pathDijkstra.begin(), pathDijkstra.end());
				
				bool lightPathsFound = false;
				bool lightPathsWithoutCapacity = false;
				
				std::vector<t_integer> lightPathsIndex;
				std::vector<t_integer> lightPathsWithoutCapacityIndex;

				for (size_t i = 0; i < myPath.size() - 1; i++)
				{
					t_integer src = myPath[i] + 1;
					t_integer dst = myPath[i + 1] + 1;
					
					bool lightPathWithCapacity = false;
					size_t j = 0;
					while (j < logicalTopology.lightPaths.size() && lightPathWithCapacity == false)
					{
						if (src == logicalTopology.lightPaths[j].sourceNode && dst == logicalTopology.lightPaths[j].destinationNode)
						{
							if (logicalTopology.lightPaths[j].capacity >= odu0s)
							{
								lightPathsIndex.push_back(j);
								lightPathWithCapacity = true;
							}
							else if (logicalTopology.lightPaths[j].capacity < odu0s)
							{
								lightPathsWithoutCapacityIndex.push_back(j);
								lightPathsWithoutCapacity = true;
							}
						}
						j++;
					}
				}
				
				if (lightPathsIndex.size() == myPath.size() - 1)
					lightPathsFound = true;

				if (lightPathsFound == true) 
				{
					std::vector<t_integer> capacityLightPaths;

					for (size_t k = 0; k < lightPathsIndex.size(); k++)
					{
						// atualizar lightPaths e opticalChannels
						logicalTopology.lightPaths[lightPathsIndex[k]].capacity -= odu0s;
						logicalTopology.opticalChannels[lightPathsIndex[k]].capacity -= odu0s;
						logicalTopology.opticalChannels[lightPathsIndex[k]].numberOfDemands++;
						logicalTopology.opticalChannels[lightPathsIndex[k]].demandsIndex.push_back(demand.demandIndex);

						//verificar se existem outros paths a usar estes lightPaths e atualizar a capacidade do path com o menor valor 
						for (size_t l = 0; l < logicalTopology.paths.size(); l++)
						{
							for (size_t m = 0; m < logicalTopology.paths[l].lightPathsIndex.size(); m++)
							{
								if (logicalTopology.paths[l].lightPathsIndex[m] == lightPathsIndex[k]) 
								{
									if (logicalTopology.paths[l].capacity > logicalTopology.lightPaths[lightPathsIndex[k]].capacity)
										logicalTopology.paths[l].capacity = logicalTopology.lightPaths[lightPathsIndex[k]].capacity;
								}
							}
						}

						capacityLightPaths.push_back(logicalTopology.lightPaths[lightPathsIndex[k]].capacity);
					}
					// criar novo path
					path.pathIndex = logicalTopology.paths.size();
					path.sourceNode = demand.sourceNode;
					path.destinationNode = demand.destinationNode;
					t_integer minCapacity = capacityLightPaths[0];
					for (size_t i = 0; i < capacityLightPaths.size(); i++)
					{
						if (capacityLightPaths[i] < minCapacity)
							minCapacity = capacityLightPaths[i];
					}
					path.capacity = minCapacity; // capacidade do path é igual à capacidade do lightPath com menor capacidade
					path.numberOfLightPaths = lightPathsIndex.size();
					path.lightPathsIndex = lightPathsIndex;
					logicalTopology.paths.push_back(path);

					processedDemands.demandIndex = demand.demandIndex;
					processedDemands.routed = true;
					processedDemands.pathIndex = path.pathIndex;
					outputSignals[2]->bufferPut((t_demand_request_routed)processedDemands);
				}

				else if (lightPathsFound == false && lightPathsWithoutCapacity == false )
				{
					pathRequest.requestIndex = requestIndex;
					pathRequest.pathTotal = 1;
					pathRequest.demandIndex = demand.demandIndex;
					pathRequest.oduType = demand.oduType;
					pathRequest.sourceNode = myPath.front() + 1;
					pathRequest.destinationNode = myPath.back() + 1;
					for (size_t i = 1; i < myPath.size() - 1; i++)
					{
						pathRequest.numberOfIntermediateNodes++;	
						pathRequest.intermediateNodes.push_back(myPath[i] + 1);
					}
					outputSignals[0]->bufferPut((t_path_request)pathRequest);
					pathRequest.intermediateNodes.clear();
					requestIndex++;
					pathRequest.numberOfIntermediateNodes = 0;
				}

				else if (lightPathsFound == false && lightPathsWithoutCapacity == true)
				{
					for (size_t i = 0; i < lightPathsWithoutCapacityIndex.size(); i++)
					{
						pathRequest.pathTotal = 0;
						pathRequest.demandIndex = demand.demandIndex;
						pathRequest.demandSourceNode = demand.sourceNode;
						pathRequest.demandDestinationNode = demand.destinationNode;
						pathRequest.oduType = demand.oduType;
						pathRequest.sourceNode = logicalTopology.lightPaths[i].sourceNode;
						pathRequest.destinationNode = logicalTopology.lightPaths[i].destinationNode;

						outputSignals[0]->bufferPut((t_path_request)pathRequest);
					}
				}
			}	
		}
	}
	
	outputSignals[1]->bufferPut((t_logical_topology)logicalTopology);
	
	return true;
};

void LogicalTopologyManager::dijkstraComputePaths(vertex_t source, const adjacency_list_t &adjacency_list, std::vector<weight_t> &min_distance, std::vector<vertex_t> &previous)
{
	int n = adjacency_list.size();
	min_distance.clear();
	min_distance.resize(n, max_weight);
	min_distance[source] = 0;
	previous.clear();
	previous.resize(n, -1);
	std::set<std::pair<weight_t, vertex_t> > vertex_queue;
	vertex_queue.insert(std::make_pair(min_distance[source], source));

	while (!vertex_queue.empty())
	{
		weight_t dist = vertex_queue.begin()->first;
		vertex_t u = vertex_queue.begin()->second;
		vertex_queue.erase(vertex_queue.begin());

		// Visit each edge exiting u
		const std::vector<neighbor> &neighbors = adjacency_list[u];
		for (std::vector<neighbor>::const_iterator neighbor_iter = neighbors.begin(); neighbor_iter != neighbors.end(); neighbor_iter++)
		{
			vertex_t v = neighbor_iter->target;
			weight_t weight = neighbor_iter->weight;
			weight_t distance_through_u = dist + weight;
			if (distance_through_u < min_distance[v]) 
			{
				vertex_queue.erase(std::make_pair(min_distance[v], v));

				min_distance[v] = distance_through_u;
				previous[v] = u;
				vertex_queue.insert(std::make_pair(min_distance[v], v));
			}
		}
	}
}

std::list<vertex_t> LogicalTopologyManager::dijkstraGetShortestPathTo(vertex_t vertex, const std::vector<vertex_t> &previous)
{
	std::list<vertex_t> path;
	for (; vertex != -1; vertex = previous[vertex])
		path.push_front(vertex);
	return path;
}

