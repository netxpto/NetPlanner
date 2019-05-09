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
	t_path_routed pathRouted;
	t_light_paths_table lightPaths;

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

			std::vector<t_integer> available;
			std::vector<t_integer> OMSIndex;

			std::vector<t_integer> path;

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
							available.push_back(0);
							physicalTopology.physicalTopologyAdjacencyMatrix[src - 1][dst - 1] = 0;
						}

						if (physicalTopology.OMS[i].availableWavelengths.front() == 1)
						{
							available.push_back(1);
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
									available.push_back(1);
									OMSIndex.push_back(i);
									availableWavelenght = true;
								}
								k++;
							}
						}
					}
				}
			}

			t_integer multi = 1;
			for (size_t a = 0; a < available.size(); a++)
			{
				//std::cout << available[a] << "\n";
				multi *= available[a];
			}

			if (multi == 1)
			{
				for (size_t o = 0; o < OMSIndex.size(); o++)
				{
					if (physicalTopology.OMS[OMSIndex[o]].availableWavelengths[0] == 1)
					{
						physicalTopology.OMS[OMSIndex[o]].availableWavelengths[0] = 0;
						lightPaths.wavelength = (double)physicalTopology.OMS[OMSIndex[o]].wavelengths[0];
					}

					else if (physicalTopology.OMS[OMSIndex[o]].availableWavelengths[0] != 1)
					{
						t_integer k = 0;
						bool foundWavelenght = false;

						while ((physicalTopology.OMS[OMSIndex[o]].availableWavelengths[k] != 1) && (!foundWavelenght))
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

				pathRouted.routed = true;
				pathRouted.numberOfLightPaths = pathRequest.numberOfIntermediateNodes + 1;

			}

			else
			{
				//dijkstra(pathRequest.sourceNode,pathRequest.destinationNode)
				pathRouted.routed = false;
			}

			pathRouted.requestIndex = pathRequest.requestIndex;
			pathRequestRouted.pathRouted.push_back(pathRouted);
			outputSignals[0]->bufferPut((t_path_request_routed)pathRequestRouted);

			outputSignals[1]->bufferPut((t_physical_topology)physicalTopology);
		}
	}
		
	return true;
};
