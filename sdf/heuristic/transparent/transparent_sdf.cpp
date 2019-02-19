# include "..\..\..\include\netxpto_20190130.h"
# include "..\..\..\include\scheduler_20190122.h"
# include "..\..\..\include\sink_20180815.h"
# include "..\..\..\include\logical_top_Gen_20190216.h"



//##########################################################################################
//######################## Simulation Input Parameters #####################################
//##########################################################################################

// Traffic
t_matrix odu0{	{0,1,1,1,1,1},
				{1,0,1,1,1,1},
				{1,1,0,1,1,1},
				{1,1,1,0,1,1},
				{1,1,1,1,0,1},
				{1,1,1,1,1,0} };

t_matrix odu1{	{0,1,1,1,1,1},
				{1,0,0,0,0,0},
				{1,0,0,0,0,0},
				{1,0,0,0,0,0},
				{1,0,0,0,0,0},
				{1,0,0,0,0,0} };

t_matrix odu2{	{0,1,1,1,0,3},
				{1,0,0,0,0,0},
				{1,5,0,0,0,0},
				{1,5,0,0,0,0},
				{1,0,0,0,0,0},
				{3,0,0,0,0,0} };

t_matrix odu3{	{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0} };

t_matrix odu4{	{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,10},
				{0,0,0,0,0,0},
				{0,0,0,10,1,0} };
				
// Demand ordering rule
t_integer orderingRule{ 1 };

// Physical topology
t_matrix physicalTopology{ {0,1,0,0,0,1},
						   {1,0,1,0,0,1},
						   {0,1,0,1,1,0},
						   {0,0,1,0,1,0},
						   {0,0,1,1,0,1},
						   {1,1,0,0,1,0} };

// Transport mode
std::string transportMode{"transparent"};

//##########################################################################################
//##########################################################################################
//##########################################################################################

int main()
{

	Demand SchedulerOut{ "SchedulerOut.sgn", 10};
	SchedulerOut.setSaveInAscii(true);
	Scheduler Scheduler_{ {},{ &SchedulerOut} };
	Scheduler_.setODU0(odu0);
	Scheduler_.setODU1(odu1);
	Scheduler_.setODU2(odu2);
	Scheduler_.setODU3(odu3);
	Scheduler_.setODU4(odu4);
	Scheduler_.setDemandsOrderingRule(orderingRule);

	
/*	logicalTopology LogicalTopologyOut{ "LogicalTopologyOut.sgn", 32 };
	Logical_Topology_Generator Logical_Topology_Generator_{ {},{&LogicalTopologyOut} };
	Logical_Topology_Generator_.setTransportMode(transportMode);
	Logical_Topology_Generator_.setPhysicalTopology(physicalTopology);


		physicalTopology PhysicalTopologyOut{ "PhysicalTopologyOut.sgn" };
		Physical Physical_Topology_Generator_{ {},{&PhysicalTopology} };


		demandListOfPaths PathGeneratorOut{ "PathGeneratorOut.sgn" };
		path RemovedPaths{ "RemovedPaths.sgn" };
		Generator Path_Generator_{ { &SchedulerOut, &LogicalTopology, &RemovedPaths },{ &PathGeneratorOut} };

		demandPathRoute RoutedDemands{ "RoutedDemands.sgn" };
		demand BlockedDemands{ "BlockedDemands.sgn" };
		Tester Path_Tester_{ { &PathGeneratorOut, &PhysicalTopology},{ &BlockedDemands, &RoutedDemands} };

		Sink Sink_Blocking_{ { &BlockedDemands },{} };
		Sink Sink_Routed_{ { &RoutedDemands },{} };
	*/

	Sink Sink_{ {&SchedulerOut},{} };
	Sink_.setDisplayNumberOfSamples(true);

	//Sink Sink_logical_{ {&LogicalTopologyOut},{} };
    
	System MainSystem{
			// BLOCKS
			&Scheduler_,
			//&Logical_Topology_Generator_
			&Sink_,
			//&Sink_logical_
			
	};
	
	MainSystem.run();
	MainSystem.terminate();

	system("pause");

	return 0;
}
