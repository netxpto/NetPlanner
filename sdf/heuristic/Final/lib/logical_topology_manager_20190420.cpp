# include "..\include\logical_topology_manager_20190420.h"
# include "..\include\Dijkstra_20190503.h"

using namespace std;


void LogicalTopologyManager::initialize(void) {
	numberOfOutputSignals = (int)outputSignals.size();

	routingCriterionLogicalTopology = getRoutingCriterionLogicalTopology();
	blockingCriterionLogicalTopology = getBlockingCriterionLogicalTopology();
}

bool LogicalTopologyManager::runBlock(void) {
	

	t_demand demand;
	t_demand_request_routed demandProcessed;
	t_path_request_routed pathRequestRouted;


	int process = inputSignals[0]->ready();		// LogicalTopology signal from LogicalTopologyManager_ block
	int process2 = inputSignals[1]->ready();	// number of samples in the buffer ready to be processed
	int process3 = inputSignals[2]->ready();	// PathRequestRouted signals from PhysicalTopologyManager_ block ready to be processed

	if (process == 0 && process2 == 0 && process3 == 0) return false;

	if (process != 0)
	{

		for (int i = 0; i < process; i++) {
			inputSignals[0]->bufferGet(&currentLogicalTopology);

			// Logical matrices for all ODU types
			logicalTopology_odu0 = currentLogicalTopology.logicalTopologyAdjacencyMatrix;
			logicalTopology_odu1 = currentLogicalTopology.logicalTopologyAdjacencyMatrix;
			logicalTopology_odu2 = currentLogicalTopology.logicalTopologyAdjacencyMatrix;
			logicalTopology_odu3 = currentLogicalTopology.logicalTopologyAdjacencyMatrix;
			logicalTopology_odu4 = currentLogicalTopology.logicalTopologyAdjacencyMatrix;
		}	
	}

	bool notProcessNextDemand = false;
	
	//if (process3 != 0) // Process PathRequestRouted signals
	//{
		if(currentLogicalTopology.transportMode == transport_mode::transparent && process3 != 0)
		{
		for (int i = 0; i < process3; i++) {
			inputSignals[2]->bufferGet(&pathRequestRouted);

			if (pathRequestRouted.pathInformation.routed == false) // Demand was not routed and has to be blocked
			{
				// direct logical connection is cutted off for demands of the same ODU type (0)
				if (pathRequestRouted.pathInformation.oduType == 0)
				{
					logicalTopology_odu0[pathRequestRouted.lightPathsTable[0].sourceNode - 1][pathRequestRouted.lightPathsTable[0].destinationNode - 1] = 0;
					currentLogicalTopology.logicalTopologyAdjacencyMatrix[pathRequestRouted.lightPathsTable[0].sourceNode - 1][pathRequestRouted.lightPathsTable[0].destinationNode - 1] = 0;
				}
				// direct logical connection is cutted off for demands of the same ODU type (1)
				else if (pathRequestRouted.pathInformation.oduType == 1)
					logicalTopology_odu1[pathRequestRouted.lightPathsTable[0].sourceNode - 1][pathRequestRouted.lightPathsTable[0].destinationNode - 1] = 0;
				// direct logical connection is cutted off for demands of the same ODU type (2)
				else if (pathRequestRouted.pathInformation.oduType == 2)
					logicalTopology_odu2[pathRequestRouted.lightPathsTable[0].sourceNode - 1][pathRequestRouted.lightPathsTable[0].destinationNode - 1] = 0;
				// direct logical connection is cutted off for demands of the same ODU type (3)
				else if (pathRequestRouted.pathInformation.oduType == 3)
					logicalTopology_odu3[pathRequestRouted.lightPathsTable[0].sourceNode - 1][pathRequestRouted.lightPathsTable[0].destinationNode - 1] = 0;
				// direct logical connection is cutted off for demands of the same ODU type (4)
				else if (pathRequestRouted.pathInformation.oduType == 4)
					logicalTopology_odu4[pathRequestRouted.lightPathsTable[0].sourceNode - 1][pathRequestRouted.lightPathsTable[0].destinationNode - 1] = 0;

				demandProcessed.demandIndex = pathRequestRouted.pathInformation.demandIndex;
				demandProcessed.routed = false;
				demandProcessed.pathIndex = -1;
				outputSignals[1]->bufferPut((t_demand_request_routed)demandProcessed);
			}
			else // if the demand was routed and a new path was created in order to do that
			{
				// First update LogicalTopologyManager_ block state variables

				//OpticalChannels variable update
				t_paths newPath;
				t_light_paths newLightPath;
				t_optical_channels newOpticalChannel;

				t_integer capacity{ 0 };
				t_integer numberOfOpticalChannels{ 0 };
				std::vector<int> ochsIndex;

				if (pathRequestRouted.pathInformation.oduType == 0)
					capacity = 1;
				else if (pathRequestRouted.pathInformation.oduType == 1)
					capacity = 2;
				else if (pathRequestRouted.pathInformation.oduType == 2)
					capacity = 8;
				else if (pathRequestRouted.pathInformation.oduType == 3)
					capacity = 32;
				else if (pathRequestRouted.pathInformation.oduType == 4)
					capacity = 80;

				if (pathRequestRouted.lightPathsTable[0].numberOfIntermediateNodes == -1)
					numberOfOpticalChannels = 1;
				else
					numberOfOpticalChannels = pathRequestRouted.lightPathsTable[0].numberOfIntermediateNodes + 1;


				// Create new optical channels
				for (size_t i = 0; i < numberOfOpticalChannels; i++)
				{
					t_optical_channels newOpticalChannel;

					if (i == 0 && numberOfOpticalChannels == 1)
					{
						newOpticalChannel.opticalChannelIndex = currentLogicalTopology.opticalChannels.size();
						newOpticalChannel.sourceNode = pathRequestRouted.lightPathsTable[0].sourceNode;
						newOpticalChannel.destinationNode = pathRequestRouted.lightPathsTable[0].destinationNode;
						newOpticalChannel.capacity = 80 - capacity;
						newOpticalChannel.wavelength = pathRequestRouted.lightPathsTable[0].wavelength;
						newOpticalChannel.numberOfDemands = 1;
						newOpticalChannel.demandsIndex.push_back(pathRequestRouted.pathInformation.demandIndex);


						ochsIndex.push_back(newOpticalChannel.opticalChannelIndex);
					}
					else if (i == 0 && numberOfOpticalChannels > 1)
					{
						newOpticalChannel.opticalChannelIndex = currentLogicalTopology.opticalChannels.size();
						newOpticalChannel.sourceNode = pathRequestRouted.lightPathsTable[0].sourceNode;
						newOpticalChannel.destinationNode = pathRequestRouted.lightPathsTable[0].intermediateNodes[i];
						newOpticalChannel.capacity = 80 - capacity;
						newOpticalChannel.wavelength = pathRequestRouted.lightPathsTable[0].wavelength;
						newOpticalChannel.numberOfDemands = 1;
						newOpticalChannel.demandsIndex.push_back(pathRequestRouted.pathInformation.demandIndex);


						ochsIndex.push_back(newOpticalChannel.opticalChannelIndex);
					}
					else if (i > 0 && numberOfOpticalChannels > 1 && i < (numberOfOpticalChannels - 1))
					{
						newOpticalChannel.opticalChannelIndex = currentLogicalTopology.opticalChannels.size();
						newOpticalChannel.sourceNode = pathRequestRouted.lightPathsTable[0].intermediateNodes[i - 1];
						newOpticalChannel.destinationNode = pathRequestRouted.lightPathsTable[0].intermediateNodes[i];
						newOpticalChannel.capacity = 80 - capacity;
						newOpticalChannel.wavelength = pathRequestRouted.lightPathsTable[0].wavelength;
						newOpticalChannel.numberOfDemands = 1;
						newOpticalChannel.demandsIndex.push_back(pathRequestRouted.pathInformation.demandIndex);


						ochsIndex.push_back(newOpticalChannel.opticalChannelIndex);
					}
					else if (numberOfOpticalChannels > 1 && i == numberOfOpticalChannels - 1) // last optical channel to be created
					{
						newOpticalChannel.opticalChannelIndex = currentLogicalTopology.opticalChannels.size();
						newOpticalChannel.sourceNode = pathRequestRouted.lightPathsTable[0].intermediateNodes[i - 1];
						newOpticalChannel.destinationNode = pathRequestRouted.lightPathsTable[0].destinationNode;
						newOpticalChannel.capacity = 80 - capacity;
						newOpticalChannel.wavelength = pathRequestRouted.lightPathsTable[0].wavelength;
						newOpticalChannel.numberOfDemands = 1;
						newOpticalChannel.demandsIndex.push_back(pathRequestRouted.pathInformation.demandIndex);


						ochsIndex.push_back(newOpticalChannel.opticalChannelIndex);
					}
					currentLogicalTopology.opticalChannels.push_back(newOpticalChannel);
				}
				// Create new lightpath

				newLightPath.lightPathIndex = currentLogicalTopology.lightPaths.size();
				newLightPath.sourceNode = pathRequestRouted.lightPathsTable[0].sourceNode;
				newLightPath.destinationNode = pathRequestRouted.lightPathsTable[0].destinationNode;
				newLightPath.capacity = 80 - capacity;
				newLightPath.numberOfOpticalChannels = numberOfOpticalChannels;
				newLightPath.opticalChannelsIndex = ochsIndex;

				currentLogicalTopology.lightPaths.push_back(newLightPath);

				// Create new path

				newPath.pathIndex = currentLogicalTopology.paths.size();
				newPath.sourceNode = pathRequestRouted.lightPathsTable[0].sourceNode;
				newPath.destinationNode = pathRequestRouted.lightPathsTable[0].destinationNode;
				newPath.capacity = 80 - capacity;
				newPath.numberOfLightPaths = 1;
				newPath.lightPathsIndex.push_back(currentLogicalTopology.lightPaths.size() - 1);

				currentLogicalTopology.paths.push_back(newPath);

				// Print DemandRequestRouted signals
				demandProcessed.demandIndex = pathRequestRouted.pathInformation.demandIndex;
				demandProcessed.routed = true;
				demandProcessed.pathIndex = newPath.pathIndex;
				outputSignals[1]->bufferPut((t_demand_request_routed)demandProcessed);
				ochsIndex.clear();
			}


		}
	}
	if (currentLogicalTopology.transportMode == transport_mode::opaque && process3 !=0)
	{
		std::vector<t_light_paths_table> lightPathsTable;

		for (t_integer p = 0; p < process3; p++)
		{
			t_path_request_routed pathRequestRouted;
			inputSignals[2]->bufferGet(&pathRequestRouted);

			t_demand_request_routed processedDemand;

			if (pathRequestRouted.pathInformation.routed == true)
			{
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

				// se foi requerido wavelengths para todos os lightPaths que constituem o path
				if (pathRequestRouted.lightPathsTable.size() == pathDij.size() - 1)
				{
					std::vector<t_integer> newLightPathsIndex;

					for (size_t i = 0; i < pathRequestRouted.lightPathsTable.size(); i++)
					{
						t_optical_channels newOpticalChannel; // novo opticalChannel
						newOpticalChannel.opticalChannelIndex = currentLogicalTopology.opticalChannels.size();
						newOpticalChannel.sourceNode = pathRequestRouted.lightPathsTable[i].sourceNode;
						newOpticalChannel.destinationNode = pathRequestRouted.lightPathsTable[i].destinationNode;
						newOpticalChannel.wavelength = pathRequestRouted.lightPathsTable[i].wavelength;
						newOpticalChannel.capacity = 80 - odu0s;
						newOpticalChannel.numberOfDemands = 1;
						newOpticalChannel.demandsIndex.push_back(demand.demandIndex);
						currentLogicalTopology.opticalChannels.push_back(newOpticalChannel);
						newOpticalChannel.demandsIndex.clear();

						t_light_paths newLightPath; // novo lightPath
						newLightPath.lightPathIndex = currentLogicalTopology.lightPaths.size();
						newLightPath.sourceNode = pathRequestRouted.lightPathsTable[i].sourceNode;
						newLightPath.destinationNode = pathRequestRouted.lightPathsTable[i].destinationNode;
						newLightPath.capacity = 80 - odu0s;
						newLightPath.numberOfOpticalChannels = 1;
						newLightPath.opticalChannelsIndex.push_back(newOpticalChannel.opticalChannelIndex);
						currentLogicalTopology.lightPaths.push_back(newLightPath);
						newLightPath.opticalChannelsIndex.clear();

						newLightPathsIndex.push_back(newLightPath.lightPathIndex);
					}
					t_paths newPath; // novo path
					newPath.pathIndex = currentLogicalTopology.paths.size();
					newPath.sourceNode = pathRequestRouted.lightPathsTable[0].sourceNode;
					newPath.destinationNode = pathRequestRouted.lightPathsTable[pathRequestRouted.lightPathsTable.size() - 1].destinationNode;
					newPath.capacity = 80 - odu0s;
					newPath.numberOfLightPaths = pathRequestRouted.pathInformation.numberOfLightPaths;
					newPath.lightPathsIndex = newLightPathsIndex;
					currentLogicalTopology.paths.push_back(newPath);

					processedDemand.demandIndex = demand.demandIndex;
					processedDemand.routed = true;
					processedDemand.pathIndex = newPath.pathIndex;
					outputSignals[1]->bufferPut((t_demand_request_routed)processedDemand);

					temporaryLogicalMatrix = currentLogicalTopology.logicalTopologyAdjacencyMatrix;

				}

				else // se foi requerido wavelengths só para alguns lightPaths que constituem o path
				{
					std::vector<t_integer> newLightPathsIndex;

					for (size_t i = 0; i < pathRequestRouted.lightPathsTable.size(); i++)
					{
						t_optical_channels newOpticalChannel; // novo opticalChannel
						newOpticalChannel.opticalChannelIndex = currentLogicalTopology.opticalChannels.size();
						newOpticalChannel.sourceNode = pathRequestRouted.lightPathsTable[i].sourceNode;
						newOpticalChannel.destinationNode = pathRequestRouted.lightPathsTable[i].destinationNode;
						newOpticalChannel.wavelength = pathRequestRouted.lightPathsTable[i].wavelength;
						newOpticalChannel.capacity = 80;
						currentLogicalTopology.opticalChannels.push_back(newOpticalChannel);

						t_light_paths newLightPath; // novo lightPath
						newLightPath.lightPathIndex = newOpticalChannel.opticalChannelIndex;
						newLightPath.sourceNode = newOpticalChannel.sourceNode;
						newLightPath.destinationNode = newOpticalChannel.destinationNode;
						newLightPath.capacity = 80;
						newLightPath.numberOfOpticalChannels = 1;
						newLightPath.opticalChannelsIndex.push_back(newOpticalChannel.opticalChannelIndex);
						currentLogicalTopology.lightPaths.push_back(newLightPath);
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
						while (j < currentLogicalTopology.lightPaths.size() && lightPathWithCapacity == false)
						{
							if (src == currentLogicalTopology.lightPaths[j].sourceNode && dst == currentLogicalTopology.lightPaths[j].destinationNode)
							{
								if (currentLogicalTopology.lightPaths[j].capacity >= odu0s)
								{
									lightPathWithCapacity = true;
									// atualizar lightPaths e opticalChannels
									currentLogicalTopology.lightPaths[j].capacity -= odu0s;
									currentLogicalTopology.opticalChannels[j].capacity -= odu0s;
									currentLogicalTopology.opticalChannels[j].numberOfDemands++;
									currentLogicalTopology.opticalChannels[j].demandsIndex.push_back(demand.demandIndex);

									for (size_t k = 0; k < currentLogicalTopology.paths.size(); k++)
									{
										for (size_t l = 0; l < currentLogicalTopology.paths[k].lightPathsIndex.size(); l++)
										{
											if (currentLogicalTopology.paths[k].lightPathsIndex[l] == i)
											{
												if (currentLogicalTopology.paths[k].capacity > currentLogicalTopology.lightPaths[j].capacity)
													currentLogicalTopology.paths[k].capacity = currentLogicalTopology.lightPaths[j].capacity;
											}
										}
									}
									capacityLightPaths.push_back(currentLogicalTopology.lightPaths[j].capacity);
									lightPathsIndex.push_back(j);
								}
							}
							j++;
						}
					}

					t_paths newPath; // novo path
					newPath.pathIndex = currentLogicalTopology.paths.size();
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
					currentLogicalTopology.paths.push_back(newPath);

					// verfica se outros paths utilizam os lightPaths que foram atualizados, de forma a atualizar a sua capacidade 
					for (size_t i = 0; i < lightPathsIndex.size(); i++)
					{
						for (size_t j = 0; j < currentLogicalTopology.paths.size(); j++)
						{
							for (size_t k = 0; k < currentLogicalTopology.paths[j].lightPathsIndex.size(); k++)
							{
								if (currentLogicalTopology.paths[j].lightPathsIndex[k] == lightPathsIndex[i])
								{
									if (currentLogicalTopology.paths[j].capacity > currentLogicalTopology.lightPaths[lightPathsIndex[i]].capacity)
										currentLogicalTopology.paths[j].capacity = currentLogicalTopology.lightPaths[lightPathsIndex[i]].capacity;
								}
							}
						}
					}
					processedDemand.demandIndex = demand.demandIndex;
					processedDemand.routed = true;
					processedDemand.pathIndex = newPath.pathIndex;
					outputSignals[1]->bufferPut((t_demand_request_routed)processedDemand);

					temporaryLogicalMatrix = currentLogicalTopology.logicalTopologyAdjacencyMatrix;
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
					outputSignals[1]->bufferPut((t_demand_request_routed)processedDemand);

					temporaryLogicalMatrix = currentLogicalTopology.logicalTopologyAdjacencyMatrix;
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
						for (size_t k = 0; k < currentLogicalTopology.lightPaths.size(); k++)
						{
							if (src == currentLogicalTopology.lightPaths[k].sourceNode && dst == currentLogicalTopology.lightPaths[k].destinationNode)
							{
								if (currentLogicalTopology.lightPaths[k].capacity != 0)
									block = false;
							}
						}
						if (block == true)
							currentLogicalTopology.logicalTopologyAdjacencyMatrix[src - 1][dst - 1] = 0;
					}
				}
			}
			pathDij.clear();
		}
	}


	if (process2 != 0 && currentLogicalTopology.transportMode == transport_mode::transparent)
	{
		for (int i = 0; i < process2; i++) {
			inputSignals[1]->bufferGet(&demand); // Reads a demand
			int oduType = demand.oduType;
			bool pathFound{false};
			
			//cout << demandProcessed.demandIndex << endl;
			//outputSignals[1]->bufferPut((t_demand)demandProcessed);

			int numberOfPaths{ 0 };
			while(numberOfPaths < currentLogicalTopology.paths.size() && pathFound == false) {

				if (currentLogicalTopology.paths[numberOfPaths].sourceNode == demand.sourceNode) // Find a path with the same source node of the demand
				{
					if (currentLogicalTopology.paths[numberOfPaths].destinationNode == demand.destinationNode)// Find a path with the same destination node of the demand
					{
						switch (oduType) {
						case 0:
							if (currentLogicalTopology.paths[numberOfPaths].capacity >= 1)
							{
								int numberOfLP;
								int numberOfOCh;
								// Then all criteria was met to route the demand through this path previously established
								pathFound = true;

								// Refresh path capacity variable

								currentLogicalTopology.paths[numberOfPaths].capacity = currentLogicalTopology.paths[numberOfPaths].capacity - 1;

								// Refresh lightPaths capacity variables
								for (numberOfLP = 0; numberOfLP < currentLogicalTopology.paths[numberOfPaths].numberOfLightPaths; numberOfLP++)
								{
									currentLogicalTopology.lightPaths[currentLogicalTopology.paths[numberOfPaths].lightPathsIndex[numberOfLP]].capacity = currentLogicalTopology.lightPaths[currentLogicalTopology.paths[numberOfPaths].lightPathsIndex[numberOfLP]].capacity - 1;

									// Refresh opticalChannels capacity, numberOfDemands and demandsIndex variables
									for (numberOfOCh = 0; numberOfOCh < currentLogicalTopology.lightPaths[currentLogicalTopology.paths[numberOfPaths].lightPathsIndex[numberOfLP]].numberOfOpticalChannels; numberOfOCh++)
									{
										currentLogicalTopology.opticalChannels[currentLogicalTopology.lightPaths[currentLogicalTopology.paths[numberOfPaths].lightPathsIndex[numberOfLP]].opticalChannelsIndex[numberOfOCh]].capacity -= 1;
										currentLogicalTopology.opticalChannels[currentLogicalTopology.lightPaths[currentLogicalTopology.paths[numberOfPaths].lightPathsIndex[numberOfLP]].opticalChannelsIndex[numberOfOCh]].numberOfDemands++;
										currentLogicalTopology.opticalChannels[currentLogicalTopology.lightPaths[currentLogicalTopology.paths[numberOfPaths].lightPathsIndex[numberOfLP]].opticalChannelsIndex[numberOfOCh]].demandsIndex.push_back(demand.demandIndex);
									}
								}
								// Print DemandRequestRouted signals
								demandProcessed.demandIndex = demand.demandIndex;
								demandProcessed.routed = true;
								demandProcessed.pathIndex = currentLogicalTopology.paths[numberOfPaths].pathIndex;
								outputSignals[1]->bufferPut((t_demand_request_routed)demandProcessed);
								
							}
							break;
						case 1:
							if (currentLogicalTopology.paths[numberOfPaths].capacity >= 2)
							{
								int numberOfLP;
								int numberOfOCh;
								// Then all criteria was met to route the demand through this path previously established
								pathFound = true;

								// Refresh path capacity variable

								currentLogicalTopology.paths[numberOfPaths].capacity = currentLogicalTopology.paths[numberOfPaths].capacity - 2;

								// Refresh lightPaths capacity variables
								for (numberOfLP = 0; numberOfLP < currentLogicalTopology.paths[numberOfPaths].numberOfLightPaths; numberOfLP++)
								{
									currentLogicalTopology.lightPaths[currentLogicalTopology.paths[numberOfPaths].lightPathsIndex[numberOfLP]].capacity = currentLogicalTopology.lightPaths[currentLogicalTopology.paths[numberOfPaths].lightPathsIndex[numberOfLP]].capacity - 2;

									// Refresh opticalChannels capacity, numberOfDemands and demandsIndex variables
									for (numberOfOCh = 0; numberOfOCh < currentLogicalTopology.lightPaths[currentLogicalTopology.paths[numberOfPaths].lightPathsIndex[numberOfLP]].numberOfOpticalChannels; numberOfOCh++)
									{
										currentLogicalTopology.opticalChannels[currentLogicalTopology.lightPaths[currentLogicalTopology.paths[numberOfPaths].lightPathsIndex[numberOfLP]].opticalChannelsIndex[numberOfOCh]].capacity -= 2;
										currentLogicalTopology.opticalChannels[currentLogicalTopology.lightPaths[currentLogicalTopology.paths[numberOfPaths].lightPathsIndex[numberOfLP]].opticalChannelsIndex[numberOfOCh]].numberOfDemands++;
										currentLogicalTopology.opticalChannels[currentLogicalTopology.lightPaths[currentLogicalTopology.paths[numberOfPaths].lightPathsIndex[numberOfLP]].opticalChannelsIndex[numberOfOCh]].demandsIndex.push_back(demand.demandIndex);
									}
								}
								// Print DemandRequestRouted signals
								demandProcessed.demandIndex = demand.demandIndex;
								demandProcessed.routed = true;
								demandProcessed.pathIndex = currentLogicalTopology.paths[numberOfPaths].pathIndex;
								outputSignals[1]->bufferPut((t_demand_request_routed)demandProcessed);
							}
							break;
						case 2:
							if (currentLogicalTopology.paths[numberOfPaths].capacity >= 8)
							{
								int numberOfLP;
								int numberOfOCh;
								// Then all criteria was met to route the demand through this path previously established
								pathFound = true;

								// Refresh path capacity variable

								currentLogicalTopology.paths[numberOfPaths].capacity = currentLogicalTopology.paths[numberOfPaths].capacity - 8;

								// Refresh lightPaths capacity variables
								for (numberOfLP = 0; numberOfLP < currentLogicalTopology.paths[numberOfPaths].numberOfLightPaths; numberOfLP++)
								{
									currentLogicalTopology.lightPaths[currentLogicalTopology.paths[numberOfPaths].lightPathsIndex[numberOfLP]].capacity = currentLogicalTopology.lightPaths[currentLogicalTopology.paths[numberOfPaths].lightPathsIndex[numberOfLP]].capacity - 8;

									// Refresh opticalChannels capacity, numberOfDemands and demandsIndex variables
									for (numberOfOCh = 0; numberOfOCh < currentLogicalTopology.lightPaths[currentLogicalTopology.paths[numberOfPaths].lightPathsIndex[numberOfLP]].numberOfOpticalChannels; numberOfOCh++)
									{
										currentLogicalTopology.opticalChannels[currentLogicalTopology.lightPaths[currentLogicalTopology.paths[numberOfPaths].lightPathsIndex[numberOfLP]].opticalChannelsIndex[numberOfOCh]].capacity -= 8;
										currentLogicalTopology.opticalChannels[currentLogicalTopology.lightPaths[currentLogicalTopology.paths[numberOfPaths].lightPathsIndex[numberOfLP]].opticalChannelsIndex[numberOfOCh]].numberOfDemands++;
										currentLogicalTopology.opticalChannels[currentLogicalTopology.lightPaths[currentLogicalTopology.paths[numberOfPaths].lightPathsIndex[numberOfLP]].opticalChannelsIndex[numberOfOCh]].demandsIndex.push_back(demand.demandIndex);
									}
								}
								// Print DemandRequestRouted signals
								demandProcessed.demandIndex = demand.demandIndex;
								demandProcessed.routed = true;
								demandProcessed.pathIndex = currentLogicalTopology.paths[numberOfPaths].pathIndex;
								outputSignals[1]->bufferPut((t_demand_request_routed)demandProcessed);
							}
							break;
						case 3:
							if (currentLogicalTopology.paths[numberOfPaths].capacity >= 32)
							{
								int numberOfLP;
								int numberOfOCh;
								// Then all criteria was met to route the demand through this path previously established
								pathFound = true;

								// Refresh path capacity variable

								currentLogicalTopology.paths[numberOfPaths].capacity = currentLogicalTopology.paths[numberOfPaths].capacity - 32;

								// Refresh lightPaths capacity variables
								for (numberOfLP = 0; numberOfLP < currentLogicalTopology.paths[numberOfPaths].numberOfLightPaths; numberOfLP++)
								{
									currentLogicalTopology.lightPaths[currentLogicalTopology.paths[numberOfPaths].lightPathsIndex[numberOfLP]].capacity = currentLogicalTopology.lightPaths[currentLogicalTopology.paths[numberOfPaths].lightPathsIndex[numberOfLP]].capacity - 32;

									// Refresh opticalChannels capacity, numberOfDemands and demandsIndex variables
									for (numberOfOCh = 0; numberOfOCh < currentLogicalTopology.lightPaths[currentLogicalTopology.paths[numberOfPaths].lightPathsIndex[numberOfLP]].numberOfOpticalChannels; numberOfOCh++)
									{
										currentLogicalTopology.opticalChannels[currentLogicalTopology.lightPaths[currentLogicalTopology.paths[numberOfPaths].lightPathsIndex[numberOfLP]].opticalChannelsIndex[numberOfOCh]].capacity -= 32;
										currentLogicalTopology.opticalChannels[currentLogicalTopology.lightPaths[currentLogicalTopology.paths[numberOfPaths].lightPathsIndex[numberOfLP]].opticalChannelsIndex[numberOfOCh]].numberOfDemands++;
										currentLogicalTopology.opticalChannels[currentLogicalTopology.lightPaths[currentLogicalTopology.paths[numberOfPaths].lightPathsIndex[numberOfLP]].opticalChannelsIndex[numberOfOCh]].demandsIndex.push_back(demand.demandIndex);
									}
								}
								// Print DemandRequestRouted signals
								demandProcessed.demandIndex = demand.demandIndex;
								demandProcessed.routed = true;
								demandProcessed.pathIndex = currentLogicalTopology.paths[numberOfPaths].pathIndex;
								outputSignals[1]->bufferPut((t_demand_request_routed)demandProcessed);
							}
							break;
						case 4:
							if (currentLogicalTopology.paths[numberOfPaths].capacity >= 80)
							{
								int numberOfLP;
								int numberOfOCh;
								// Then all criteria was met to route the demand through this path previously established
								pathFound = true;

								// Refresh path capacity variable

								currentLogicalTopology.paths[numberOfPaths].capacity = currentLogicalTopology.paths[numberOfPaths].capacity - 80;

								// Refresh lightPaths capacity variables
								for (numberOfLP = 0; numberOfLP < currentLogicalTopology.paths[numberOfPaths].numberOfLightPaths; numberOfLP++)
								{
									currentLogicalTopology.lightPaths[currentLogicalTopology.paths[numberOfPaths].lightPathsIndex[numberOfLP]].capacity = currentLogicalTopology.lightPaths[currentLogicalTopology.paths[numberOfPaths].lightPathsIndex[numberOfLP]].capacity - 80;

									// Refresh opticalChannels capacity, numberOfDemands and demandsIndex variables
									for (numberOfOCh = 0; numberOfOCh < currentLogicalTopology.lightPaths[currentLogicalTopology.paths[numberOfPaths].lightPathsIndex[numberOfLP]].numberOfOpticalChannels; numberOfOCh++)
									{
										currentLogicalTopology.opticalChannels[currentLogicalTopology.lightPaths[currentLogicalTopology.paths[numberOfPaths].lightPathsIndex[numberOfLP]].opticalChannelsIndex[numberOfOCh]].capacity -= 80;
										currentLogicalTopology.opticalChannels[currentLogicalTopology.lightPaths[currentLogicalTopology.paths[numberOfPaths].lightPathsIndex[numberOfLP]].opticalChannelsIndex[numberOfOCh]].numberOfDemands++;
										currentLogicalTopology.opticalChannels[currentLogicalTopology.lightPaths[currentLogicalTopology.paths[numberOfPaths].lightPathsIndex[numberOfLP]].opticalChannelsIndex[numberOfOCh]].demandsIndex.push_back(demand.demandIndex);
									}
								}
								// Print DemandRequestRouted signals
								demandProcessed.demandIndex = demand.demandIndex;
								demandProcessed.routed = true;
								demandProcessed.pathIndex = currentLogicalTopology.paths[numberOfPaths].pathIndex;
								outputSignals[1]->bufferPut((t_demand_request_routed)demandProcessed);
							}
							break;
						default:
							break;

						}
					}
				}
				numberOfPaths++;
			}
			

			if (pathFound == false)
			{
				// try to create a new path to route the demand
				t_path_request pathRequest; // Create a PathRequest type signal
				
				if (oduType == 0)
				{
					if (logicalTopology_odu0[demand.sourceNode - 1][demand.destinationNode - 1] == 1)
					{
						
						Graph g(logicalTopology_odu0[0].size()); // Precisa desta linha para executar sem erros (???)
					
						pathRequest.requestIndex = requestIndex;
						pathRequest.demandIndex = demand.demandIndex;
						pathRequest.oduType = demand.oduType;
						pathRequest.sourceNode = demand.sourceNode;
						pathRequest.destinationNode = demand.destinationNode;
						pathRequest.intermediateNodes.push_back(-1);
					

						requestIndex++;
						outputSignals[2]->bufferPut((t_path_request)pathRequest); // Send a PathRequest to the PhysicalTopologyManager_ block
						pathRequest.intermediateNodes.clear();
					}
				}
				else if (oduType == 1)
				{
					if (logicalTopology_odu1[demand.sourceNode - 1][demand.destinationNode - 1] == 1)
					{

						pathRequest.requestIndex = requestIndex;
						pathRequest.demandIndex = demand.demandIndex;
						pathRequest.oduType = demand.oduType;
						pathRequest.sourceNode = demand.sourceNode;
						pathRequest.destinationNode = demand.destinationNode;
						pathRequest.intermediateNodes.push_back(-1);
						requestIndex++;
						outputSignals[2]->bufferPut((t_path_request)pathRequest); // Send a PathRequest to the PhysicalTopologyManager_ block
						pathRequest.intermediateNodes.clear();
					}
				}
				else if (oduType == 2)
				{
					if (logicalTopology_odu2[demand.sourceNode - 1][demand.destinationNode - 1] == 1)
					{
						

						pathRequest.requestIndex = requestIndex;
						pathRequest.demandIndex = demand.demandIndex;
						pathRequest.oduType = demand.oduType;
						pathRequest.sourceNode = demand.sourceNode;
						pathRequest.destinationNode = demand.destinationNode;
						pathRequest.intermediateNodes.push_back(-1);
						requestIndex++;
						outputSignals[2]->bufferPut((t_path_request)pathRequest); // Send a PathRequest to the PhysicalTopologyManager_ block
						pathRequest.intermediateNodes.clear();
					}
				}
				else if (oduType == 3)
				{
					if (logicalTopology_odu3[demand.sourceNode - 1][demand.destinationNode - 1] == 1)
					{

						pathRequest.requestIndex = requestIndex;
						pathRequest.demandIndex = demand.demandIndex;
						pathRequest.oduType = demand.oduType;
						pathRequest.sourceNode = demand.sourceNode;
						pathRequest.destinationNode = demand.destinationNode;
						pathRequest.intermediateNodes.push_back(-1);
						requestIndex++;
						outputSignals[2]->bufferPut((t_path_request)pathRequest); // Send a PathRequest to the PhysicalTopologyManager_ block
						pathRequest.intermediateNodes.clear();
					}
					
				}
				else if (oduType == 4)
				{
					if (logicalTopology_odu4[demand.sourceNode - 1][demand.destinationNode - 1] == 1)
					{
	

						pathRequest.requestIndex = requestIndex;
						pathRequest.demandIndex = demand.demandIndex;
						pathRequest.oduType = demand.oduType;
						pathRequest.sourceNode = demand.sourceNode;
						pathRequest.destinationNode = demand.destinationNode;
						pathRequest.intermediateNodes.push_back(-1);
						requestIndex++;
						outputSignals[2]->bufferPut((t_path_request)pathRequest); // Send a PathRequest to the PhysicalTopologyManager_ block
						pathRequest.intermediateNodes.clear();
					}
				}
					
				}
			
			
		}
	}
	if (process2 != 0 && currentLogicalTopology.transportMode == transport_mode::opaque)
	{
		t_demand_request_routed processedDemand;
		std::vector<t_integer> lightPathsIndex;

		if (process2 != 0 && notProcessNextDemand == false)
		{
			for (t_integer dmd = 0; dmd < process2; dmd++)
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
				while ((i < currentLogicalTopology.paths.size()) && (!pathFound)) // verifica se existe um path com origem e destino iguais aos da demand
				{
					if (demand.sourceNode == currentLogicalTopology.paths[i].sourceNode
						&& demand.destinationNode == currentLogicalTopology.paths[i].destinationNode) // se existir
					{
						if (odu0s <= currentLogicalTopology.paths[i].capacity) // e com capacidade
						{
							// atualizar path
							currentLogicalTopology.paths[i].capacity -= odu0s;

							lightPathsIndex = currentLogicalTopology.paths[i].lightPathsIndex; // guarda os lightPaths, para os procurar nos outros paths 

							for (size_t j = 0; j < lightPathsIndex.size(); j++)
							{
								// atualizar lightPaths e opticalChannels
								currentLogicalTopology.lightPaths[lightPathsIndex[j]].capacity -= odu0s;
								currentLogicalTopology.opticalChannels[lightPathsIndex[j]].capacity -= odu0s;
								currentLogicalTopology.opticalChannels[lightPathsIndex[j]].numberOfDemands++;
								currentLogicalTopology.opticalChannels[lightPathsIndex[j]].demandsIndex.push_back(demand.demandIndex);
							}

							pathFound = true;
							processedDemand.demandIndex = demand.demandIndex;
							processedDemand.routed = true;
							processedDemand.pathIndex = currentLogicalTopology.paths[i].pathIndex;
							outputSignals[1]->bufferPut((t_demand_request_routed)processedDemand);
						}
					}
					i++;
				}

				if (pathFound == true) // verfica se outros paths utilizam os lightPaths que foram atualizados, de forma a atualizar a sua capacidade 
				{
					for (size_t i = 0; i < lightPathsIndex.size(); i++)
					{
						for (size_t j = 0; j < currentLogicalTopology.paths.size(); j++)
						{
							for (size_t k = 0; k < currentLogicalTopology.paths[j].lightPathsIndex.size(); k++)
							{
								if (currentLogicalTopology.paths[j].lightPathsIndex[k] == lightPathsIndex[i])
								{
									if (currentLogicalTopology.paths[j].capacity > currentLogicalTopology.lightPaths[lightPathsIndex[i]].capacity)
										currentLogicalTopology.paths[j].capacity = currentLogicalTopology.lightPaths[lightPathsIndex[i]].capacity;
								}
							}
						}
					}
				}

				if (pathFound == false) // corre o dijkstra que lhe dá os lightPaths 
				{
					adjacency_list_t adjacency_list(currentLogicalTopology.logicalTopologyAdjacencyMatrix.size());

					for (size_t i = 0; i < currentLogicalTopology.logicalTopologyAdjacencyMatrix.size(); i++)
					{
						for (size_t j = 0; j < currentLogicalTopology.logicalTopologyAdjacencyMatrix.size(); j++)
						{
							if (currentLogicalTopology.logicalTopologyAdjacencyMatrix[i][j] != 0)
							{
								if (routingCriterionLogicalTopology == t_routing_criterion_logical_topology::hops)
									adjacency_list[i].push_back(neighbor(j, 1));

								else if (routingCriterionLogicalTopology == t_routing_criterion_logical_topology::distance)
									adjacency_list[i].push_back(neighbor(j, currentLogicalTopology.distancesBetweenNodes[i][j]));
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
						outputSignals[1]->bufferPut((t_demand_request_routed)processedDemand);
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
							while (j < currentLogicalTopology.lightPaths.size() && lightPathWithCapacity == false) // verfica se já existem esses lightPaths
							{
								if (src == currentLogicalTopology.lightPaths[j].sourceNode && dst == currentLogicalTopology.lightPaths[j].destinationNode) // existem
								{
									if (currentLogicalTopology.lightPaths[j].capacity >= odu0s) // com capacidade
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
								currentLogicalTopology.lightPaths[lightPathsWithCapacityIndex[i]].capacity -= odu0s;
								currentLogicalTopology.opticalChannels[lightPathsWithCapacityIndex[i]].capacity -= odu0s;
								currentLogicalTopology.opticalChannels[lightPathsWithCapacityIndex[i]].numberOfDemands++;
								currentLogicalTopology.opticalChannels[lightPathsWithCapacityIndex[i]].demandsIndex.push_back(demand.demandIndex);

								//verificar se existem outros paths a usar estes lightPaths e atualizar a capacidade do path com o menor valor 
								for (size_t j = 0; j < currentLogicalTopology.paths.size(); j++)
								{
									for (size_t k = 0; k < currentLogicalTopology.paths[j].lightPathsIndex.size(); k++)
									{
										if (currentLogicalTopology.paths[j].lightPathsIndex[k] == lightPathsWithCapacityIndex[i])
										{
											if (currentLogicalTopology.paths[j].capacity > currentLogicalTopology.lightPaths[lightPathsWithCapacityIndex[i]].capacity)
												currentLogicalTopology.paths[j].capacity = currentLogicalTopology.lightPaths[lightPathsWithCapacityIndex[i]].capacity;
										}
									}
								}

								capacityLightPaths.push_back(currentLogicalTopology.lightPaths[lightPathsWithCapacityIndex[i]].capacity);

							}
							t_paths newPath; // criar novo path
							newPath.pathIndex = currentLogicalTopology.paths.size();
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
							currentLogicalTopology.paths.push_back(newPath);

							processedDemand.demandIndex = demand.demandIndex;
							processedDemand.routed = true;
							processedDemand.pathIndex = newPath.pathIndex;
							outputSignals[1]->bufferPut((t_demand_request_routed)processedDemand);

							temporaryLogicalMatrix = currentLogicalTopology.logicalTopologyAdjacencyMatrix;
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

							outputSignals[2]->bufferPut((t_path_request)pathRequest);
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
									if (src == currentLogicalTopology.lightPaths[lightPathsWithCapacityIndex[j]].sourceNode
										&& dst == currentLogicalTopology.lightPaths[lightPathsWithCapacityIndex[j]].destinationNode)
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

									outputSignals[2]->bufferPut((t_path_request)pathRequest);
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
						if (routingCriterionLogicalTopology == t_routing_criterion_logical_topology::hops)
							adjacency_list[i].push_back(neighbor(j, 1));

						else if (routingCriterionLogicalTopology == t_routing_criterion_logical_topology::distance)
							adjacency_list[i].push_back(neighbor(j, currentLogicalTopology.distancesBetweenNodes[i][j]));
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
				outputSignals[1]->bufferPut((t_demand_request_routed)processedDemand);
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
					while (j < currentLogicalTopology.lightPaths.size() && lightPathWithCapacity == false) // verfica se já existem esses lightPaths
					{
						if (src == currentLogicalTopology.lightPaths[j].sourceNode && dst == currentLogicalTopology.lightPaths[j].destinationNode) // existem
						{
							if (currentLogicalTopology.lightPaths[j].capacity >= odu0s) // com capacidade
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
						currentLogicalTopology.lightPaths[lightPathsWithCapacityIndex[i]].capacity -= odu0s;
						currentLogicalTopology.opticalChannels[lightPathsWithCapacityIndex[i]].capacity -= odu0s;
						currentLogicalTopology.opticalChannels[lightPathsWithCapacityIndex[i]].numberOfDemands++;
						currentLogicalTopology.opticalChannels[lightPathsWithCapacityIndex[i]].demandsIndex.push_back(demand.demandIndex);

						//verificar se existem outros paths a usar estes lightPaths e atualizar a capacidade do path com o menor valor 
						for (size_t j = 0; j < currentLogicalTopology.paths.size(); j++)
						{
							for (size_t k = 0; k < currentLogicalTopology.paths[j].lightPathsIndex.size(); k++)
							{
								if (currentLogicalTopology.paths[j].lightPathsIndex[k] == lightPathsWithCapacityIndex[i])
								{
									if (currentLogicalTopology.paths[j].capacity > currentLogicalTopology.lightPaths[lightPathsWithCapacityIndex[i]].capacity)
										currentLogicalTopology.paths[j].capacity = currentLogicalTopology.lightPaths[lightPathsWithCapacityIndex[i]].capacity;
								}
							}
						}

						capacityLightPaths.push_back(currentLogicalTopology.lightPaths[lightPathsWithCapacityIndex[i]].capacity);

					}
					t_paths newPath; // criar novo path
					newPath.pathIndex = currentLogicalTopology.paths.size();
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
					currentLogicalTopology.paths.push_back(newPath);

					processedDemand.demandIndex = demand.demandIndex;
					processedDemand.routed = true;
					processedDemand.pathIndex = newPath.pathIndex;
					outputSignals[1]->bufferPut((t_demand_request_routed)processedDemand);
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
							if (src == currentLogicalTopology.lightPaths[lightPathsWithCapacityIndex[j]].sourceNode
								&& dst == currentLogicalTopology.lightPaths[lightPathsWithCapacityIndex[j]].destinationNode)
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

							outputSignals[2]->bufferPut((t_path_request)pathRequest);
						}
					}
				}
			}
		}
	}

	outputSignals[0]->bufferPut((t_logical_topology)currentLogicalTopology); 
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
