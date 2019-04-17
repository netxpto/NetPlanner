# include "..\..\..\include\netxpto_20190130.h"
# include "..\..\..\include\scheduler_20190122.h"
# include "..\..\..\include\logical_topology_generator_20190216.h"
# include "..\..\..\include\physical_topology_generator_20190227.h"
# include "..\..\..\include\sink_20180815.h"

void readImputParametersValuesFile(t_integer &numberOfNodes, t_matrix &odu0, t_matrix &odu1, t_matrix &odu2, t_matrix &odu3, t_matrix &odu4, ordering_rule &orderingRule, t_matrix &physicalTopologyAdjacencyMatrix, transport_mode &transportMode);

//##########################################################################################
//######################## Simulation Input Parameters #####################################
//##########################################################################################

// devera ser feito de acordo com IPS do netxpto

t_integer numberOfNodes; // Number of nodes existent in the network

// Traffic in terms of ODU0s
t_matrix odu0;
t_matrix odu1;
t_matrix odu2;
t_matrix odu3;
t_matrix odu4;

// Demand ordering rule
ordering_rule orderingRule; // default orderingRule value 0 (descendingOrder)


// Physical topology adjacency matrix
t_matrix physicalTopologyAdjacencyMatrix;

// Transport mode
transport_mode transportMode; // default transportMode value

// Optical channels per transport system
t_integer opticalChannels { 5 };// 5 optical channels per transport system

// Transport systems
t_integer transportSystems { 1 }; // 1 transport system between each par of nodes

// Optical channels capacity
t_integer opticalChannelCapacity { 80 }; // In this case each channel supports up to 80 ODU0s

// Criterion
criterion shortestPathType{ criterion::hops }; // By default the paremeter chosen is hops

// Number of paths
t_integer numberOfPaths{ 3 }; // 3 shortest paths are attributed to each demand

//##########################################################################################
//##########################################################################################
//##########################################################################################


int main()
{
	readImputParametersValuesFile(numberOfNodes, odu0, odu1, odu2, odu3, odu4, orderingRule, physicalTopologyAdjacencyMatrix, transportMode);

	/* Signals Declaration */
	Demand Scheduler_Out{ "SchedulerOut.sgn", 7};
	Scheduler_Out.setSaveInAscii(true);

	/*LogicalTopology LogicalTopologyOut{ "LogicalTopologyOut.sgn"};
	LogicalTopologyOut.setSaveInAscii(true);

	PhysicalTopology PhysicalTopologyOut{ "PhysicalTopologyOut.sgn", 1};
	PhysicalTopologyOut.setSaveInAscii(true);*/

	/* Blocks Decalration */
	Scheduler Scheduler_{ {},{ &Scheduler_Out} };
	Scheduler_.setNumberOfNodes(numberOfNodes);
	Scheduler_.setODU0(odu0);
	Scheduler_.setODU1(odu1);
	Scheduler_.setODU2(odu2);
	Scheduler_.setODU3(odu3);
	Scheduler_.setODU4(odu4);
	Scheduler_.setOrderingRule(orderingRule);

	Sink SinkScheduler_{ { &Scheduler_Out },{} };
	SinkScheduler_.setDisplayNumberOfSamples(true);


	//LogicalTopologyGenerator LogicalTopologyGenerator_{ {},{&LogicalTopologyOut} };
	//LogicalTopologyGenerator_.setTransportMode(transportMode);
	//LogicalTopologyGenerator_.setPhysicalTopology(physicalTopology);

	//Sink SinkLogicalTopology_{ { &LogicalTopologyOut },{} };
	//SinkLogicalTopology_.setDisplayNumberOfSamples(true);

	//PhysicalTopologyGenerator PhysicalTopologyGenerator_{ {},{&PhysicalTopologyOut} };
	//PhysicalTopologyGenerator_.setOpticalChannels(opticalChannels);
	//PhysicalTopologyGenerator_.setPhysicalTopology(physicalTopology);
	//PhysicalTopologyGenerator_.setTransportSystems(transportSystems);
	//PhysicalTopologyGenerator_.setOpticalChannelCapacity(opticalChannelCapacity);

	//Sink SinkPhysicalTopology_{ { &PhysicalTopologyOut },{} };
	//SinkPhysicalTopology_.setDisplayNumberOfSamples(true);


	//DemandListOfPahts PathGeneratorOut{ "PathGeneratorOut.sgn",1 };
	//PathGeneratorOut.setSaveInAscii(true);
	//Path RemovedPaths{ "RemovedPaths.sgn" };
	//PathGenerator PathGenerator_{ { &SchedulerOut, &LogicalTopologyOut/*, &RemovedPaths*/ },{ &PathGeneratorOut} };
	//PathGenerator_.setCriterion(shortestPathType);
	//PathGenerator_.setNumberOfPaths(numberOfPaths);

	//Sink SinkPathGenerator_{ { &PhysicalTopologyOut },{} };
	//SinkPathGenerator_.setDisplayNumberOfSamples(true);

/*
		demandPathRoute RoutedDemands{ "RoutedDemands.sgn" };
		demand BlockedDemands{ "BlockedDemands.sgn" };
		Tester Path_Tester_{ { &PathGeneratorOut, &PhysicalTopology},{ &BlockedDemands, &RoutedDemands} };

		Sink Sink_Blocking_{ { &BlockedDemands },{} };
		Sink Sink_Routed_{ { &RoutedDemands },{} };
*/



	//Sink Sink_logical_{ {&LogicalTopologyOut},{} };
    
	System MainSystem{
		// BLOCKS
		&Scheduler_,
		&SinkScheduler_
		//&LogicalTopologyGenerator_,
		//&SinkLogicalTopology_,
		//&PhysicalTopologyGenerator_,
		//&SinkPhysicalTopology_,
		//&PathGenerator_,
		//&SinkPathGenerator_
};
	
	MainSystem.run();
	MainSystem.terminate();

	system("pause");

	return 0;
}


void readImputParametersValuesFile(t_integer &numberOfNodes, t_matrix &odu0, t_matrix &odu1, t_matrix &odu2, t_matrix &odu3, t_matrix &odu4, ordering_rule &orderingRule, t_matrix &physicalTopologyAdjacencyMatrix, transport_mode &transportMode)
{
	ifstream inFile;
	inFile.open("InputParameters.txt", ios::in);  //opening inputtxt file 
	string line;

	// ################### Entry variable names ##################################

	string s1 = "numberOfNodes";
	string s2 = "odu0";
	string s3 = "odu1";
	string s4 = "odu2";
	string s5 = "odu3";
	string s6 = "odu4";
	string s7 = "orderingRule";
	string s8 = "physicalTopologyAdjacencyMatrix";
	string s9 = "transportMode";

	//###########################################################################
	string evaluateVariable;

	int vectorValue;
	string variableValue;
	vector<int> oduValues;
	string ordering;
	string transport;

	if (!inFile.fail())
	{
		// for each line in the file 
		while (getline(inFile, line))
		{
			if (!(line[0] == '#')) // ignore if it is commented
			{
				stringstream s(line);
				getline(s, evaluateVariable, ':');
				size_t foundS1 = evaluateVariable.find(s1);
				size_t foundS2 = evaluateVariable.find(s2);
				size_t foundS3 = evaluateVariable.find(s3);
				size_t foundS4 = evaluateVariable.find(s4);
				size_t foundS5 = evaluateVariable.find(s5);
				size_t foundS6 = evaluateVariable.find(s6);
				size_t foundS7 = evaluateVariable.find(s7);
				size_t foundS8 = evaluateVariable.find(s8);
				size_t foundS9 = evaluateVariable.find(s9);

				if (foundS1 != string::npos)
				{
					getline(s, variableValue);
					numberOfNodes = stoi(variableValue);
				}
				if (foundS2 != string::npos)
				{
					for (auto j = 0; j < numberOfNodes; j++)
					{
						for (auto k = 0; k < numberOfNodes - 1; k++) // search through a line
						{
							getline(s, variableValue, ',');
							vectorValue = stoi(variableValue);
							oduValues.push_back(vectorValue);
						}

						getline(s, variableValue, ';');
						vectorValue = stoi(variableValue);
						oduValues.push_back(vectorValue);

						odu0.push_back(oduValues);
						oduValues.clear();
					}

				}
				if (foundS3 != string::npos)
				{
					for (auto j = 0; j < numberOfNodes; j++)
					{
						for (auto k = 0; k < numberOfNodes - 1; k++) // search through a line
						{
							getline(s, variableValue, ',');
							vectorValue = stoi(variableValue);
							oduValues.push_back(vectorValue);
						}

						getline(s, variableValue, ';');
						vectorValue = stoi(variableValue);
						oduValues.push_back(vectorValue);

						odu1.push_back(oduValues);
						oduValues.clear();
					}

				}
				if (foundS4 != string::npos)
				{
					for (auto j = 0; j < numberOfNodes; j++)
					{
						for (auto k = 0; k < numberOfNodes - 1; k++) // search through a line
						{
							getline(s, variableValue, ',');
							vectorValue = stoi(variableValue);
							oduValues.push_back(vectorValue);
						}

						getline(s, variableValue, ';');
						vectorValue = stoi(variableValue);
						oduValues.push_back(vectorValue);

						odu2.push_back(oduValues);
						oduValues.clear();
					}

				}
				if (foundS5 != string::npos)
				{
					for (auto j = 0; j < numberOfNodes; j++)
					{
						for (auto k = 0; k < numberOfNodes - 1; k++) // search through a line
						{
							getline(s, variableValue, ',');
							vectorValue = stoi(variableValue);
							oduValues.push_back(vectorValue);
						}

						getline(s, variableValue, ';');
						vectorValue = stoi(variableValue);
						oduValues.push_back(vectorValue);

						odu3.push_back(oduValues);
						oduValues.clear();
					}

				}
				if (foundS6 != string::npos)
				{
					for (auto j = 0; j < numberOfNodes; j++)
					{
						for (auto k = 0; k < numberOfNodes - 1; k++) // search through a line
						{
							getline(s, variableValue, ',');
							vectorValue = stoi(variableValue);
							oduValues.push_back(vectorValue);
						}

						getline(s, variableValue, ';');
						vectorValue = stoi(variableValue);
						oduValues.push_back(vectorValue);

						odu4.push_back(oduValues);
						oduValues.clear();
					}

				}
				if (foundS7 != string::npos)
				{
					getline(s, ordering);

					if (ordering == "descendingOrder")
					{
						orderingRule = ordering_rule::descendingOrder;
					}
					else if (ordering == "ascendingOrder")
					{
						orderingRule = ordering_rule::ascendingOrder;
					}
				}
				if (foundS8 != string::npos)
				{
					for (auto j = 0; j < numberOfNodes; j++)
					{
						for (auto k = 0; k < numberOfNodes - 1; k++) // search through a line
						{
							getline(s, variableValue, ',');
							vectorValue = stoi(variableValue);
							oduValues.push_back(vectorValue);
						}

						getline(s, variableValue, ';');
						vectorValue = stoi(variableValue);
						oduValues.push_back(vectorValue);

						physicalTopologyAdjacencyMatrix.push_back(oduValues);
						oduValues.clear();
					}

				}
				if (foundS9 != string::npos)
				{
					getline(s, transport);

					if (transport == "opaque")
					{
						transportMode = transport_mode::opaque;
					}
					else if (transport == "transparent")
					{
						transportMode = transport_mode::transparent;
					}
					else if (transport == "translucent")
					{
						transportMode = transport_mode::translucent;
					}
				}

			}
		}
		// PRINT VARIABLES
		cout << "numberOfNodes: " << variableValue << endl;

		cout << "odu0 demands" << endl;

		for (auto line = 0; line < numberOfNodes; line++)
		{
			for (auto column = 0; column < numberOfNodes; column++)
			{
				cout << odu0[line][column];
			}
			cout << endl;
		}
		cout << "odu1 demands" << endl;

		for (auto line = 0; line < numberOfNodes; line++)
		{
			for (auto column = 0; column < numberOfNodes; column++)
			{
				cout << odu1[line][column];
			}
			cout << endl;
		}
		cout << "odu2 demands" << endl;

		for (auto line = 0; line < numberOfNodes; line++)
		{
			for (auto column = 0; column < numberOfNodes; column++)
			{
				cout << odu2[line][column];
			}
			cout << endl;
		}
		cout << "odu3 demands" << endl;

		for (auto line = 0; line < numberOfNodes; line++)
		{
			for (auto column = 0; column < numberOfNodes; column++)
			{
				cout << odu3[line][column];
			}
			cout << endl;
		}
		cout << "odu4 demands" << endl;

		for (auto line = 0; line < numberOfNodes; line++)
		{
			for (auto column = 0; column < numberOfNodes; column++)
			{
				cout << odu4[line][column];
			}
			cout << endl;
		}
		cout << "orderingRule: " << ordering << endl;

		cout << "physicalTopologyAdjacencyMatrix" << endl;

		for (auto line = 0; line < numberOfNodes; line++)
		{
			for (auto column = 0; column < numberOfNodes; column++)
			{
				cout << physicalTopologyAdjacencyMatrix[line][column];
			}
			cout << endl;
		}
		cout << "transportMode: " << transport << endl;

	}
	else
	{
		cerr << "Error opening IputParameters.txt file" << endl;
		system("pause");
		exit(1);
	}
}