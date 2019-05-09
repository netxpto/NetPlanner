#include "..\include_opaque\physical_topology_manager_20190420.h"

void PhysicalTopologyManager::initialize(void) 
{

}

bool PhysicalTopologyManager::runBlock(void) 
{
	t_integer readyPhysicalTopology = inputSignals[0]->ready();
	t_integer readyPathRequest = inputSignals[1]->ready();

	if (readyPhysicalTopology == 0 && readyPathRequest == 0) return false;

	t_path_request pathRequest;
	t_path_request_routed pathRequestRouted;

	if (readyPhysicalTopology != 0)
	{
		for (t_integer i = 0; i < readyPhysicalTopology; i++)
			inputSignals[0]->bufferGet(&physicalTopology);
	}
		
	if (readyPathRequest != 0)
	{
		for (t_integer r = 0; r < readyPathRequest; r++)
		{
			inputSignals[1]->bufferGet(&pathRequest);

			//std::vector<t_integer> available;
			bool wavelengthCapacity = true;
			std::vector<t_integer> OMSIndex;

			std::vector<t_integer> path;

			//std::cout << pathRequest.sourceNode;
			//std::cout << pathRequest.destinationNode;

			path.push_back(pathRequest.sourceNode);
			if (pathRequest.numberOfIntermediateNodes >= 1) 
			{
				for (size_t i = 0; i < pathRequest.intermediateNodes.size(); i++)
				{
					path.push_back(pathRequest.intermediateNodes[i]);
				}
			}
			path.push_back(pathRequest.destinationNode);

			for (size_t j = 0; j < path.size() - 1; j++)
			{
				t_integer src = path[j];
				t_integer dst = path[j + 1];

				//std::cout << src << "\n";
				//std::cout << dst << "\n";

				for (size_t i = 0; i < physicalTopology.OMS.size(); i++)
				{
					if (src == physicalTopology.OMS[i].sourceNode && dst == physicalTopology.OMS[i].destinationNode)
					{
						if (physicalTopology.OMS[i].availableWavelengths.back() == 0) //não tem capacidade, atualizar matriz física
						{
							wavelengthCapacity = false;
							//available.push_back(0);
							physicalTopology.physicalTopologyAdjacencyMatrix[src - 1][dst - 1] = 0;
							break;
						}

						if (physicalTopology.OMS[i].availableWavelengths.front() == 1)
						{
							//available.push_back(1);
							OMSIndex.push_back(i);
						}

						else if ((physicalTopology.OMS[i].availableWavelengths.front() != 1) && (physicalTopology.OMS[i].availableWavelengths.back() == 1))
						{
							bool availableWavelenght{ false };
							t_integer k{ 0 };
							while ((physicalTopology.OMS[i].availableWavelengths[k] != 1) && (!availableWavelenght))
							{
								if (physicalTopology.OMS[i].availableWavelengths[k + 1] == 1)
								{
									//available.push_back(1);
									OMSIndex.push_back(i);
									availableWavelenght = true;
								}
								k++;
							}
						}
					}
				}
			}
			/*
			t_integer multi = 1;
			for (size_t a = 0; a < available.size(); a++)
			{
				//std::cout << available[a] << "\n";
				multi *= available[a];
			}
			*/
			if (wavelengthCapacity == true)
			{
				for (size_t o = 0; o < OMSIndex.size(); o++)
				{
					t_light_paths_table lightPaths;

					//std::cout << OMSIndex[o];
					if (physicalTopology.OMS[OMSIndex[o]].availableWavelengths[0] == 1)
					{
						physicalTopology.OMS[OMSIndex[o]].availableWavelengths[0] = 0;
						lightPaths.wavelength = (double)physicalTopology.OMS[OMSIndex[o]].wavelengths[0];
					}

					else if (physicalTopology.OMS[OMSIndex[o]].availableWavelengths[0] != 1)
					{
						t_integer k = 0;
						bool foundWavelenght = false;

						while (physicalTopology.OMS[OMSIndex[o]].availableWavelengths[k] != 1 && foundWavelenght == false)
						{
							if (physicalTopology.OMS[OMSIndex[o]].availableWavelengths[k + 1] == 1)
							{
								physicalTopology.OMS[OMSIndex[o]].availableWavelengths[k + 1] = 0;
								lightPaths.wavelength = (double)physicalTopology.OMS[OMSIndex[o]].wavelengths[k + 1];

								foundWavelenght = true;
							}
							k++;
						}
					}

					lightPaths.sourceNode = physicalTopology.OMS[OMSIndex[o]].sourceNode;
					lightPaths.destinationNode = physicalTopology.OMS[OMSIndex[o]].destinationNode;
					lightPaths.numberOfIntermediateNodes = 0;

					pathRequestRouted.lightPathsTable.push_back(lightPaths);
				}

				OMSIndex.clear();
				//available.clear();
				//path.clear();

				
				pathRequestRouted.pathInformation.routed = true;
				pathRequestRouted.pathInformation.numberOfLightPaths = pathRequest.numberOfIntermediateNodes + 1;

			}

			else
			{
				adjacency_list_t adjacency_list(physicalTopology.physicalTopologyAdjacencyMatrix.size());

				for (size_t i = 0; i < physicalTopology.physicalTopologyAdjacencyMatrix.size(); i++)
				{
					for (size_t j = 0; j < physicalTopology.physicalTopologyAdjacencyMatrix.size(); j++)
					{
						if (physicalTopology.physicalTopologyAdjacencyMatrix[i][j] != 0)
						{
							if (routingCriterionPhysicalTopology == "hops")
								adjacency_list[i].push_back(neighbor(j, 1));

							//else if (routingCriterionPhysicalTopology == "km")
								//matriz de distâncias
						}
					}
				}
				std::vector<weight_t> min_distance;
				std::vector<vertex_t> previous;
				dijkstraComputePaths(pathRequest.sourceNode - 1, adjacency_list, min_distance, previous);
				std::list<vertex_t> pathDijkstra = dijkstraGetShortestPathTo(pathRequest.destinationNode - 1, previous);
				std::vector<vertex_t> myPath(pathDijkstra.begin(), pathDijkstra.end());

				bool wavelengthCap = true;
				std::vector<t_integer> OMSIndexs;

				for (size_t i = 0; i < myPath.size() - 1; i++)
				{
					t_integer src = myPath[i] + 1;
					t_integer dst = myPath[i + 1] + 1;

					//std::cout << src << endl;
					//std::cout << dst << endl;

					for (size_t i = 0; i < physicalTopology.OMS.size(); i++)
					{
						if (src == physicalTopology.OMS[i].sourceNode && dst == physicalTopology.OMS[i].destinationNode)
						{
							if (physicalTopology.OMS[i].availableWavelengths.back() == 0) //não tem capacidade, atualizar matriz física
							{
								wavelengthCap = false;
								//available.push_back(0);
								physicalTopology.physicalTopologyAdjacencyMatrix[src - 1][dst - 1] = 0;
								break;
							}

							if (physicalTopology.OMS[i].availableWavelengths.front() == 1)
							{
								//available.push_back(1);
								OMSIndexs.push_back(i);
							}

							else if ((physicalTopology.OMS[i].availableWavelengths.front() != 1) && (physicalTopology.OMS[i].availableWavelengths.back() == 1))
							{
								bool availableWavelenght{ false };
								t_integer k{ 0 };
								while ((physicalTopology.OMS[i].availableWavelengths[k] != 1) && (!availableWavelenght))
								{
									if (physicalTopology.OMS[i].availableWavelengths[k + 1] == 1)
									{
										//available.push_back(1);
										OMSIndexs.push_back(i);
										availableWavelenght = true;
									}
									k++;
								}
							}
						}
					}
					//pathRequestRouted.pathInformation.routed = false;
					//pathRequestRouted.pathInformation.numberOfLightPaths = -1;
				}

				if (wavelengthCap == true)
				{
					for (size_t o = 0; o < OMSIndexs.size(); o++)
					{
						t_light_paths_table lightPaths;

						//std::cout << OMSIndexs[o];
						if (physicalTopology.OMS[OMSIndexs[o]].availableWavelengths[0] == 1)
						{
							physicalTopology.OMS[OMSIndexs[o]].availableWavelengths[0] = 0;
							lightPaths.wavelength = (double)physicalTopology.OMS[OMSIndexs[o]].wavelengths[0];
						}

						else if (physicalTopology.OMS[OMSIndexs[o]].availableWavelengths[0] != 1)
						{
							t_integer k = 0;
							bool foundWavelenght = false;

							while (physicalTopology.OMS[OMSIndexs[o]].availableWavelengths[k] != 1 && foundWavelenght == false)
							{
								if (physicalTopology.OMS[OMSIndexs[o]].availableWavelengths[k + 1] == 1)
								{
									physicalTopology.OMS[OMSIndexs[o]].availableWavelengths[k + 1] = 0;
									lightPaths.wavelength = (double)physicalTopology.OMS[OMSIndexs[o]].wavelengths[k + 1];

									foundWavelenght = true;
								}
								k++;
							}
						}

						lightPaths.sourceNode = physicalTopology.OMS[OMSIndexs[o]].sourceNode;
						lightPaths.destinationNode = physicalTopology.OMS[OMSIndexs[o]].destinationNode;
						lightPaths.numberOfIntermediateNodes = 0;

						pathRequestRouted.lightPathsTable.push_back(lightPaths);
					}
					OMSIndexs.clear();
					//available.clear();
					//path.clear();


					pathRequestRouted.pathInformation.routed = true;
					pathRequestRouted.pathInformation.numberOfLightPaths = myPath.size() - 1;
				}
			}

			pathRequestRouted.pathInformation.pathTotal = pathRequest.pathTotal;
			pathRequestRouted.pathInformation.requestIndex = pathRequest.requestIndex;
			pathRequestRouted.pathInformation.demandIndex = pathRequest.demandIndex;
			pathRequestRouted.pathInformation.demandSourceNode = pathRequest.demandSourceNode;
			pathRequestRouted.pathInformation.demandDestinationNode = pathRequest.demandDestinationNode;
			pathRequestRouted.pathInformation.oduType = pathRequest.oduType;

			outputSignals[0]->bufferPut((t_path_request_routed)pathRequestRouted);

			pathRequestRouted.lightPathsTable.clear();

			outputSignals[1]->bufferPut((t_physical_topology)physicalTopology);
		}
	}
		
	return true;
};

void PhysicalTopologyManager::dijkstraComputePaths(vertex_t source, const adjacency_list_t &adjacency_list, std::vector<weight_t> &min_distance, std::vector<vertex_t> &previous)
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

std::list<vertex_t> PhysicalTopologyManager::dijkstraGetShortestPathTo(vertex_t vertex, const std::vector<vertex_t> &previous)
{
	std::list<vertex_t> path;
	for (; vertex != -1; vertex = previous[vertex])
		path.push_front(vertex);
	return path;
}

