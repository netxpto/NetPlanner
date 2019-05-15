#include "..\include_opaque\physical_topology_manager_20190420.h"

void PhysicalTopologyManager::initialize(void) {}

bool PhysicalTopologyManager::runBlock(void) 
{
	t_integer processPhysicalTopology = inputSignals[0]->ready();
	t_integer processPathRequest = inputSignals[1]->ready();

	if (processPhysicalTopology == 0 && processPathRequest == 0) return false;

	if (processPhysicalTopology != 0)
		inputSignals[0]->bufferGet(&physicalTopology);
	
	t_path_request pathRequest;
	t_path_request_routed pathRequestRouted;
		
	std::vector<t_path_request> pathRequests;

	bool wavelengthAvailable = true;
	std::vector<t_integer> OMSIndex;

	if (processPathRequest > 1)
	{
		for (t_integer p = 0; p < processPathRequest; p++)
		{
			inputSignals[1]->bufferGet(&pathRequest);

			pathRequests.push_back(pathRequest);
		}
	}

	if (processPathRequest == 1)
	{	
		inputSignals[1]->bufferGet(&pathRequest);

		std::vector<t_integer> myPath;

		myPath.push_back(pathRequest.sourceNode);
		if (pathRequest.numberOfIntermediateNodes >= 1)
		{
			for (size_t i = 0; i < pathRequest.intermediateNodes.size(); i++)
			{
				myPath.push_back(pathRequest.intermediateNodes[i]);
			}
		}
		myPath.push_back(pathRequest.destinationNode);

		for (size_t i = 0; i < myPath.size() - 1; i++)
		{
			t_integer src = myPath[i];
			t_integer dst = myPath[i + 1];

			for (size_t j = 0; j < physicalTopology.OMS.size(); j++) // verifica se tem wavelengths disponíveis
			{
				if (src == physicalTopology.OMS[j].sourceNode && dst == physicalTopology.OMS[j].destinationNode)
				{
					if (physicalTopology.OMS[j].availableWavelengths.back() == 0) //não tem wavelength disponível
					{
						wavelengthAvailable = false;

						t_light_paths_table lightPathsTable;

						lightPathsTable.sourceNode = src;
						lightPathsTable.destinationNode = dst;
						lightPathsTable.wavelength = -1;
						pathRequestRouted.lightPathsTable.push_back(lightPathsTable);		
					}

					if (physicalTopology.OMS[j].availableWavelengths.front() == 1)
						OMSIndex.push_back(j);

					else if ((physicalTopology.OMS[j].availableWavelengths.front() != 1) && (physicalTopology.OMS[j].availableWavelengths.back() == 1))
					{
						bool availableWavelenght = false;
						t_integer k = 0;
						while ((physicalTopology.OMS[j].availableWavelengths[k] != 1) && (!availableWavelenght))
						{
							if (physicalTopology.OMS[j].availableWavelengths[k + 1] == 1)
							{
								OMSIndex.push_back(j);
								availableWavelenght = true;
							}
							k++;
						}
					}
				}
			}
		}
	}

	else
	{
		for (size_t i = 0; i < pathRequests.size(); i++)
		{
			t_integer src = pathRequests[i].sourceNode;
			t_integer dst = pathRequests[i].destinationNode;

			for (size_t j = 0; j < physicalTopology.OMS.size(); j++) // verifica se tem wavelengths disponíveis
			{
				if (src == physicalTopology.OMS[j].sourceNode && dst == physicalTopology.OMS[j].destinationNode)
				{
					if (physicalTopology.OMS[j].availableWavelengths.back() == 0) //não tem wavelength disponível
					{
						wavelengthAvailable = false;

						t_light_paths_table lightPathsTable;

						lightPathsTable.sourceNode = src;
						lightPathsTable.destinationNode = dst;
						lightPathsTable.wavelength = -1;
						pathRequestRouted.lightPathsTable.push_back(lightPathsTable);
					}

					if (physicalTopology.OMS[j].availableWavelengths.front() == 1)
						OMSIndex.push_back(j);

					else if ((physicalTopology.OMS[j].availableWavelengths.front() != 1) && (physicalTopology.OMS[j].availableWavelengths.back() == 1))
					{
						bool availableWavelenght = false;
						t_integer k = 0;
						while ((physicalTopology.OMS[j].availableWavelengths[k] != 1) && (!availableWavelenght))
						{
							if (physicalTopology.OMS[j].availableWavelengths[k + 1] == 1)
							{
								OMSIndex.push_back(j);
								availableWavelenght = true;
							}
							k++;
						}
					}
				}
			}
		}
	}

	if (wavelengthAvailable == true) // wavelengths disponíveis, atualizar OMS
	{
		for (size_t i = 0; i < OMSIndex.size(); i++)
		{
			t_light_paths_table lightPathsTable;

			if (physicalTopology.OMS[OMSIndex[i]].availableWavelengths[0] == 1)
			{
				physicalTopology.OMS[OMSIndex[i]].availableWavelengths[0] = 0;
				lightPathsTable.wavelength = (double)physicalTopology.OMS[OMSIndex[i]].wavelengths[0];
			}

			else if (physicalTopology.OMS[OMSIndex[i]].availableWavelengths[0] != 1)
			{
				t_integer k = 0;
				bool foundWavelenght = false;

				while (physicalTopology.OMS[OMSIndex[i]].availableWavelengths[k] != 1 && foundWavelenght == false)
				{
					if (physicalTopology.OMS[OMSIndex[i]].availableWavelengths[k + 1] == 1)
					{
						physicalTopology.OMS[OMSIndex[i]].availableWavelengths[k + 1] = 0;
						lightPathsTable.wavelength = (double)physicalTopology.OMS[OMSIndex[i]].wavelengths[k + 1];

						foundWavelenght = true;
					}
					k++;
				}
			}
			lightPathsTable.sourceNode = physicalTopology.OMS[OMSIndex[i]].sourceNode;
			lightPathsTable.destinationNode = physicalTopology.OMS[OMSIndex[i]].destinationNode;
			lightPathsTable.numberOfIntermediateNodes = 0;

			pathRequestRouted.lightPathsTable.push_back(lightPathsTable);
		}
		pathRequestRouted.pathInformation.routed = true;
	}

	else
	{
		pathRequestRouted.pathInformation.routed = false;
		pathRequestRouted.pathInformation.numberOfLightPaths = pathRequestRouted.lightPathsTable.size();
	}

	if (processPathRequest == 1)
	{
		pathRequestRouted.pathInformation.requestIndex = pathRequest.requestIndex;
		pathRequestRouted.pathInformation.numberOfLightPaths = pathRequest.numberOfIntermediateNodes + 1;	
	}
		
	else
	{
		//pathReuquestRouted.pathInformation.requestIndex
		pathRequestRouted.pathInformation.numberOfLightPaths = pathRequests.size();
	}
		
	outputSignals[0]->bufferPut((t_path_request_routed)pathRequestRouted);
	outputSignals[1]->bufferPut((t_physical_topology)physicalTopology);
		
	return true;
};


