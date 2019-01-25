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

	
};

//##########################################################################################
//######################## Simulation Input Parameters #####################################
//##########################################################################################

// INICIALIZAR TODAS AS MATRIZES DE ODU0s
t_matrix ODU0{};
t_matrix ODU1{};
t_matrix ODU2{};
t_matrix ODU3{};
t_matrix ODU4{};
t_integer OrderingRule{ 0 };

//##########################################################################################
//##########################################################################################
//##########################################################################################

int main()
{

	Demand SchedulerOut{ "SchedulerOut.sgn" };
	Scheduler Scheduler_{ {},{ &SchedulerOut} };
	Scheduler_.setODU0(ODU0);
	Scheduler_.setODU1(ODU1);
	Scheduler_.setODU2(ODU2);
	Scheduler_.setODU3(ODU3);
	Scheduler_.setODU4(ODU4);
	Scheduler_.setDemandsOrderingRule(OrderingRule);

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

	system("pause");
	return 0;
}
