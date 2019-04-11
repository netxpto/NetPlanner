#include "..\..\..\include_opaque\netxpto_20190130.h"
#include "..\..\..\include_opaque\scheduler_20190122.h"
#include "..\..\..\include_opaque\sink_20180815.h"
#include "..\..\..\include_opaque\logical_topology_generator_20190216.h"
#include "..\..\..\include_opaque\physical_topology_generator_20190221.h"


//##########################################################################################
//######################## Simulation Input Parameters #####################################
//##########################################################################################

// Traffic matrices
t_matrix odu0{	{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0} };

t_matrix odu1{	{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0} };

t_matrix odu2{	{0,0,0,0,0,1},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0} };

t_matrix odu3{	{0,0,3,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0} };

t_matrix odu4{	{0,1,2,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0},
				{0,0,0,0,0,0} };
				
// Demands odering rule
t_integer orderingRule{ 0 };

// Transport mode
transport_mode transportMode{transport_mode::opaque};

// Adjacency matrix of the physical network 
t_matrix adjacencyMatrix{ {0,1,0,0,0,1},
						  {0,0,1,0,0,0},
						  {0,0,0,0,0,0},
						  {0,0,1,0,0,0},
						  {0,0,1,1,0,0},
						  {0,1,0,0,1,0} };

// Number of transmission systems
t_integer transmissionSystems{ 1 };

// Number of optical channels per link
t_integer opticalChannels{ 2 };

// Capacity of each optical channel in ODU0s
t_integer opticalChannelCapacity{ 80 };

// Routing Criterion Logical Topology
routing_criterion routingCriterionLogicalTopology{ routing_criterion::hops };

// Routing Criterion Physical Topology
routing_criterion routingCriterionPhysicalTopology{ routing_criterion::hops };

//##########################################################################################
//##########################################################################################
//##########################################################################################

int main() {

	/* Signals Declaration */
	DemandRequest Scheduler_Out{ "Scheduler_Out.sgn", 10};
	Scheduler_Out.setSaveInAscii(true);
	
	LogicalTopology LogicalTopology_Out{ "LogicalTopology_Out.sgn", 32 };
	LogicalTopology_Out.setSaveInAscii(true);

	PhysicalTopology PhysicalTopology_Out{ "PhysicalTopology_Out.sgn", 32 };
	PhysicalTopology_Out.setSaveInAscii(true);

	/*
	PathRequest LogicalManager_PathRequest{ "LogicalTopologyManager_PathRequest.sgn", 32 };
	LogicalTopologyManager_PathRequest.setSaveInAscii(true);

	PathRequestRouted PhysicalTopologyManager_PathRequestRouted{ "PhysicalTopologyManager_PathRequestRouted.sgn", 32 };
	PhysicalTopologyManager_PathRequestRouted.setSaveInAscii(true);

	DemandRequestRouted ProcessedDemand{ "ProcessedDemand.sgn", 32 };
	ProcessedDemand.setSaveInAscii(true);

	LogicalTopology FinalLogicalTopology{ "FinalLogicalTopology.sgn", 32 };
	FinalLogicalTopology.setSaveInAscii(true);

	PhysicalTopology FinalPhysicalTopology{ "FinalPhysicalTopology.sgn", 32 };
	FinalPhysicalTopology.setSaveInAscii(true);
	
	*/

	/* Blocks Declaration */
	Scheduler Scheduler_{ {},{ &Scheduler_Out} };
	Scheduler_.setODU0(odu0);
	Scheduler_.setODU1(odu1);
	Scheduler_.setODU2(odu2);
	Scheduler_.setODU3(odu3);
	Scheduler_.setODU4(odu4);
	Scheduler_.setOrderingRule(orderingRule);

	Sink SinkScheduler_{ { &Scheduler_Out },{} };
	SinkScheduler_.setDisplayNumberOfSamples(true);

	LogicalTopologyGenerator LogicalTopologyGenerator_{ {},{ &LogicalTopology_Out } };
	LogicalTopologyGenerator_.setTransportMode(transportMode);
	LogicalTopologyGenerator_.setAdjacencyMatrix(adjacencyMatrix);

	Sink SinkLogicalTopologyGenerator_{ { &LogicalTopology_Out },{} };
	SinkLogicalTopologyGenerator_.setDisplayNumberOfSamples(true);

	PhysicalTopologyGenerator PhysicalTopologyGenerator_{ {},{ &PhysicalTopology_Out } };
	PhysicalTopologyGenerator_.setAdjacencyMatrix(adjacencyMatrix);
	PhysicalTopologyGenerator_.setTransmissionSystems(transmissionSystems);
	PhysicalTopologyGenerator_.setOpticalChannels(opticalChannels);
	PhysicalTopologyGenerator_.setOpticalChannelCapacity(opticalChannelCapacity);

	Sink SinkPhysicalTopologyGenerator_{ { &PhysicalTopology_Out },{} };
	SinkPhysicalTopologyGenerator_.setDisplayNumberOfSamples(true);

	/*
	LogicalTopologyManager LogicalTopologyManager_{ { &Scheduler_Out, &LogicalTopology_Out, PhysicalTopologyManager_PathRequestRouted},{ &LogicalTopologyManager_PathRequest, &FinalLogicalTopology, &ProcessedDemand } };
	LogicalTopologyManager_.setRoutingCriterionLogicalTopology(routingCriterionLogicalTopology);
	LogicalTopologyManager_.setBlockingCriterionLogicalTopology(blockingCriterionLogicalTopology);

	PhysicalTopologyManager PhysicalTopologyManager_{ { &PhysicalTopology_Out, &LogicalTopologyManager_PathRequest},{ &FinalPhysicalManager } };
	PhysicalTopologyManager_.setRoutingCriterionPhysicalTopology(routingCriterionPhysicalTopology);
	PhysicalTopologyManager_.setBlockingCriterionPhysicalTopology(blockingCriterionPhysicalTopology);

	Sink SinkLogicalTopology_{ {&FinalLogicalTopology},{} };
	SinkLogicalTopology_.setDisplayNumberOfSamples(true);

	Sink SinkPhysicalTopology_{ {&FinalPhysicalTopology},{} };
	SinkPhysicalTopology_.setDisplayNumberOfSamples(true);

	Sink SinkRoutedOrBlocked_{ {&ProcessedDemand},{} };
	SinkRoutedOrBlocked_.setDisplayNumberOfSamples(true);
	*/

	System MainSystem{
			// BLOCKS
			&Scheduler_,
			&SinkScheduler_,
			&LogicalTopologyGenerator_,
			&SinkLogicalTopologyGenerator_,
			&PhysicalTopologyGenerator_,
			&SinkPhysicalTopologyGenerator_,
			//&LogicalTopologyManager_,
			//&PhysicalTopologyManager_,
			//&SinkLogicalTopology_,
			//&SinkPhysicalTopology_,
			//&SinkRoutedOrBlocked_,
		
	};
	
	MainSystem.run();
	MainSystem.terminate();

	system("pause");

	return 0;

}
