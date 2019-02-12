# include "..\..\..\include\netxpto_20190130.h"
# include "..\..\..\include\scheduler_20190122.h"
# include "..\..\..\include\sink_20180815.h"



//##########################################################################################
//######################## Simulation Input Parameters #####################################
//##########################################################################################

// Traffic
t_matrix odu0(6, std::vector<int>(6, 1));
t_matrix odu1(6, std::vector<int>(6, 1));
t_matrix odu2(6, std::vector<int>(6, 1));
t_matrix odu3(6, std::vector<int>(6, 1));
t_matrix odu4(6, std::vector<int>(6, 1));

/*t_matrix odu0{	{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0} };
t_matrix odu1{ {0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0} };
t_matrix odu2{ {0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0} };
t_matrix odu3{ {0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0} };
t_matrix odu4{ {0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0} };
				*/
/*t_matrix odu2{	std::vector<int>{0,0,0,0,0,0},
				std::vector<int>{0,0,0,0,0,0},
				std::vector<int>{0,0,0,0,0,0},
				std::vector<int>{0,0,0,0,0,0},
				std::vector<int>{0,0,0,0,0,0},
				std::vector<int>{0,0,0,0,0,0} };

t_matrix odu3{	std::vector<int>{0,0,0,0,0,0},
				std::vector<int>{0,0,0,0,0,0},
				std::vector<int>{0,0,0,0,0,0},
				std::vector<int>{0,0,0,0,0,0},
				std::vector<int>{0,0,0,0,0,0},
				std::vector<int>{0,0,0,0,0,0} };

t_matrix odu4{	std::vector<int>{1,0,0,0,0,0},
				std::vector<int>{0,0,0,0,0,0},
				std::vector<int>{0,0,0,0,0,0},
				std::vector<int>{0,0,0,0,0,0},
				std::vector<int>{0,0,0,0,0,0},
				std::vector<int>{0,0,0,0,0,0} };
*/
// Demand ordering rule
t_integer orderingRule{ 0 };

//##########################################################################################
//##########################################################################################
//##########################################################################################

int main()
{

	Demand SchedulerOut{ "SchedulerOut.sgn" };
	Scheduler Scheduler_{ {},{ &SchedulerOut} };
	Scheduler_.setODU0(odu0);
	Scheduler_.setODU1(odu1);
	Scheduler_.setODU2(odu2);
	Scheduler_.setODU3(odu3);
	Scheduler_.setODU4(odu4);
	Scheduler_.setDemandsOrderingRule(orderingRule);

	/*
		logicalTopology LogicalTopologyOut{ "LogicalTopologyOut.sgn" };
		Logical_Topology_Generator Logical_Topology_Generator_{ {},{&LogicalTopologyOut} };
			Logical_Topology_Generator_.setTransportMode(transparent);


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

    
	System MainSystem{
			// BLOCKS
			&Scheduler_,
            &Sink_
	};
	
	MainSystem.run();
	MainSystem.terminate();

	return 0;
}
