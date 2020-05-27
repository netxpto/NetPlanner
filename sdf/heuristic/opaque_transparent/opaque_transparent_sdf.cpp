# include "..\opaque_transparent\include\netxpto_20190130.h"
# include "..\opaque_transparent\include\scheduler_20190122.h"
# include "..\opaque_transparent\include\logical_topology_generator_20190216.h"
# include "..\opaque_transparent\include\physical_topology_generator_20190227.h"
# include "..\opaque_transparent\include\sink_20180815.h"
# include "..\opaque_transparent\include\logical_topology_manager_20190420.h"
# include "..\opaque_transparent\include\physical_topology_manager_20190421.h"



//##########################################################################################
//######################## Simulation Input Parameters #####################################
//##########################################################################################

// devera ser feito de acordo com IPS do netxpto

class NetworkSimulatorInputParameters : public SystemInputParameters {
public:

	// INPUT PARAMETERS (with default values)
	int numberOfNodes{ 0 }; // Number of nodes existent in the network
	t_matrix odu0{ 0 };
	t_matrix odu1{ 0 };
	t_matrix odu2{ 0 };
	t_matrix odu3{ 0 };
	t_matrix odu4{ 0 };
	ordering_rule orderingRule{ ordering_rule::descendingOrder };
	t_matrix physicalTopologyAdjacencyMatrix{ 0 };
	int span{ 0 };
	t_matrix distancesBetweenNodes{ 0 };
	transport_mode transportMode{ transport_mode::transparent };
	int numberOfOMSPerLink{ 0 };
	int numberOfOpticalChannelsPerOMS{ 0 };
	double initialWavelenght{ 1550 };
	double wavelenghtSpacing{ 0.8 };
	int opticalChannelCapacity{ 0 };
	t_routing_criterion_logical_topology routingCriterionLogicalTopology{ t_routing_criterion_logical_topology::hops };
	int blockingCriterionLogicalTopology{ 1 };
	t_routing_criterion_physical_topology routingCriterionPhysicalTopology{ t_routing_criterion_physical_topology::hops };
	int blockingCriterionPhysicalTopology{ 3 };
	int OLTsCost{ 15000 };
	int TranspondersCost{ 5000 };
	int AmplifiersCost{ 10000 };//2000
	int EXCsCost{ 10000 };
	int ODU0portsCost{ 12 }; //125
	int ODU1portsCost{ 25 }; //250
	int ODU2portsCost{ 100 }; //1000
	int ODU3portsCost{ 400 }; //4000
	int ODU4portsCost{ 1000 }; //1000
	int OTU4portsCost{ 1000 }; //10000
	int OXCsCost{ 20000 };
	int addPortsCost{ 2500 };
	int linePortsCost{ 2500 };



	/* Initializes default input parameters*/
	NetworkSimulatorInputParameters() : SystemInputParameters() {
		initializeInputParameterMap();
	}

	/* Initializes input parameters according to the program arguments */
	/* Usage: .\network_simulator.exe <InpuParameters.txt> <output_directory>*/
	NetworkSimulatorInputParameters(int argc, char *argv[]) : SystemInputParameters(argc, argv) {
		initializeInputParameterMap();
		readSystemInputParameters();
	}

	NetworkSimulatorInputParameters(std::string fName) : SystemInputParameters(fName) {
		initializeInputParameterMap();
		readSystemInputParameters();
	}

	// Each parameter must be added to the parameter std::map by calling this function
	void initializeInputParameterMap() {
		addInputParameter("numberOfNodes", &numberOfNodes);
		addInputParameter("odu0", &odu0);
		addInputParameter("odu1", &odu1);
		addInputParameter("odu2", &odu2);
		addInputParameter("odu3", &odu3);
		addInputParameter("odu4", &odu4);
		addInputParameter("orderingRule", &orderingRule);
		addInputParameter("physicalTopologyAdjacencyMatrix", &physicalTopologyAdjacencyMatrix);
		addInputParameter("distancesBetweenNodes", &distancesBetweenNodes);
		addInputParameter("span", &span);
		addInputParameter("transportMode", &transportMode);
		addInputParameter("numberOfOMSPerLink", &numberOfOMSPerLink);
		addInputParameter("numberOfOpticalChannelsPerOMS", &numberOfOpticalChannelsPerOMS);
		addInputParameter("initialWavelenght", &initialWavelenght);
		addInputParameter("wavelenghtSpacing", &wavelenghtSpacing);
		addInputParameter("opticalChannelCapacity", &opticalChannelCapacity);
		addInputParameter("routingCriterionLogicalTopology", &routingCriterionLogicalTopology);
		addInputParameter("blockingCriterionLogicalTopology", &blockingCriterionLogicalTopology);
		addInputParameter("routingCriterionPhysicalTopology", &routingCriterionPhysicalTopology);
		addInputParameter("blockingCriterionPhysicalTopology", &blockingCriterionPhysicalTopology);
		addInputParameter( "OLTsCost", &OLTsCost);
		addInputParameter("TranspondersCost", &TranspondersCost);
		addInputParameter("AmplifiersCost", &AmplifiersCost);
		addInputParameter("EXCsCost", &EXCsCost);
		addInputParameter("ODU0portsCost", &ODU0portsCost);
		addInputParameter("ODU1portsCost", &ODU1portsCost);
		addInputParameter("ODU2portsCost", &ODU2portsCost);
		addInputParameter("ODU3portsCost", &ODU3portsCost);
		addInputParameter("ODU4portsCost", &ODU4portsCost);
		addInputParameter("OTU4portsCost", &OTU4portsCost);
		addInputParameter("OXCsCost", &OXCsCost);
		addInputParameter("addPortsCost", &addPortsCost);
		addInputParameter("linePortsCost", &linePortsCost);
	}
};


//##########################################################################################
//##########################################################################################
//##########################################################################################


//int main(int argc, char *argv[])
int main()
{

	//NetworkSimulatorInputParameters param(argc, argv);
	NetworkSimulatorInputParameters param("input_parameters_values_TF.txt");


	/* Signals Declaration */
	DemandRequest Scheduler_Out{ "Scheduler_Out.sgn", 1 };
	Scheduler_Out.setSaveInAscii(true);
	Scheduler_Out.setSaveSignal(true);

	LogicalTopology LogicalTopologyGenerator_Out{ "LogicalTopologyGenerator_Out.sgn",1};
	LogicalTopologyGenerator_Out.setSaveInAscii(true);
	LogicalTopologyGenerator_Out.setSaveSignal(true);

	PhysicalTopology PhysicalTopologyGenerator_Out{ "PhysicalTopologyGenerator_Out.sgn",1};
	PhysicalTopologyGenerator_Out.setSaveInAscii(true);
	PhysicalTopologyGenerator_Out.setSaveSignal(true);

	LogicalTopology FinalLogicalTopology{ "FinalLogicalTopology.sgn",1};
	FinalLogicalTopology.setSaveInAscii(true);
	FinalLogicalTopology.setSaveSignal(true);

	PhysicalTopology FinalPhysicalTopology{ "FinalPhysicalTopology.sgn",1};
	FinalPhysicalTopology.setSaveInAscii(true);
	FinalPhysicalTopology.setSaveSignal(true);

	PathRequest LogicalTopologyManager_PathRequest{ "LogicalTopologyManager_PathRequest.sgn"};
	LogicalTopologyManager_PathRequest.setSaveInAscii(true);
	LogicalTopologyManager_PathRequest.setSaveSignal(true);

	PathRequestRouted PhysicalTopologyManager_PathRequestRouted{ "PhysicalTopologyManager_PathRequestRouted.sgn"};
	PhysicalTopologyManager_PathRequestRouted.setSaveInAscii(true);
	PhysicalTopologyManager_PathRequestRouted.setSaveSignal(true);

	DemandRequestRouted ProcessedDemand{ "ProcessedDemand.sgn",1};
	ProcessedDemand.setSaveInAscii(true);
	ProcessedDemand.setSaveSignal(true);

	/* Blocks Decalration */
	Scheduler Scheduler_{ {},{ &Scheduler_Out} };
	Scheduler_.setNumberOfNodes(param.numberOfNodes);
	Scheduler_.setODU0(param.odu0);
	Scheduler_.setODU0Copy();
	Scheduler_.setODU1(param.odu1);
	Scheduler_.setODU1Copy();
	Scheduler_.setODU2(param.odu2);
	Scheduler_.setODU2Copy();
	Scheduler_.setODU3(param.odu3);
	Scheduler_.setODU3Copy();
	Scheduler_.setODU4(param.odu4);
	Scheduler_.setODU4Copy();
	Scheduler_.setOrderingRule(param.orderingRule);

	Sink SinkScheduler_{ { &Scheduler_Out },{} };
	SinkScheduler_.setDisplayNumberOfSamples(true);


	LogicalTopologyGenerator LogicalTopologyGenerator_{ {},{&LogicalTopologyGenerator_Out} };
	LogicalTopologyGenerator_.setTransportMode(param.transportMode);
	LogicalTopologyGenerator_.setPhysicalTopologyAjacencyMatrix(param.physicalTopologyAdjacencyMatrix);

	Sink SinkLogicalTopologyGenerator_{ { &LogicalTopologyGenerator_Out },{} };
	SinkLogicalTopologyGenerator_.setDisplayNumberOfSamples(true);

	PhysicalTopologyGenerator PhysicalTopologyGenerator_{ {},{&PhysicalTopologyGenerator_Out} };
	PhysicalTopologyGenerator_.setNumberOfOpticalChannelsPerOMS(param.numberOfOpticalChannelsPerOMS);
	PhysicalTopologyGenerator_.setInitialWavelenght(param.initialWavelenght);
	PhysicalTopologyGenerator_.setWavelenghtSpacing(param.wavelenghtSpacing);
	PhysicalTopologyGenerator_.setPhysicalTopologyAdjacencyMatrix(param.physicalTopologyAdjacencyMatrix);
	PhysicalTopologyGenerator_.setDistancesBetweenNodes(param.distancesBetweenNodes);
	PhysicalTopologyGenerator_.setSpan(param.span);
	PhysicalTopologyGenerator_.setNumberOfOMSPerLink(param.numberOfOMSPerLink);
	PhysicalTopologyGenerator_.setOpticalChannelCapacity(param.opticalChannelCapacity);
	PhysicalTopologyGenerator_.setTransportMode(param.transportMode);

	Sink SinkPhysicalTopologyGenerator_{ { &PhysicalTopologyGenerator_Out },{} };
	SinkPhysicalTopologyGenerator_.setDisplayNumberOfSamples(true);


	LogicalTopologyManager LogicalTopologyManager_{ {&LogicalTopologyGenerator_Out, &Scheduler_Out, &PhysicalTopologyManager_PathRequestRouted},{&FinalLogicalTopology, &ProcessedDemand, &LogicalTopologyManager_PathRequest} };
	LogicalTopologyManager_.setRoutingCriterionLogicalTopology(param.routingCriterionLogicalTopology);
	LogicalTopologyManager_.setBlockingCriterionLogicalTopology(param.blockingCriterionLogicalTopology);

	Sink SinkLogicalTopology_{ {&FinalLogicalTopology},{} };
	SinkLogicalTopology_.setDisplayNumberOfSamples(false);

	Sink SinkLogicalTopologyProcessedDemands_{ {&ProcessedDemand},{} };
	SinkLogicalTopology_.setDisplayNumberOfSamples(true);

	Sink SinkRoutedOrBlocked_{ {&ProcessedDemand},{} };
	SinkRoutedOrBlocked_.setDisplayNumberOfSamples(true);

	PhysicalTopologyManager PhysicalTopologyManager_{ {&PhysicalTopologyGenerator_Out, &LogicalTopologyManager_PathRequest},{&FinalPhysicalTopology , &PhysicalTopologyManager_PathRequestRouted} };
	PhysicalTopologyManager_.setRoutingCriterionPhysicalTopology(param.routingCriterionPhysicalTopology);
	PhysicalTopologyManager_.setBlockingCriterionPhysicalTopology(param.blockingCriterionPhysicalTopology);

	Sink SinkPhysicalTopology_{ {&FinalPhysicalTopology},{} };
	SinkPhysicalTopology_.setDisplayNumberOfSamples(false);


	System MainSystem{
		// BLOCKS
		&Scheduler_,
		&LogicalTopologyGenerator_,
		&PhysicalTopologyGenerator_,
		&LogicalTopologyManager_,
		&SinkLogicalTopology_,
		&SinkLogicalTopologyProcessedDemands_,
		&PhysicalTopologyManager_,
		&SinkPhysicalTopology_
	};

	MainSystem.run();
	MainSystem.terminate();
	MainSystem.writeReport(LogicalTopologyManager_.getLogicalTopology(), PhysicalTopologyManager_.getPhysicalTopology(), param.odu0, param.odu1, param.odu2, param.odu3, param.odu4, param.orderingRule, param.OLTsCost, param.TranspondersCost, param.AmplifiersCost, param.EXCsCost, param.ODU0portsCost, param.ODU1portsCost, param.ODU2portsCost, param.ODU3portsCost, param.ODU4portsCost, param.OTU4portsCost, param.OXCsCost, param.addPortsCost, param.linePortsCost);

	system("pause");

	return 0;
}
