# include "..\include\physical_topology_manager_20190421.h"
# include "..\include\Dijkstra_physical_20190504.h"
# include "..\include\Dijkstra_20190309.h"


//using namespace std;    // (commented by Romil 08/05/2020)


void PhysicalTopologyManager::initialize(void) {

}

bool PhysicalTopologyManager::runBlock(void) {

	int process = inputSignals[0]->ready(); // number of samples in the buffer ready to be processed
	int process2 = inputSignals[1]->ready(); // number of PathRequest signals in the buffer ready to be processed
	t_path_request newPathRequest;

	if (process == 0 && process2 == 0) return false;

	if (process != 0)
	{
		for (int i = 0; i < process; i++) {
			inputSignals[0]->bufferGet(&currentPhysicalTopology);
		}
		outputSignals[0]->bufferPut((t_physical_topology)currentPhysicalTopology);
	}

	if (process2 != 0 && currentPhysicalTopology.transportMode == transport_mode::transparent) // there are PathRequests in the buffer
	{
		for (int i = 0; i < process2; i++) {
			inputSignals[1]->bufferGet(&newPathRequest); // reads a PathRequest signal
			
			vector<int> intermediateNodes;
			// Generates shortest physical paths (Dijkstra algorithm)

			Graph g(currentPhysicalTopology.physicalTopologyAdjacencyMatrix[0].size());

			for (int line = 0; line < currentPhysicalTopology.physicalTopologyAdjacencyMatrix[0].size(); line++)
			{
				for (int column = 0; column < currentPhysicalTopology.physicalTopologyAdjacencyMatrix[0].size(); column++)
				{
					if (currentPhysicalTopology.physicalTopologyAdjacencyMatrix[line][column] == 1)
						g.addEdge(line, column);
				}
			}

			int sourceNode = newPathRequest.sourceNode-1;
			int destinationNode = newPathRequest.destinationNode-1;
			
			g.printAllPaths(sourceNode, destinationNode);
			std::vector<vector<int>> possiblePaths = g.printFinalPaths(blockingCriterionPhysicalTopology);

			std::vector<std::vector<double>> availableWavelenghtsIndexOfAllOMS; // informaçao acerca de 1 só caminho!!!!
			// Search through the vector that contains the shortest paths

			int pathsLine{ 0 }; 
			bool pathAvailable{ false };

			while (pathsLine < blockingCriterionPhysicalTopology && pathAvailable == false)
			{
				// Read and test one path
				
					for (int j = 0; j < possiblePaths[pathsLine].size() - 1; j++)
					{
						double k{ 0 };
						bool omsFound{ false };
						while (k < currentPhysicalTopology.opticalMultiplexingSystems.size() && omsFound == false)
						{
							// Find an OMS with the same source and destination nodes
							if ((currentPhysicalTopology.opticalMultiplexingSystems[k].sourceNode == (possiblePaths[pathsLine][j] + 1) && currentPhysicalTopology.opticalMultiplexingSystems[k].destinationNode == (possiblePaths[pathsLine][j + 1] + 1)))
							{
								if (possiblePaths[pathsLine].size() > 2 && j > 0)
									intermediateNodes.push_back(possiblePaths[pathsLine][j] + 1);

								std::vector<double> OMSInformation;
								omsFound = true;

								OMSInformation.push_back(k); // first element of each vector identifies the OMS analyzed through its OMS index

								for (int l = 0; l < currentPhysicalTopology.opticalMultiplexingSystems[k].availableWavelengths.size(); l++)
								{
									if (currentPhysicalTopology.opticalMultiplexingSystems[k].availableWavelengths[l] == 1)
										OMSInformation.push_back(currentPhysicalTopology.opticalMultiplexingSystems[k].wavelengths[l]);
								}
								availableWavelenghtsIndexOfAllOMS.push_back(OMSInformation);
								OMSInformation.clear();
							}
							k++;
						}
					} //Aqui já temos os oms que fazem parte de um caminho (o index e os comps. de onda disponiveis neles)
				
				// Test the OMS needed to create the possible first path
				// Analyze which wavelenghts are available in each OMS
				double wavelenghtSelected{ 0 };
				bool wavelenghtFound{ false };
				double wavelenghtTested{ 0 };
				bool test{ false };
				int x{ 1 };
				vector<bool> testResults;
				bool tryNextPath{ false };
				
				while (wavelenghtFound == false && x < availableWavelenghtsIndexOfAllOMS[0].size()) // poderá ter de correr todas as wavelenghts do primeiro oms até encontrar uma compativel a todos
				{
					tryNextPath = false;
					wavelenghtTested = availableWavelenghtsIndexOfAllOMS[0][x];
					//Test that wavelenght in the reamaining OMS of the path

					if (availableWavelenghtsIndexOfAllOMS.size() == 1) // for paths with only one oms
						tryNextPath = false;

					for (size_t t = 1; t < availableWavelenghtsIndexOfAllOMS.size(); t++)
					{
						int y{ 1 };
						test = false;

						while (test == false && y < availableWavelenghtsIndexOfAllOMS[t].size())
						{
							if (availableWavelenghtsIndexOfAllOMS[t][y] == wavelenghtTested)
								test = true;
							else
								y++;
						}
						testResults.push_back(test);
					}
					for (int z = 0; z < testResults.size(); z++)
					{
						if (testResults[z] == false)
							tryNextPath = true;
					}
					if (tryNextPath == false)
					{
						wavelenghtFound = true;
						wavelenghtSelected = wavelenghtTested;

					}
					testResults.clear();
					x++;
				}
			
				if (wavelenghtFound == true)
				{
					pathAvailable = true;
				
					t_path_request_routed pathRequestRouted;
					// Update PhysicalTopologyManager_ block state variables
					for (int i = 0; i < availableWavelenghtsIndexOfAllOMS.size(); i++)
					{
						for (int j = 0; j < currentPhysicalTopology.opticalMultiplexingSystems[availableWavelenghtsIndexOfAllOMS[i][0]].wavelengths.size(); j++)
						{
							if (wavelenghtSelected == currentPhysicalTopology.opticalMultiplexingSystems[availableWavelenghtsIndexOfAllOMS[i][0]].wavelengths[j])
								currentPhysicalTopology.opticalMultiplexingSystems[availableWavelenghtsIndexOfAllOMS[i][0]].availableWavelengths[j] = 0;
						}
					}

					t_physical_topology physicalTopologyIllustrator;
					physicalTopologyIllustrator = currentPhysicalTopology;
					

					for (int s = 0; s < currentPhysicalTopology.opticalMultiplexingSystems.size(); s++)
					{
						bool omsFull{ true };
						for (int f = 0; f < currentPhysicalTopology.opticalMultiplexingSystems[s].numberOfWavelenghts; f++)
						{
							if (currentPhysicalTopology.opticalMultiplexingSystems[s].availableWavelengths[f] == 1)
								omsFull = false;
						}
						if (omsFull)
							physicalTopologyIllustrator.physicalTopologyAdjacencyMatrix[currentPhysicalTopology.opticalMultiplexingSystems[s].sourceNode-1][currentPhysicalTopology.opticalMultiplexingSystems[s].destinationNode-1] = 0;
					}


					//outputSignals[0]->bufferPut((t_physical_topology)currentPhysicalTopology);
					outputSignals[0]->bufferPut((t_physical_topology)physicalTopologyIllustrator);

					pathRequestRouted.pathInformation.requestIndex = newPathRequest.requestIndex;
					pathRequestRouted.pathInformation.demandIndex = newPathRequest.demandIndex;
					pathRequestRouted.pathInformation.oduType = newPathRequest.oduType;
					pathRequestRouted.pathInformation.routed = true;
					pathRequestRouted.pathInformation.numberOfLightPaths = 1;

					t_light_paths_table lpt;

					lpt.sourceNode = newPathRequest.sourceNode;
					lpt.destinationNode = newPathRequest.destinationNode;

					if (availableWavelenghtsIndexOfAllOMS.size()==1)
						lpt.numberOfIntermediateNodes = -1;
					else
						lpt.numberOfIntermediateNodes = availableWavelenghtsIndexOfAllOMS.size() - 1;

					lpt.numberOfIntermediateNodes = intermediateNodes.size();
					lpt.intermediateNodes = intermediateNodes;
					lpt.wavelength = wavelenghtSelected;
					pathRequestRouted.lightPathsTable.push_back(lpt);

					outputSignals[1]->bufferPut((t_path_request_routed)pathRequestRouted);



					intermediateNodes.clear();
				}
				intermediateNodes.clear();
				pathsLine++;
				availableWavelenghtsIndexOfAllOMS.clear();


			}

			if (pathAvailable == false) // PathRequest was not successful
			{
				t_path_request_routed pathRequestRouted;
				pathRequestRouted.pathInformation.requestIndex = newPathRequest.requestIndex;
				pathRequestRouted.pathInformation.demandIndex = newPathRequest.demandIndex;
				pathRequestRouted.pathInformation.oduType = newPathRequest.oduType;
				pathRequestRouted.pathInformation.routed = false;
				pathRequestRouted.pathInformation.numberOfLightPaths = -1;
				t_light_paths_table lpt;

				lpt.sourceNode = newPathRequest.sourceNode;
				lpt.destinationNode = newPathRequest.destinationNode;
				pathRequestRouted.lightPathsTable.push_back(lpt);

				outputSignals[1]->bufferPut((t_path_request_routed)pathRequestRouted);
			}
		}
		return true;
	}
	else if (process2 != 0 && currentPhysicalTopology.transportMode == transport_mode::opaque)
	{
		t_path_request pathRequest;
		t_path_request_routed pathRequestRouted;

		std::vector<t_path_request> pathRequests;

		bool wavelengthAvailable = true;
		std::vector<t_integer> OMSIndex;


		if (process2 > 1)
		{
			for (t_integer p = 0; p < process2; p++)
			{
				inputSignals[1]->bufferGet(&pathRequest);

				pathRequests.push_back(pathRequest);
			}
		}

		if (process2 == 1)
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

				for (size_t j = 0; j < currentPhysicalTopology.opticalMultiplexingSystems.size(); j++) // verifica se tem wavelengths disponíveis
				{
					if (src == currentPhysicalTopology.opticalMultiplexingSystems[j].sourceNode && dst == currentPhysicalTopology.opticalMultiplexingSystems[j].destinationNode)
					{
						if (currentPhysicalTopology.opticalMultiplexingSystems[j].availableWavelengths.back() == 0) //não tem wavelength disponível
						{
							wavelengthAvailable = false;

							t_light_paths_table lightPathsTable;

							lightPathsTable.sourceNode = src;
							lightPathsTable.destinationNode = dst;
							lightPathsTable.wavelength = -1;
							pathRequestRouted.lightPathsTable.push_back(lightPathsTable);
						}

						if (currentPhysicalTopology.opticalMultiplexingSystems[j].availableWavelengths.front() == 1)
							OMSIndex.push_back(j);

						else if ((currentPhysicalTopology.opticalMultiplexingSystems[j].availableWavelengths.front() != 1) && (currentPhysicalTopology.opticalMultiplexingSystems[j].availableWavelengths.back() == 1))
						{
							bool availableWavelenght = false;
							t_integer k = 0;
							while ((currentPhysicalTopology.opticalMultiplexingSystems[j].availableWavelengths[k] != 1) && (!availableWavelenght))
							{
								if (currentPhysicalTopology.opticalMultiplexingSystems[j].availableWavelengths[k + 1] == 1)
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

				for (size_t j = 0; j < currentPhysicalTopology.opticalMultiplexingSystems.size(); j++) // verifica se tem wavelengths disponíveis
				{
					if (src == currentPhysicalTopology.opticalMultiplexingSystems[j].sourceNode && dst == currentPhysicalTopology.opticalMultiplexingSystems[j].destinationNode)
					{
						if (currentPhysicalTopology.opticalMultiplexingSystems[j].availableWavelengths.back() == 0) //não tem wavelength disponível
						{
							wavelengthAvailable = false;

							t_light_paths_table lightPathsTable;

							lightPathsTable.sourceNode = src;
							lightPathsTable.destinationNode = dst;
							lightPathsTable.wavelength = -1;
							pathRequestRouted.lightPathsTable.push_back(lightPathsTable);
						}

						if (currentPhysicalTopology.opticalMultiplexingSystems[j].availableWavelengths.front() == 1)
							OMSIndex.push_back(j);

						else if ((currentPhysicalTopology.opticalMultiplexingSystems[j].availableWavelengths.front() != 1) && (currentPhysicalTopology.opticalMultiplexingSystems[j].availableWavelengths.back() == 1))
						{
							bool availableWavelenght = false;
							t_integer k = 0;
							while ((currentPhysicalTopology.opticalMultiplexingSystems[j].availableWavelengths[k] != 1) && (!availableWavelenght))
							{
								if (currentPhysicalTopology.opticalMultiplexingSystems[j].availableWavelengths[k + 1] == 1)
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

				if (currentPhysicalTopology.opticalMultiplexingSystems[OMSIndex[i]].availableWavelengths[0] == 1)
				{
					currentPhysicalTopology.opticalMultiplexingSystems[OMSIndex[i]].availableWavelengths[0] = 0;
					lightPathsTable.wavelength = (double)currentPhysicalTopology.opticalMultiplexingSystems[OMSIndex[i]].wavelengths[0];
				}

				else if (currentPhysicalTopology.opticalMultiplexingSystems[OMSIndex[i]].availableWavelengths[0] != 1)
				{
					t_integer k = 0;
					bool foundWavelenght = false;

					while (currentPhysicalTopology.opticalMultiplexingSystems[OMSIndex[i]].availableWavelengths[k] != 1 && foundWavelenght == false)
					{
						if (currentPhysicalTopology.opticalMultiplexingSystems[OMSIndex[i]].availableWavelengths[k + 1] == 1)
						{
							currentPhysicalTopology.opticalMultiplexingSystems[OMSIndex[i]].availableWavelengths[k + 1] = 0;
							lightPathsTable.wavelength = (double)currentPhysicalTopology.opticalMultiplexingSystems[OMSIndex[i]].wavelengths[k + 1];

							foundWavelenght = true;
						}
						k++;
					}
				}
				lightPathsTable.sourceNode = currentPhysicalTopology.opticalMultiplexingSystems[OMSIndex[i]].sourceNode;
				lightPathsTable.destinationNode = currentPhysicalTopology.opticalMultiplexingSystems[OMSIndex[i]].destinationNode;
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

		if (process2 == 1)
		{
			pathRequestRouted.pathInformation.requestIndex = pathRequest.requestIndex;
			pathRequestRouted.pathInformation.numberOfLightPaths = pathRequest.numberOfIntermediateNodes + 1;
			pathRequestRouted.pathInformation.demandIndex = pathRequest.demandIndex;
			pathRequestRouted.pathInformation.oduType = pathRequest.oduType;
		}

		else
		{
			//pathReuquestRouted.pathInformation.requestIndex
			pathRequestRouted.pathInformation.numberOfLightPaths = pathRequests.size();
			pathRequestRouted.pathInformation.demandIndex = pathRequest.demandIndex;
			pathRequestRouted.pathInformation.oduType = pathRequest.oduType;
			pathRequestRouted.pathInformation.requestIndex = pathRequest.requestIndex;
		}

		outputSignals[1]->bufferPut((t_path_request_routed)pathRequestRouted);
		outputSignals[0]->bufferPut((t_physical_topology)currentPhysicalTopology);

		return true;
	}
};
