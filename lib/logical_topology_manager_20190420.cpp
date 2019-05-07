# include "..\include\logical_topology_manager_20190420.h"
# include "..\include\Dijkstra_20190503.h"

using namespace std;


void LogicalTopologyManager::initialize(void) {
	
}

bool LogicalTopologyManager::runBlock(void) {
	
	bool transparent{ true };

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

			

			// Verify Transport Mode
			for (int line = 0; line < currentLogicalTopology.logicalTopologyAdjacencyMatrix.size(); line++)
			{
				for (int column = 0; column < currentLogicalTopology.logicalTopologyAdjacencyMatrix[0].size(); column++)
				{
					if (line != column)
					{
						if (currentLogicalTopology.logicalTopologyAdjacencyMatrix[line][column] == 0)
							transparent = false;
					}
				}
			}
			if (transparent)
				cout << "transparent" << endl;
			else
				cout << "opaque" << endl;
		}
		//outputSignals[0]->bufferPut((t_logical_topology)currentLogicalTopology);
	}

	
	if (process3 != 0) // Process PathRequestRouted signals
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
					logicalTopology_odu2[pathRequestRouted.lightPathsTable[0].sourceNode-1][pathRequestRouted.lightPathsTable[0].destinationNode-1] = 0;
				// direct logical connection is cutted off for demands of the same ODU type (3)
				else if (pathRequestRouted.pathInformation.oduType == 3)
					logicalTopology_odu3[pathRequestRouted.lightPathsTable[0].sourceNode-1][pathRequestRouted.lightPathsTable[0].destinationNode-1] = 0;
				// direct logical connection is cutted off for demands of the same ODU type (4)
				else if (pathRequestRouted.pathInformation.oduType == 4)
					logicalTopology_odu4[pathRequestRouted.lightPathsTable[0].sourceNode-1][pathRequestRouted.lightPathsTable[0].destinationNode-1] = 0;

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
						newOpticalChannel.wavelenght = pathRequestRouted.lightPathsTable[0].wavelenght;
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
						newOpticalChannel.wavelenght = pathRequestRouted.lightPathsTable[0].wavelenght;
						newOpticalChannel.numberOfDemands = 1;
						newOpticalChannel.demandsIndex.push_back(pathRequestRouted.pathInformation.demandIndex);
						

						ochsIndex.push_back(newOpticalChannel.opticalChannelIndex);
					}
					else if(i > 0 && numberOfOpticalChannels > 1 && i < (numberOfOpticalChannels-1))
					{
						newOpticalChannel.opticalChannelIndex = currentLogicalTopology.opticalChannels.size();
						newOpticalChannel.sourceNode = pathRequestRouted.lightPathsTable[0].intermediateNodes[i-1];
						newOpticalChannel.destinationNode = pathRequestRouted.lightPathsTable[0].intermediateNodes[i];
						newOpticalChannel.capacity = 80 - capacity;
						newOpticalChannel.wavelenght = pathRequestRouted.lightPathsTable[0].wavelenght;
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
						newOpticalChannel.wavelenght = pathRequestRouted.lightPathsTable[0].wavelenght;
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
				newPath.lightPathsIndex.push_back(currentLogicalTopology.lightPaths.size()-1);

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
	

//	int space = inputSignals[1]->space();	// Buffer free space 

//	process2 = std::min(space, process2);

	if (process2 != 0)
	{
		for (int i = 0; i < process2; i++) {
			inputSignals[1]->bufferGet(&demand); // Reads a demand
			int oduType = demand.oduType;
			bool pathFound{false};
			
			//cout << demandProcessed.demandIndex << endl;
			//outputSignals[1]->bufferPut((t_demand)demandProcessed);

			int numberOfPaths{ 0 };
			while(numberOfPaths < currentLogicalTopology.paths.size() && pathFound == false) {
			//for (int numberOfPaths = 0; numberOfPaths < currentLogicalTopology.paths.size(); numberOfPaths++) // Searchs list of existing pahts
			
				//if (pathFound == true)
					
				//break;
				//pathFound = false;

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
					//If the direct logical connection between source and destination nodes is still available
					

					/*pathRequest.requestIndex = requestIndex;
					pathRequest.demandIndex = demand.demandIndex;
					pathRequest.sourceNode = demand.sourceNode;
					pathRequest.destinationNode = demand.destinationNode;
					pathRequest.intermediateNodes = -1;
					requestIndex++;*/
					if (logicalTopology_odu0[demand.sourceNode - 1][demand.destinationNode - 1] == 1)
					{
						
						Graph g(logicalTopology_odu0[0].size());
						for (size_t line = 0; line < logicalTopology_odu0[0].size(); line++)
						{
							for (size_t column = 0; column < logicalTopology_odu0.size(); column++)
							{
								if (logicalTopology_odu0[line][column] == 1)
									g.addEdge(line, column);
							}
						}

						int sourceNode = demand.sourceNode - 1;
						int destinationNode = demand.destinationNode - 1;
						g.printAllPaths(sourceNode, destinationNode);
						std::vector<vector<int>> possiblePaths = g.printFinalPaths(getBlockingCriterionLogicalTopology());
						

						pathRequest.requestIndex = requestIndex;
						pathRequest.demandIndex = demand.demandIndex;
						pathRequest.oduType = demand.oduType;
						pathRequest.sourceNode = demand.sourceNode;
						pathRequest.destinationNode = demand.destinationNode;
						for (size_t i = 1; i < possiblePaths[0].size() - 1; i++)
						{
							pathRequest.intermediateNodes.push_back(possiblePaths[0][i] + 1);
						}
						if (pathRequest.intermediateNodes.size() == 0)
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
						Graph g(logicalTopology_odu1[0].size());
						for (size_t line = 0; line < logicalTopology_odu1[0].size(); line++)
						{
							for (size_t column = 0; column < logicalTopology_odu1.size(); column++)
							{
								if (logicalTopology_odu1[line][column] == 1)
									g.addEdge(line, column);
							}
						}

						int sourceNode = demand.sourceNode - 1;
						int destinationNode = demand.destinationNode - 1;
						g.printAllPaths(sourceNode, destinationNode);
						std::vector<vector<int>> possiblePaths = g.printFinalPaths(getBlockingCriterionLogicalTopology());

						pathRequest.requestIndex = requestIndex;
						pathRequest.demandIndex = demand.demandIndex;
						pathRequest.oduType = demand.oduType;
						pathRequest.sourceNode = demand.sourceNode;
						pathRequest.destinationNode = demand.destinationNode;
						for (size_t i = 1; i < possiblePaths[0].size() - 1; i++)
						{
							pathRequest.intermediateNodes.push_back(possiblePaths[0][i] + 1);
						}
						if (pathRequest.intermediateNodes.size() == 0)
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
						Graph g(logicalTopology_odu2[0].size());
						for (size_t line = 0; line < logicalTopology_odu2[0].size(); line++)
						{
							for (size_t column = 0; column < logicalTopology_odu2.size(); column++)
							{
								if (logicalTopology_odu2[line][column] == 1)
									g.addEdge(line, column);
							}
						}

						int sourceNode = demand.sourceNode - 1;
						int destinationNode = demand.destinationNode - 1;
						g.printAllPaths(sourceNode, destinationNode);
						std::vector<vector<int>> possiblePaths = g.printFinalPaths(getBlockingCriterionLogicalTopology());

						pathRequest.requestIndex = requestIndex;
						pathRequest.demandIndex = demand.demandIndex;
						pathRequest.oduType = demand.oduType;
						pathRequest.sourceNode = demand.sourceNode;
						pathRequest.destinationNode = demand.destinationNode;
						for (size_t i = 1; i < possiblePaths[0].size() - 1; i++)
						{
							pathRequest.intermediateNodes.push_back(possiblePaths[0][i] + 1);
						}
						if (pathRequest.intermediateNodes.size() == 0)
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
						Graph g(logicalTopology_odu3[0].size());
						for (size_t line = 0; line < logicalTopology_odu3[0].size(); line++)
						{
							for (size_t column = 0; column < logicalTopology_odu3.size(); column++)
							{
								if (logicalTopology_odu3[line][column] == 1)
									g.addEdge(line, column);
							}
						}

						int sourceNode = demand.sourceNode - 1;
						int destinationNode = demand.destinationNode - 1;
						g.printAllPaths(sourceNode, destinationNode);
						std::vector<vector<int>> possiblePaths = g.printFinalPaths(getBlockingCriterionLogicalTopology());

						pathRequest.requestIndex = requestIndex;
						pathRequest.demandIndex = demand.demandIndex;
						pathRequest.oduType = demand.oduType;
						pathRequest.sourceNode = demand.sourceNode;
						pathRequest.destinationNode = demand.destinationNode;
						for (size_t i = 1; i < possiblePaths[0].size() - 1; i++)
						{
							pathRequest.intermediateNodes.push_back(possiblePaths[0][i] + 1);
						}
						if (pathRequest.intermediateNodes.size() == 0)
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
						Graph g(logicalTopology_odu4[0].size());
						for (size_t line = 0; line < logicalTopology_odu4[0].size(); line++)
						{
							for (size_t column = 0; column < logicalTopology_odu4.size(); column++)
							{
								if (logicalTopology_odu4[line][column] == 1)
									g.addEdge(line, column);
							}
						}

						int sourceNode = demand.sourceNode - 1;
						int destinationNode = demand.destinationNode - 1;
						g.printAllPaths(sourceNode, destinationNode);
						std::vector<vector<int>> possiblePaths = g.printFinalPaths(getBlockingCriterionLogicalTopology());
						
						
						if (transparent)
						{
							for (int i = 0; i < possiblePaths.size(); i++)
							{
								if (possiblePaths[i].size() > 2)
								{
									possiblePaths.erase(possiblePaths.begin()+i, possiblePaths.begin()+ (possiblePaths.size()));
									break;
								}
							}
						}
	

						pathRequest.requestIndex = requestIndex;
						pathRequest.demandIndex = demand.demandIndex;
						pathRequest.oduType = demand.oduType;
						pathRequest.sourceNode = demand.sourceNode;
						pathRequest.destinationNode = demand.destinationNode;
						for (size_t i = 1; i < possiblePaths[0].size() - 1; i++)
						{
							pathRequest.intermediateNodes.push_back(possiblePaths[0][i] + 1);
						}
						if (pathRequest.intermediateNodes.size() == 0)
							pathRequest.intermediateNodes.push_back(-1);
						requestIndex++;
						outputSignals[2]->bufferPut((t_path_request)pathRequest); // Send a PathRequest to the PhysicalTopologyManager_ block
						pathRequest.intermediateNodes.clear();
					}
				}
					
				}
			
			//outputSignals[1]->bufferPut((t_demand)demand);
			
		}
	}
	outputSignals[0]->bufferPut((t_logical_topology)currentLogicalTopology); // Prints the final logical topology after processing all the demands in the buffer
	return true;
};