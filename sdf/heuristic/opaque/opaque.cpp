# include "../../include/netxpto_20180815.h"
# include "../../include/scheduler_20190129.h"
# include "../../include/sink_20180815.h"

//##########################################################################################
//######################## Simulation Input Parameters #####################################
//##########################################################################################

int ODU0[6][6] = { {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}};
int ODU1[6][6] = { {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}};
int ODU2[6][6] = { {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}};
int ODU3[6][6] = { {0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}};
int ODU4[6][6] = { {0,0,0,0,0,0},{0,0,0,9,0,0},{0,0,0,0,0,0},{0,9,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}};
int DemandsOrderingCriterion = 0; 

class Demand : public Signal {

public:
	Demand() : Signal() { Signal::setBuffer(); }
	Demand(string fileName) : Signal(fileName) { setBuffer(); }
	Demand(string fileName, t_unsigned_long bLength) : Signal(fileName, bLength) { setBuffer(); }
	Demand(string fileName, string folderName) : Signal(fileName, folderName) { setBuffer(); }
	Demand(string fileName, string folderName, t_unsigned_long bLength) : Signal(fileName, folderName, bLength) { setBuffer(); }
	Demand(t_unsigned_long bLength) : Signal(bLength) { setBuffer(); }

private:
	
	int DemandIndex = 0;
	int SourceNode = 0;
	int DestinationNode = 0;
	int ODUType = 0;
	int RestorationMethod = 0;

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
    Scheduler_.setODU2(ODU3);
    Scheduler_.setODU2(ODU4);
    Scheduler_.setDemandsOrderingCriterion(DemandsOrderingCriterion);

/*
    logicalTopology LogicalTopology{ "LogicalTopology.sgn" };
    LogicalTopologyGenerator LogicalTopologyGenerator_{ {},{LogicalTopology}};
    LogicalTopologyGenerator_.set(TransportMode);
    LogicalTopologyGenerator_.set(PhysicalTopology); 

    physicalTopology PhysicalTopology{ "PhysicalTopology.sgn" };
    PhysicalTopologyGenerator PhysicalTopologyGenerator_{ {},{PhysicalTopology}};
    PhysicalTopologyGenerator_.set(PhysicalTopology); 
    PhysicalTopologyGenerator_.set(OpticalChannels);
    PhysicalTopologyGenerator_.set(TransportSystems);
    PhysicalTopologyGenerator_.set(OpticalChannelCapacity);
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
