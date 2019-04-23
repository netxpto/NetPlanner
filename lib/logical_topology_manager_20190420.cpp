# include "..\include\logical_topology_manager_20190420.h"


using namespace std;

void LogicalTopologyManager::initialize(void) {
	
}

bool LogicalTopologyManager::runBlock(void) {

	int process = inputSignals[0]->ready(); // number of samples in the buffer ready to be processed
	int process2 = inputSignals[1]->ready();

	if (process == 0 && process2 == 0) return false;

	if (process != 0)
	{
		for (int i = 0; i < process; i++) {
			inputSignals[0]->bufferGet(&currentLogicalTopology);
		}
		//outputSignals[0]->bufferPut((t_logical_topology)currentLogicalTopology);
	}

	int space = outputSignals[1]->space();	// Buffer free space 

	process2 = std::min(space, process2);
	

	if (process2 != 0)
	{
		for (int i = 0; i < process2; i++) {
			inputSignals[1]->bufferGet(&demand); // Reads a demand
			int oduType = demand.oduType;
			bool pathFound{ false };
			
			//cout << demandProcessed.demandIndex << endl;
			//outputSignals[1]->bufferPut((t_demand)demandProcessed);

			for (int numberOfPaths = 0; numberOfPaths < currentLogicalTopology.paths.size(); numberOfPaths++) // Searchs list of existing pahts
			{
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

							}
							break;
						case 4:
							if (currentLogicalTopology.paths[numberOfPaths].capacity >= 32)
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

							}
							break;
						default:
							break;
							
						}
					}
				}
			}

			if (pathFound == false)
			{
				// try to create a new path to route the demand
			}
			outputSignals[1]->bufferPut((t_demand)demand);
			outputSignals[0]->bufferPut((t_logical_topology)currentLogicalTopology);
		}
	}
	return true;
};