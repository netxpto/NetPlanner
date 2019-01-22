# include "../../include/netxpto_20180815.h"

# include "../../include/alice_20180815.h"
# include "../../include/sop_rotator_20180815.h"
# include "../../include/sink_20180815.h"

//##########################################################################################
//######################## Simulation Input Parameters #####################################
//##########################################################################################

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

	demand SchedulerOut{ "SchedulerOut.sgn" }, BlockedDemands{ "BlockedDemands.sgn" }; // Sinais e tipo de sinais
	Scheduler Scheduler_{ {},{ &SchedulerOut} }; // Bloco e sinais de entrada e saída
//	Scheduler_.set*****(1/bitRate);  Variáveis a serem iniciadas

	logicalTopology LogicalTopology{ "LogicalTopology.sgn" };
	Logical Logical_Topology_Generator{ {},{&LogicalTopology} };

	physicalTopology PhysicalTopology{ "PhysicalTopology.sgn" };
	Physical Physical_Topology_Generator{ {},{&PhysicalTopology} };

	demandListOfPaths PathGeneratorOut{ "PathGeneratorOut.sgn" };
	path RemovedPaths{ "RemovedPaths.sgn" };
	Generator Path_Generator{ { &SchedulerOut, &LogicalTopology, &RemovedPaths },{ &PathGeneratorOut} };
	
	demandPathRoute RoutedDemands{ "RoutedDemands.sgn" };
	Tester Path_Tester{ { &PathGeneratorOut, &PhysicalTopology},{ &BlockedDemands, &RoutedDemands} };

	Sink Sink_Blocking{ { &BlockedDemands },{} };
	Sink Sink_Routed{ { &RoutedDemands },{} };

	System MainSystem{
						&Scheduler_,
						&Logical_Topology_Generator,
						&Physical_Topology_Generator,
						&Path_Generator,
						&Path_Tester,
						&Sink_Blocking,
						&Sink_Routed
	};
	
	MainSystem.run();
	MainSystem.terminate();

	system("pause");
	return 0;
}
