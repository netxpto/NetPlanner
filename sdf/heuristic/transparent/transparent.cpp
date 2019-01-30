# include "../../include/netxpto_20180815.h"
# include "../../include/scheduler_20190122.h"
# include "../../include/sink_20180815.h"

//##########################################################################################
//######################## Simulation Input Parameters #####################################
//##########################################################################################

tipo_matrix ODU0{};

/*
t_integer numberOfSymbols{ 10000 };
t_real bitRate{ 1.0e3 };
t_integer samplesPerSymbol{ 16 };
bool saveInternalSignals{ true };
sop_rotation_type sopRotationType{ sop_rotation_type::Deterministic };
*/

//##########################################################################################
//##########################################################################################
//##########################################################################################

int main()
{

	demand SchedulerOut{ "SchedulerOut.sgn" };
	Scheduler Scheduler_{ {},{ &SchedulerOut} }; 
	Scheduler_.setODU0(ODU0);
    Scheduler_.setODU1(ODU1); 
    Scheduler_.setODU2(ODU2); 

/*
	logicalTopology LogicalTopologyOut{ "LogicalTopologyOut.sgn" };
	Logical_Topology_Generator Logical_Topology_Generator_{ {},{&LogicalTopologyOut} };
    Logical_Topology_Generator_.setTransportMode(transparent);
    

	physicalTopology PhysicalTopology{ "PhysicalTopology.sgn" };
	Physical Physical_Topology_Generator{ {},{&PhysicalTopology} };

, BlockedDemands{ "BlockedDemands.sgn" }; // Sinais e tipo de sinais
	demandListOfPaths PathGeneratorOut{ "PathGeneratorOut.sgn" };
	path RemovedPaths{ "RemovedPaths.sgn" };
	Generator Path_Generator{ { &SchedulerOut, &LogicalTopology, &RemovedPaths },{ &PathGeneratorOut} };
	
	demandPathRoute RoutedDemands{ "RoutedDemands.sgn" };
	Tester Path_Tester{ { &PathGeneratorOut, &PhysicalTopology},{ &BlockedDemands, &RoutedDemands} };

	Sink Sink_Blocking{ { &BlockedDemands },{} };
	Sink Sink_Routed{ { &RoutedDemands },{} };
*/
    Sink Sink_{{&SchedulerOut},{}}
    
	System MainSystem{
						&Scheduler_,
                        &Sink_
	};
	
	MainSystem.run();
	MainSystem.terminate();

	system("pause");
	return 0;
}
