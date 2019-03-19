#include "..\..\..\include_opaque\netxpto_20190130.h"
#include "..\..\..\include_opaque\scheduler_20190122.h"
#include "..\..\..\include_opaque\sink_20180815.h"
#include "..\..\..\include_opaque\logical_topology_generator_20190216.h"
#include "..\..\..\include_opaque\physical_topology_generator_20190221.h"


//##########################################################################################
//######################## Simulation Input Parameters #####################################
//##########################################################################################

// Traffic matrices
t_matrix odu0{	{0,5,1,3,1,3},
				{5,0,0,1,5,0},
				{1,0,0,1,4,1},
				{3,1,1,0,1,1},
				{1,5,4,1,0,3},
				{3,0,1,1,3,0} };

t_matrix odu1{	{0,2,4,2,0,5},
				{2,0,0,3,1,1},
				{4,0,0,1,1,0},
				{2,3,1,0,1,3},
				{0,1,1,1,0,1},
				{5,1,0,3,1,0} };

t_matrix odu2{	{0,1,1,1,0,0},
				{1,0,0,0,1,0},
				{1,0,0,1,1,0},
				{1,0,1,0,1,0},
				{0,1,1,1,0,1},
				{0,0,0,0,1,0} };

t_matrix odu3{	{0,0,0,0,0,0},
				{0,0,1,0,0,1},
				{0,1,0,0,1,0},
				{0,0,0,0,0,0},
				{0,0,1,0,0,0},
				{0,1,0,0,0,0} };

t_matrix odu4{	{0,0,0,0,0,0},
				{0,0,0,0,0,1},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,1},
				{0,1,0,0,1,0} };
				
// Demands odering rule
t_integer orderingRule{ 0 };

// Transport mode
std::string transportMode{ "opaque" };

// Adjacency matrix of the physical network 
t_matrix adjacencyMatrix{ {0,1,1,0,0,0},
						  {1,0,1,1,0,0},
						  {1,1,0,0,1,0},
						  {0,1,0,0,1,1},
						  {0,0,1,1,0,1},
						  {0,0,0,1,1,0} };

// Number of transmission systems
t_integer transmissionSystems{ 1 };

// Number of optical channels per link
t_integer opticalChannels{ 4 };

// Capacity of each optical channel in ODU0s
t_integer opticalChannelCapacity{ 80 };


//##########################################################################################
//##########################################################################################
//##########################################################################################

int main() {

	/* Signals Declaration */
	Demand SchedulerOut{ "SchedulerOut.sgn", 10};
	SchedulerOut.setSaveInAscii(true);
	
	LogicalTopology LogicalTopologyOut{ "LogicalTopologyOut.sgn", 32 };
	LogicalTopologyOut.setSaveInAscii(true);

	PhysicalTopology PhysicalTopologyOut{ "PhysicalTopologyOut.sgn" };
	PhysicalTopologyOut.setSaveInAscii(true);
	

	/* Blocks Declaration */
	Scheduler Scheduler_{ {},{ &SchedulerOut} };
	Scheduler_.setODU0(odu0);
	Scheduler_.setODU1(odu1);
	Scheduler_.setODU2(odu2);
	Scheduler_.setODU3(odu3);
	Scheduler_.setODU4(odu4);
	Scheduler_.setDemandsOrderingRule(orderingRule);

	Sink SinkScheduler_{ { &SchedulerOut },{} };
	SinkScheduler_.setDisplayNumberOfSamples(true);

	LogicalTopologyGenerator LogicalTopologyGenerator_{ {},{ &LogicalTopologyOut } };
	LogicalTopologyGenerator_.setTransportMode(transportMode);
	LogicalTopologyGenerator_.setAdjacencyMatrix(adjacencyMatrix);

	Sink SinkLogicalTopology_{ { &LogicalTopologyOut },{} };
	SinkLogicalTopology_.setDisplayNumberOfSamples(true);

	PhysicalTopologyGenerator PhysicalTopologyGenerator_{ {},{ &PhysicalTopologyOut } };
	PhysicalTopologyGenerator_.setAdjacencyMatrix(adjacencyMatrix);
	PhysicalTopologyGenerator_.setTransmissionSystems(transmissionSystems);
	PhysicalTopologyGenerator_.setOpticalChannels(opticalChannels);
	PhysicalTopologyGenerator_.setOpticalChannelCapacity(opticalChannelCapacity);

	Sink SinkPhysicalTopology_{ { &PhysicalTopologyOut },{} };
	SinkPhysicalTopology_.setDisplayNumberOfSamples(true);

    
	System MainSystem{
			// BLOCKS
			&Scheduler_,
			&SinkScheduler_,
			&LogicalTopologyGenerator_,
			&SinkLogicalTopology_,
			&PhysicalTopologyGenerator_,
			&SinkPhysicalTopology_,

	};
	
	MainSystem.run();
	MainSystem.terminate();

	system("pause");

	return 0;

}
