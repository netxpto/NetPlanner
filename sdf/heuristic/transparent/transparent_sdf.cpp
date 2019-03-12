# include "..\..\..\include\netxpto_20190130.h"
# include "..\..\..\include\scheduler_20190122.h"
# include "..\..\..\include\sink_20180815.h"
# include "..\..\..\include\logical_topology_generator_20190216.h"
# include "..\..\..\include\physical_topology_generator_20190227.h"
# include "..\..\..\include\path_generator_20190302.h"



//##########################################################################################
//######################## Simulation Input Parameters #####################################
//##########################################################################################

// Traffic in terms of ODU0s

t_matrix odu0{	{0,1,1,1,1,1},
				{1,0,1,1,1,1},
				{1,1,0,1,1,1},
				{1,1,1,0,1,1},
				{1,1,1,1,0,1},
				{1,1,1,1,1,0} };

t_matrix odu1{	{0,1,1,1,1,1},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0} };

t_matrix odu2{	{0,1,1,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0} };

t_matrix odu3{	{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0} };

t_matrix odu4{	{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,1,1,0} };
				
// Demand ordering rule
t_integer orderingRule{ 0 };

// Physical topology
t_matrix physicalTopology{  {0,1,0,0,0,1},
							{1,0,1,0,0,1},
							{0,1,0,1,1,0},
							{0,0,1,0,1,0},
							{0,0,1,1,0,1},
							{1,1,0,0,1,0} };

// Transport mode
transport_mode transportMode{transport_mode::transparent};

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

	/* Signals Declaration */
	Demand SchedulerOut{ "SchedulerOut.sgn",84};
	SchedulerOut.setSaveInAscii(true);

	LogicalTopology LogicalTopologyOut{ "LogicalTopologyOut.sgn"};
	LogicalTopologyOut.setSaveInAscii(true);

	PhysicalTopology PhysicalTopologyOut{ "PhysicalTopologyOut.sgn", 1};
	PhysicalTopologyOut.setSaveInAscii(true);

	/* Blocks Decalration */
	Scheduler Scheduler_{ {},{ &SchedulerOut} };
	Scheduler_.setODU0(odu0);
	Scheduler_.setODU1(odu1);
	Scheduler_.setODU2(odu2);
	Scheduler_.setODU3(odu3);
	Scheduler_.setODU4(odu4);
	Scheduler_.setDemandsOrderingRule(orderingRule);

	//Sink SinkScheduler_{ { &SchedulerOut },{} };
	//SinkScheduler_.setDisplayNumberOfSamples(true);


	LogicalTopologyGenerator LogicalTopologyGenerator_{ {},{&LogicalTopologyOut} };
	LogicalTopologyGenerator_.setTransportMode(transportMode);
	LogicalTopologyGenerator_.setPhysicalTopology(physicalTopology);

	//Sink SinkLogicalTopology_{ { &LogicalTopologyOut },{} };
	//SinkLogicalTopology_.setDisplayNumberOfSamples(true);

	PhysicalTopologyGenerator PhysicalTopologyGenerator_{ {},{&PhysicalTopologyOut} };
	PhysicalTopologyGenerator_.setOpticalChannels(opticalChannels);
	PhysicalTopologyGenerator_.setPhysicalTopology(physicalTopology);
	PhysicalTopologyGenerator_.setTransportSystems(transportSystems);
	PhysicalTopologyGenerator_.setOpticalChannelCapacity(opticalChannelCapacity);

	//Sink SinkPhysicalTopology_{ { &PhysicalTopologyOut },{} };
	//SinkPhysicalTopology_.setDisplayNumberOfSamples(true);


	DemandListOfPahts PathGeneratorOut{ "PathGeneratorOut.sgn",1 };
	PathGeneratorOut.setSaveInAscii(true);
	//Path RemovedPaths{ "RemovedPaths.sgn" };
	PathGenerator PathGenerator_{ { &SchedulerOut, &LogicalTopologyOut/*, &RemovedPaths*/ },{ &PathGeneratorOut} };
	PathGenerator_.setCriterion(shortestPathType);
	PathGenerator_.setNumberOfPaths(numberOfPaths);

	Sink SinkPathGenerator_{ { &PhysicalTopologyOut },{} };
	SinkPathGenerator_.setDisplayNumberOfSamples(true);

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
		//&SinkScheduler_,
		&LogicalTopologyGenerator_,
		//&SinkLogicalTopology_,
		&PhysicalTopologyGenerator_,
		//&SinkPhysicalTopology_,
		&PathGenerator_,
		&SinkPathGenerator_
};
	
	MainSystem.run();
	MainSystem.terminate();

	system("pause");

	return 0;
}
