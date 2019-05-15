#include "..\include_opaque\logical_topology_manager_20190419.h"

void LogicalTopologyManager::initialize(void) {}

bool LogicalTopologyManager::runBlock(void)
{
	t_integer processLogicalTopology = inputSignals[0]->ready();
	t_integer processDemands = inputSignals[1]->ready();
	t_integer processPathRequestRouted = inputSignals[2]->ready();

	if (processLogicalTopology == 0 && processDemands == 0 && processPathRequestRouted == 0) return false;
	
	if (processLogicalTopology != 0)
		inputSignals[0]->bufferGet(&logicalTopology);
	
	bool notProcessNextDemand = false;
	
	if (processPathRequestRouted != 0)
	{
		std::vector<t_light_paths_table> lightPathsTable;

		for (t_integer p = 0; p < processPathRequestRouted; p++)
		{
			t_path_request_routed pathRequestRouted;
			inputSignals[2]->bufferGet(&pathRequestRouted);
			
			t_demand_request_routed processedDemand;

			if (pathRequestRouted.pathInformation.routed == true)
			{
				t_integer odu0s;
				switch(demand.oduType)
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

				// se foi requerido wavelengths para todos os lightPaths que constituem o path
				//if (demand.sourceNode == pathRequestRouted.lightPathsTable[0].sourceNode &&
					//demand.destinationNode == pathRequestRouted.lightPathsTable[pathRequestRouted.lightPathsTable.size() - 1].destinationNode)
				if (pathRequestRouted.lightPathsTable.size() == pathDij.size() - 1)
				{
					std::vector<t_integer> newLightPathsIndex;

					for (size_t i = 0; i < pathRequestRouted.lightPathsTable.size(); i++)
					{
						t_optical_channel newOpticalChannel; // novo opticalChannel
						newOpticalChannel.opticalChannelIndex = logicalTopology.opticalChannels.size();
						newOpticalChannel.sourceNode = pathRequestRouted.lightPathsTable[i].sourceNode;
						newOpticalChannel.destinationNode = pathRequestRouted.lightPathsTable[i].destinationNode;
						newOpticalChannel.wavelength = pathRequestRouted.lightPathsTable[i].wavelength;
						newOpticalChannel.capacity = 80 - odu0s;
						newOpticalChannel.numberOfDemands = 1;
						newOpticalChannel.demandsIndex.push_back(demand.demandIndex);
						logicalTopology.opticalChannels.push_back(newOpticalChannel);
						newOpticalChannel.demandsIndex.clear();

						t_light_path newLightPath; // novo lightPath
						newLightPath.lightPathIndex = logicalTopology.lightPaths.size();
						newLightPath.sourceNode = pathRequestRouted.lightPathsTable[i].sourceNode;
						newLightPath.destinationNode = pathRequestRouted.lightPathsTable[i].destinationNode;
						newLightPath.capacity = 80 - odu0s;
						newLightPath.numberOfOpticalChannels = 1;
						newLightPath.opticalChannelsIndex.push_back(newOpticalChannel.opticalChannelIndex);
						logicalTopology.lightPaths.push_back(newLightPath);
						newLightPath.opticalChannelsIndex.clear();

						newLightPathsIndex.push_back(newLightPath.lightPathIndex);
					}
					t_path newPath; // novo path
					newPath.pathIndex = logicalTopology.paths.size();
					newPath.sourceNode = pathRequestRouted.lightPathsTable[0].sourceNode;
					newPath.destinationNode = pathRequestRouted.lightPathsTable[pathRequestRouted.lightPathsTable.size() - 1].destinationNode;
					newPath.capacity = 80 - odu0s;
					newPath.numberOfLightPaths = pathRequestRouted.pathInformation.numberOfLightPaths;
					newPath.lightPathsIndex = newLightPathsIndex;
					logicalTopology.paths.push_back(newPath);

					processedDemand.demandIndex = demand.demandIndex;
					processedDemand.routed = true;
					processedDemand.pathIndex = newPath.pathIndex;
					outputSignals[2]->bufferPut((t_demand_request_routed)processedDemand);

					temporaryLogicalMatrix = logicalTopology.logicalTopologyAdjacencyMatrix;

				}
	
				else // se foi requerido wavelengths só para alguns lightPaths que constituem o path
				{
					std::vector<t_integer> newLightPathsIndex;

					for (size_t i = 0; i < pathRequestRouted.lightPathsTable.size(); i++)
					{
						t_optical_channel newOpticalChannel; // novo opticalChannel
						newOpticalChannel.opticalChannelIndex = logicalTopology.opticalChannels.size();
						newOpticalChannel.sourceNode = pathRequestRouted.lightPathsTable[i].sourceNode;
						newOpticalChannel.destinationNode = pathRequestRouted.lightPathsTable[i].destinationNode;
						newOpticalChannel.wavelength = pathRequestRouted.lightPathsTable[i].wavelength;
						newOpticalChannel.capacity = 80;
						logicalTopology.opticalChannels.push_back(newOpticalChannel);

						t_light_path newLightPath; // novo lightPath
						newLightPath.lightPathIndex = newOpticalChannel.opticalChannelIndex;
						newLightPath.sourceNode = newOpticalChannel.sourceNode;
						newLightPath.destinationNode = newOpticalChannel.destinationNode;
						newLightPath.capacity = 80;
						newLightPath.numberOfOpticalChannels = 1;
						newLightPath.opticalChannelsIndex.push_back(newOpticalChannel.opticalChannelIndex);
						logicalTopology.lightPaths.push_back(newLightPath);
						newLightPath.opticalChannelsIndex.clear();

						newLightPathsIndex.push_back(newLightPath.lightPathIndex);
					}

					std::vector<t_integer> lightPathsIndex;
					std::vector<t_integer> capacityLightPaths;

					for (size_t i = 0; i < pathDij.size() - 1; i++)
					{
						t_integer src = pathDij[i];
						t_integer dst = pathDij[i + 1];

						bool lightPathWithCapacity = false;
						size_t j = 0;
						while (j < logicalTopology.lightPaths.size() && lightPathWithCapacity == false)
						{
							if (src == logicalTopology.lightPaths[j].sourceNode && dst == logicalTopology.lightPaths[j].destinationNode)
							{
								if (logicalTopology.lightPaths[j].capacity >= odu0s)
								{
									lightPathWithCapacity = true;
									// atualizar lightPaths e opticalChannels
									logicalTopology.lightPaths[j].capacity -= odu0s;
									logicalTopology.opticalChannels[j].capacity -= odu0s;
									logicalTopology.opticalChannels[j].numberOfDemands++;
									logicalTopology.opticalChannels[j].demandsIndex.push_back(demand.demandIndex);

									for (size_t k = 0; k < logicalTopology.paths.size(); k++)
									{
										for (size_t l = 0; l < logicalTopology.paths[k].lightPathsIndex.size(); l++)
										{
											if (logicalTopology.paths[k].lightPathsIndex[l] == i)
											{
												if (logicalTopology.paths[k].capacity > logicalTopology.lightPaths[j].capacity)
													logicalTopology.paths[k].capacity = logicalTopology.lightPaths[j].capacity;
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

					t_path newPath; // novo path
					newPath.pathIndex = logicalTopology.paths.size();
					newPath.sourceNode = demand.sourceNode;
					newPath.destinationNode = demand.destinationNode;
					t_integer minCapacity = capacityLightPaths[0];
					for (size_t i = 0; i < capacityLightPaths.size(); i++)
					{
						if (capacityLightPaths[i] < minCapacity)
							minCapacity = capacityLightPaths[i];
					}
					newPath.capacity = minCapacity;
					newPath.numberOfLightPaths = lightPathsIndex.size();
					newPath.lightPathsIndex = lightPathsIndex;
					logicalTopology.paths.push_back(newPath);

					// verfica se outros paths utilizam os lightPaths que foram atualizados, de forma a atualizar a sua capacidade 
					for (size_t i = 0; i < lightPathsIndex.size(); i++)
					{
						for (size_t j = 0; j < logicalTopology.paths.size(); j++)
						{
							for (size_t k = 0; k < logicalTopology.paths[j].lightPathsIndex.size(); k++)
							{
								if (logicalTopology.paths[j].lightPathsIndex[k] == lightPathsIndex[i])
								{
									if (logicalTopology.paths[j].capacity > logicalTopology.lightPaths[lightPathsIndex[i]].capacity)
											logicalTopology.paths[j].capacity = logicalTopology.lightPaths[lightPathsIndex[i]].capacity;
								}
							}
						}
					}
					processedDemand.demandIndex = demand.demandIndex;
					processedDemand.routed = true;
					processedDemand.pathIndex = newPath.pathIndex;
					outputSignals[2]->bufferPut((t_demand_request_routed)processedDemand);

					temporaryLogicalMatrix = logicalTopology.logicalTopologyAdjacencyMatrix;
				}	
			}
			
			else
			{
				tryAnotherPath++;
				if (tryAnotherPath == blockingCriterionLogicalTopology)
				{
					processedDemand.demandIndex = demand.demandIndex;
					processedDemand.routed = false;
					processedDemand.pathIndex = -1;
					outputSignals[2]->bufferPut((t_demand_request_routed)processedDemand);

					temporaryLogicalMatrix = logicalTopology.logicalTopologyAdjacencyMatrix;
				}
				else
				{
					notProcessNextDemand = true;
					for (size_t i = 0; i < pathRequestRouted.lightPathsTable.size(); i++)
					{
						t_integer src = pathRequestRouted.lightPathsTable[i].sourceNode;
						t_integer dst = pathRequestRouted.lightPathsTable[i].destinationNode;

						temporaryLogicalMatrix[src - 1][dst - 1] = 0;

						bool block = true;
						for (t_integer k = 0; k < logicalTopology.lightPaths.size(); k++)
						{
							if (src == logicalTopology.lightPaths[k].sourceNode && dst == logicalTopology.lightPaths[k].destinationNode)
							{
								if (logicalTopology.lightPaths[k].capacity != 0)
									block = false;
							}
						}
						if (block == true)
							logicalTopology.logicalTopologyAdjacencyMatrix[src - 1][dst - 1] = 0;	
					}
				}
			}
			pathDij.clear();
		}
	}
	
	t_demand_request_routed processedDemand;
	std::vector<t_integer> lightPathsIndex;

	if (processDemands != 0  && notProcessNextDemand == false)
	{
		for (t_integer dmd = 0; dmd < processDemands; dmd++)
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
			
			bool pathFound = false;
			size_t i = 0;
			while ((i < logicalTopology.paths.size()) && (!pathFound)) // verifica se existe um path com origem e destino iguais aos da demand
			{
				if (demand.sourceNode == logicalTopology.paths[i].sourceNode 
					&& demand.destinationNode == logicalTopology.paths[i].destinationNode) // se existir
				{
					if (odu0s <= logicalTopology.paths[i].capacity) // e com capacidade
					{
						// atualizar path
						logicalTopology.paths[i].capacity -= odu0s;

						lightPathsIndex = logicalTopology.paths[i].lightPathsIndex; // guarda os lightPaths, para os procurar nos outros paths 

						for (size_t j = 0; j < lightPathsIndex.size(); j++)
						{
							// atualizar lightPaths e opticalChannels
							logicalTopology.lightPaths[lightPathsIndex[j]].capacity -= odu0s;
							logicalTopology.opticalChannels[lightPathsIndex[j]].capacity -= odu0s;
							logicalTopology.opticalChannels[lightPathsIndex[j]].numberOfDemands++;
							logicalTopology.opticalChannels[lightPathsIndex[j]].demandsIndex.push_back(demand.demandIndex);
						}
						
						pathFound = true;
						processedDemand.demandIndex = demand.demandIndex;
						processedDemand.routed = true;
						processedDemand.pathIndex = logicalTopology.paths[i].pathIndex;
						outputSignals[2]->bufferPut((t_demand_request_routed)processedDemand);
					}
				}
				i++;
			}
			
			if (pathFound == true) // verfica se outros paths utilizam os lightPaths que foram atualizados, de forma a atualizar a sua capacidade 
			{
				for (size_t i = 0; i < lightPathsIndex.size(); i++)
				{
					for (size_t j = 0; j < logicalTopology.paths.size(); j++)
					{
						for (size_t k = 0; k < logicalTopology.paths[j].lightPathsIndex.size(); k++)
						{
							if (logicalTopology.paths[j].lightPathsIndex[k] == lightPathsIndex[i])
							{
								if (logicalTopology.paths[j].capacity > logicalTopology.lightPaths[lightPathsIndex[i]].capacity)
									logicalTopology.paths[j].capacity = logicalTopology.lightPaths[lightPathsIndex[i]].capacity;
							}
						}
					}
				}
			}
			
			if (pathFound == false) // corre o dijkstra que lhe dá os lightPaths 
			{
				adjacency_list_t adjacency_list(logicalTopology.logicalTopologyAdjacencyMatrix.size());

				for (size_t i = 0; i < logicalTopology.logicalTopologyAdjacencyMatrix.size(); i++)
				{
					for (size_t j = 0; j < logicalTopology.logicalTopologyAdjacencyMatrix.size(); j++)
					{
						if (logicalTopology.logicalTopologyAdjacencyMatrix[i][j] != 0)
						{
							if (routingCriterionLogicalTopology == "hops")
								adjacency_list[i].push_back(neighbor(j, 1));
						}
					}
				}
				std::vector<weight_t> min_distance;
				std::vector<vertex_t> previous;
				dijkstraComputePaths(demand.sourceNode - 1, adjacency_list, min_distance, previous);
				std::list<vertex_t> pathDijkstra = dijkstraGetShortestPathTo(demand.destinationNode - 1, previous);
				std::vector<vertex_t> myPath(pathDijkstra.begin(), pathDijkstra.end());

				if (myPath.size() - 1 == 0) // não existem caminhos 
				{
					processedDemand.demandIndex = demand.demandIndex;
					processedDemand.routed = false;
					processedDemand.pathIndex = -1;
					outputSignals[2]->bufferPut((t_demand_request_routed)processedDemand);
				}
				else
				{
					std::vector<t_integer> lightPathsWithCapacityIndex;

					for (size_t i = 0; i < myPath.size() - 1; i++)
					{
						t_integer src = myPath[i] + 1;
						t_integer dst = myPath[i + 1] + 1;

						pathDij.push_back(src);
						pathDij.push_back(dst);

						bool lightPathWithCapacity = false;
						size_t j = 0;
						while (j < logicalTopology.lightPaths.size() && lightPathWithCapacity == false) // verfica se já existem esses lightPaths
						{
							if (src == logicalTopology.lightPaths[j].sourceNode && dst == logicalTopology.lightPaths[j].destinationNode) // existem
							{
								if (logicalTopology.lightPaths[j].capacity >= odu0s) // com capacidade
								{
									lightPathsWithCapacityIndex.push_back(j);
									lightPathWithCapacity = true; // já encontrou o lightPath com capacidade, sai do ciclo 
								}
							}
							j++;
						}
					}

					if (lightPathsWithCapacityIndex.size() == myPath.size() - 1) // encontrou todos os lightPaths e com capacidade 
					{
						pathDij.clear();
						std::vector<t_integer> capacityLightPaths;

						for (size_t i = 0; i < lightPathsWithCapacityIndex.size(); i++)
						{
							// atualizar lightPaths e opticalChannels
							logicalTopology.lightPaths[lightPathsWithCapacityIndex[i]].capacity -= odu0s;
							logicalTopology.opticalChannels[lightPathsWithCapacityIndex[i]].capacity -= odu0s;
							logicalTopology.opticalChannels[lightPathsWithCapacityIndex[i]].numberOfDemands++;
							logicalTopology.opticalChannels[lightPathsWithCapacityIndex[i]].demandsIndex.push_back(demand.demandIndex);

							//verificar se existem outros paths a usar estes lightPaths e atualizar a capacidade do path com o menor valor 
							for (size_t j = 0; j < logicalTopology.paths.size(); j++)
							{
								for (size_t k = 0; k < logicalTopology.paths[j].lightPathsIndex.size(); k++)
								{
									if (logicalTopology.paths[j].lightPathsIndex[k] == lightPathsWithCapacityIndex[i])
									{
										if (logicalTopology.paths[j].capacity > logicalTopology.lightPaths[lightPathsWithCapacityIndex[i]].capacity)
											logicalTopology.paths[j].capacity = logicalTopology.lightPaths[lightPathsWithCapacityIndex[i]].capacity;
									}
								}
							}

							capacityLightPaths.push_back(logicalTopology.lightPaths[lightPathsWithCapacityIndex[i]].capacity);

						}
						t_path newPath; // criar novo path
						newPath.pathIndex = logicalTopology.paths.size();
						newPath.sourceNode = demand.sourceNode;
						newPath.destinationNode = demand.destinationNode;
						t_integer minCapacity = capacityLightPaths[0];
						for (size_t i = 0; i < capacityLightPaths.size(); i++)
						{
							if (capacityLightPaths[i] < minCapacity)
								minCapacity = capacityLightPaths[i];
						}
						newPath.capacity = minCapacity; // capacidade do path é igual à capacidade do lightPath com menor capacidade
						newPath.numberOfLightPaths = lightPathsWithCapacityIndex.size();
						newPath.lightPathsIndex = lightPathsWithCapacityIndex;
						logicalTopology.paths.push_back(newPath);

						processedDemand.demandIndex = demand.demandIndex;
						processedDemand.routed = true;
						processedDemand.pathIndex = newPath.pathIndex;
						outputSignals[2]->bufferPut((t_demand_request_routed)processedDemand);

						temporaryLogicalMatrix = logicalTopology.logicalTopologyAdjacencyMatrix;
					}

					else if (lightPathsWithCapacityIndex.size() == 0) // não encontrou qualquer lightPath com capacidade
					{
						t_path_request pathRequest;
						pathRequest.requestIndex = requestIndex;
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

					else //encontrou alguns lightPaths com capacidade e outros ou sem capacidade ou não existem  
					{
						//quais os lightPaths com capacidade e quais os sem capacidade?
						for (size_t i = 0; i < myPath.size() - 1; i++)
						{
							t_integer src = myPath[i] + 1;
							t_integer dst = myPath[i + 1] + 1;

							bool foundLightPathsWithCapacity = false;
							for (size_t j = 0; j < lightPathsWithCapacityIndex.size(); j++)
							{
								if (src == logicalTopology.lightPaths[lightPathsWithCapacityIndex[j]].sourceNode
									&& dst == logicalTopology.lightPaths[lightPathsWithCapacityIndex[j]].destinationNode)
									foundLightPathsWithCapacity = true;
							}

							if (foundLightPathsWithCapacity == false) // lightPaths sem capacidade
							{
								t_path_request pathRequest;
								pathRequest.requestIndex = requestIndex;
								pathRequest.sourceNode = src;
								pathRequest.destinationNode = dst;
								pathRequest.numberOfIntermediateNodes = 0;
								requestIndex++;

								outputSignals[0]->bufferPut((t_path_request)pathRequest);
							}
						}
					}
				}
			}
			lightPathsIndex.clear();
		}
	}

	else if (notProcessNextDemand == true)
	{
		adjacency_list_t adjacency_list(temporaryLogicalMatrix.size());

		for (size_t i = 0; i < temporaryLogicalMatrix.size(); i++)
		{
			for (size_t j = 0; j < temporaryLogicalMatrix.size(); j++)
			{
				if (temporaryLogicalMatrix[i][j] != 0)
				{
					if (routingCriterionLogicalTopology == "hops")
						adjacency_list[i].push_back(neighbor(j, 1));
				}
			}
		}
		std::vector<weight_t> min_distance;
		std::vector<vertex_t> previous;
		dijkstraComputePaths(demand.sourceNode - 1, adjacency_list, min_distance, previous);
		std::list<vertex_t> pathDijkstra = dijkstraGetShortestPathTo(demand.destinationNode - 1, previous);
		std::vector<vertex_t> myPath(pathDijkstra.begin(), pathDijkstra.end());

		if (myPath.size() - 1 == 0) // não existem caminhos 
		{
			processedDemand.demandIndex = demand.demandIndex;
			processedDemand.routed = false;
			processedDemand.pathIndex = -1;
			outputSignals[2]->bufferPut((t_demand_request_routed)processedDemand);
		}
		else
		{
			std::vector<t_integer> lightPathsWithCapacityIndex;

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

			for (size_t i = 0; i < myPath.size() - 1; i++)
			{
				t_integer src = myPath[i] + 1;
				t_integer dst = myPath[i + 1] + 1;

				pathDij.push_back(src);
				pathDij.push_back(dst);

				bool lightPathWithCapacity = false;
				size_t j = 0;
				while (j < logicalTopology.lightPaths.size() && lightPathWithCapacity == false) // verfica se já existem esses lightPaths
				{
					if (src == logicalTopology.lightPaths[j].sourceNode && dst == logicalTopology.lightPaths[j].destinationNode) // existem
					{
						if (logicalTopology.lightPaths[j].capacity >= odu0s) // com capacidade
						{
							lightPathsWithCapacityIndex.push_back(j);
							lightPathWithCapacity = true; // já encontrou o lightPath com capacidade, sai do ciclo 
						}
					}
					j++;
				}
			}

			if (lightPathsWithCapacityIndex.size() == myPath.size() - 1) // encontrou todos os lightPaths e com capacidade 
			{
				pathDij.clear();
				std::vector<t_integer> capacityLightPaths;

				for (size_t i = 0; i < lightPathsWithCapacityIndex.size(); i++)
				{
					// atualizar lightPaths e opticalChannels
					logicalTopology.lightPaths[lightPathsWithCapacityIndex[i]].capacity -= odu0s;
					logicalTopology.opticalChannels[lightPathsWithCapacityIndex[i]].capacity -= odu0s;
					logicalTopology.opticalChannels[lightPathsWithCapacityIndex[i]].numberOfDemands++;
					logicalTopology.opticalChannels[lightPathsWithCapacityIndex[i]].demandsIndex.push_back(demand.demandIndex);

					//verificar se existem outros paths a usar estes lightPaths e atualizar a capacidade do path com o menor valor 
					for (size_t j = 0; j < logicalTopology.paths.size(); j++)
					{
						for (size_t k = 0; k < logicalTopology.paths[j].lightPathsIndex.size(); k++)
						{
							if (logicalTopology.paths[j].lightPathsIndex[k] == lightPathsWithCapacityIndex[i])
							{
								if (logicalTopology.paths[j].capacity > logicalTopology.lightPaths[lightPathsWithCapacityIndex[i]].capacity)
									logicalTopology.paths[j].capacity = logicalTopology.lightPaths[lightPathsWithCapacityIndex[i]].capacity;
							}
						}
					}

					capacityLightPaths.push_back(logicalTopology.lightPaths[lightPathsWithCapacityIndex[i]].capacity);

				}
				t_path newPath; // criar novo path
				newPath.pathIndex = logicalTopology.paths.size();
				newPath.sourceNode = demand.sourceNode;
				newPath.destinationNode = demand.destinationNode;
				t_integer minCapacity = capacityLightPaths[0];
				for (size_t i = 0; i < capacityLightPaths.size(); i++)
				{
					if (capacityLightPaths[i] < minCapacity)
						minCapacity = capacityLightPaths[i];
				}
				newPath.capacity = minCapacity; // capacidade do path é igual à capacidade do lightPath com menor capacidade
				newPath.numberOfLightPaths = lightPathsWithCapacityIndex.size();
				newPath.lightPathsIndex = lightPathsWithCapacityIndex;
				logicalTopology.paths.push_back(newPath);

				processedDemand.demandIndex = demand.demandIndex;
				processedDemand.routed = true;
				processedDemand.pathIndex = newPath.pathIndex;
				outputSignals[2]->bufferPut((t_demand_request_routed)processedDemand);
			}

			else if (lightPathsWithCapacityIndex.size() == 0) // não encontrou qualquer lightPath com capacidade
			{
				t_path_request pathRequest;
				pathRequest.requestIndex = requestIndex;
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

			else //encontrou alguns lightPaths com capacidade e outros ou sem capacidade ou não existem  
			{
				//quais os lightPaths com capacidade e quais os sem capacidade?
				for (size_t i = 0; i < myPath.size() - 1; i++)
				{
					t_integer src = myPath[i] + 1;
					t_integer dst = myPath[i + 1] + 1;

					bool foundLightPathsWithCapacity = false;
					for (size_t j = 0; j < lightPathsWithCapacityIndex.size(); j++)
					{
						if (src == logicalTopology.lightPaths[lightPathsWithCapacityIndex[j]].sourceNode
							&& dst == logicalTopology.lightPaths[lightPathsWithCapacityIndex[j]].destinationNode)
							foundLightPathsWithCapacity = true;
					}

					if (foundLightPathsWithCapacity == false) // sem capacidade
					{
						t_path_request pathRequest;
						pathRequest.requestIndex = requestIndex;
						pathRequest.sourceNode = src;
						pathRequest.destinationNode = dst;
						pathRequest.numberOfIntermediateNodes = 0;
						requestIndex++;

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

