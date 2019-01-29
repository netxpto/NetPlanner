# include "..\..\..\include\netxpto_20180815.h"
# include "..\..\..\include\scheduler_20190122.h"
# include "..\..\..\include\sink_20180815.h"


const int NUMBER_OF_NODES = 6;

typedef struct { t_integer matrix[NUMBER_OF_NODES][NUMBER_OF_NODES]; } t_matrix;

class Demand : public Signal {

public:
	Demand() : Signal() { Signal::setBuffer(); }
	Demand(string fileName) : Signal(fileName) { setBuffer(); }
	Demand(string fileName, t_unsigned_long bLength) : Signal(fileName, bLength) { setBuffer(); }
	Demand(string fileName, string folderName) : Signal(fileName, folderName) { setBuffer(); }
	Demand(string fileName, string folderName, t_unsigned_long bLength) : Signal(fileName, folderName, bLength) { setBuffer(); }
	Demand(t_unsigned_long bLength) : Signal(bLength) { setBuffer(); }

private:
	t_integer demandIndex = { 0 };
	t_integer sourceNode = { 0 };
	t_integer destinationNode = { 0 };
	t_integer oduType = { 0 };
	t_integer restorationMethod = { 0 };
};

//##########################################################################################
//######################## Simulation Input Parameters #####################################
//##########################################################################################

// INICIALIZAR TODAS AS MATRIZES DE ODU0s
t_matrix odu0{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} };
t_matrix odu1{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} };
t_matrix odu2{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} };
t_matrix odu3{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} };
t_matrix odu4{ {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0} };
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

	System("pause");
	return 0;
}
