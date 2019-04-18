# include "..\..\..\include\netxpto_20190130.h"
# include "..\..\..\include\scheduler_20190122.h"
# include "..\..\..\include\logical_topology_generator_20190216.h"
# include "..\..\..\include\physical_topology_generator_20190227.h"
# include "..\..\..\include\sink_20180815.h"


//##########################################################################################
//######################## Simulation Input Parameters #####################################
//##########################################################################################

// devera ser feito de acordo com IPS do netxpto

class NetworkSimulatorInputParameters : public SystemInputParameters {
public:

	// INPUT PARAMETERS
	int numberOfNodes{ 0 }; // Number of nodes existent in the network

	/* Initializes default input parameters*/
	NetworkSimulatorInputParameters() : SystemInputParameters() {
		initializeInputParameterMap();
	}

	/* Initializes input parameters according to the program arguments */
	/* Usage: .\network_simulator.exe <InpuParameters.txt> <output_directory>*/
	NetworkSimulatorInputParameters(int argc, char *argv[]) : SystemInputParameters(argc, argv) {
		initializeInputParameterMap();
		readSystemInputParameters();
	}

	// Each parameter must be added to the parameter map by calling this function
	void initializeInputParameterMap() {
		addInputParameter("numberOfNodes", &numberOfNodes);
	}
};


//##########################################################################################
//##########################################################################################
//##########################################################################################


int main(int argc, char *argv[])
{
	NetworkSimulatorInputParameters param(argc, argv);


	/* Signals Declaration */
	Demand Scheduler_Out{"SchedulerOut.sgn", 7 };
	Scheduler_Out.setSaveInAscii(true);

	/*LogicalTopology LogicalTopologyOut{ "LogicalTopologyOut.sgn"};
	LogicalTopologyOut.setSaveInAscii(true);

	PhysicalTopology PhysicalTopologyOut{ "PhysicalTopologyOut.sgn", 1};
	PhysicalTopologyOut.setSaveInAscii(true);*/

	/* Blocks Decalration */
	Scheduler Scheduler_{ {},{ &Scheduler_Out} };
	Scheduler_.setNumberOfNodes(numberOfNodes);
	//Scheduler_.setODU0(odu0);
	//Scheduler_.setODU1(odu1);
	//Scheduler_.setODU2(odu2);
	//Scheduler_.setODU3(odu3);
	//Scheduler_.setODU4(odu4);
	//Scheduler_.setOrderingRule(orderingRule);

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
