#include "..\..\..\include_opaque\netxpto_20190130.h"
#include "..\..\..\include_opaque\logical_topology_generator_20190216.h"
#include "..\..\..\include_opaque\physical_topology_generator_20190221.h"
#include "..\..\..\include_opaque\scheduler_20190122.h"
#include "..\..\..\include_opaque\logical_topology_manager_20190419.h"
#include "..\..\..\include_opaque\physical_topology_manager_20190420.h"
#include "..\..\..\include_opaque\sink_20180815.h"

//##########################################################################################
//######################## Simulation Input Parameters #####################################
//##########################################################################################

class SimulationInputParameters : public SystemInputParameters 
{
public:
	//INPUT PARAMETERS
	t_matrix odu0{ 0 };
	t_matrix odu1{ 0 };
	t_matrix odu2{ 0 };
	t_matrix odu3{ 0 };
	t_matrix odu4{ 0 };
	t_ordering_rule orderingRule{ t_ordering_rule::descendingOrder }; //Demands odering rule
	t_transport_mode transportMode{ t_transport_mode::opaque };
	t_matrix physicalTopologyAdjacencyMatrix{ 0 };
	t_matrix distanceMatrix{ 0 };
	t_integer span{ 100 };
	t_integer numberOfOMSPerLink{ 1 };
	t_integer numberOfOpticalChannelsPerOMS{ 2 };
	double initialWavelength{ 1550 };
	double wavelengthSpacing{ 0.8 };
	t_integer opticalChannelCapacity{ 80 }; //Capacity of each optical channel in ODU0s
	t_routing_criterion_logical_topology routingCriterionLogicalTopology{ t_routing_criterion_logical_topology::hops };
	t_integer blockingCriterionLogicalTopology{ 3 };
	t_routing_criterion_physical_topology routingCriterionPhysicalTopology{ t_routing_criterion_physical_topology::hops };
	t_integer blockingCriterionPhysicalTopology{ 3 };
	int OLTsCost{ 15000 };
	int TranspondersCost{ 5000 };
	int AmplifiersCost{ 4000 };
	int EXCsCost{ 10000 };
	int ODU0portsCost{ 10 };
	int ODU1portsCost{ 15 };
	int ODU2portsCost{ 30 };
	int ODU3portsCost{ 60 };
	int ODU4portsCost{ 100 };
	int OTU4portsCost{ 100000 };
	int OXCsCost{ 20000 };
	int addPortsCost{ 2500 };
	int linePortsCost{ 2500 };
	
	/* Initializes default input parameters */
	SimulationInputParameters() : SystemInputParameters() 
	{
		initializeInputParameterMap();
	}

	/* Initializes input parameters according to the program arguments */
	/* Usage: .\opaque.exe <input_parameters_opaque.txt> <output_directory> */
	SimulationInputParameters(int argc, char*argv[]) : SystemInputParameters(argc, argv) 
	{
		initializeInputParameterMap();
		readSystemInputParameters();
	}

	SimulationInputParameters(string fName) : SystemInputParameters(fName) 
	{
		initializeInputParameterMap();
		readSystemInputParameters();
	}
	//Each parameter must be added to the parameter map by calling addInputParameter(string,param*)
	void initializeInputParameterMap() 
	{
		addInputParameter("odu0", &odu0);
		addInputParameter("odu1", &odu1);
		addInputParameter("odu2", &odu2);
		addInputParameter("odu3", &odu3);
		addInputParameter("odu4", &odu4);
		addInputParameter("orderingRule", &orderingRule);
		addInputParameter("transportMode", &transportMode);
		addInputParameter("physicalTopologyAdjacencyMatrix", &physicalTopologyAdjacencyMatrix);
		addInputParameter("distanceMatrix", &distanceMatrix);
		addInputParameter("span", &span);
		addInputParameter("numberOfOMSPerLink", &numberOfOMSPerLink);
		addInputParameter("numberOfOpticalChannelsPerOMS", &numberOfOpticalChannelsPerOMS);
		addInputParameter("initialWavelength", &initialWavelength);
		addInputParameter("wavelengthSpacing", &wavelengthSpacing);
		addInputParameter("opticalChannelCapacity", &opticalChannelCapacity);
		addInputParameter("routingCriterionLogicalTopology", &routingCriterionLogicalTopology);
		addInputParameter("blockingCriterionLogicalTopology", &blockingCriterionLogicalTopology);
		addInputParameter("routingCriterionLogicalTopology", &routingCriterionLogicalTopology);
		addInputParameter("blockingCriterionPhysicalTopology", &blockingCriterionPhysicalTopology);
		addInputParameter("OLTsCost", &OLTsCost);
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

//int main(int argc, char*argv[]) 
int main()
{
	//SimulationInputParameters param(argc, argv);
	SimulationInputParameters param("input_parameters.txt");

	//Signals Declaration 
	DemandRequest Scheduler_Out{ "Scheduler_Out.sgn", 1 };
	Scheduler_Out.setSaveInAscii(true);

	LogicalTopology LogicalTopologyGenerator_Out{ "LogicalTopologyGenerator_Out.sgn" };
	LogicalTopologyGenerator_Out.setSaveInAscii(true);

	PhysicalTopology PhysicalTopologyGenerator_Out{ "PhysicalTopologyGenerator_Out.sgn" };
	PhysicalTopologyGenerator_Out.setSaveInAscii(true);

	PathRequest LogicalTopologyManager_PathRequest{ "LogicalTopologyManager_PathRequest.sgn" };
	LogicalTopologyManager_PathRequest.setSaveInAscii(true);

	PathRequestRouted PhysicalTopologyManager_PathRequestRouted{ "PhysicalTopologyManager_PathRequestRouted.sgn" };
	PhysicalTopologyManager_PathRequestRouted.setSaveInAscii(true);

	DemandRequestRouted ProcessedDemand{ "ProcessedDemand.sgn" };
	ProcessedDemand.setSaveInAscii(true);

	LogicalTopology FinalLogicalTopology{ "FinalLogicalTopology.sgn" };
	FinalLogicalTopology.setSaveInAscii(true);
	
	PhysicalTopology FinalPhysicalTopology{ "FinalPhysicalTopology.sgn" };
	FinalPhysicalTopology.setSaveInAscii(true);


	/* Blocks Declaration */
	Scheduler Scheduler_{ {},{ &Scheduler_Out } };
	Scheduler_.setODU0(param.odu0);
	Scheduler_.setODU1(param.odu1);
	Scheduler_.setODU2(param.odu2);
	Scheduler_.setODU3(param.odu3);
	Scheduler_.setODU4(param.odu4);
	Scheduler_.setOrderingRule(param.orderingRule);

	LogicalTopologyGenerator LogicalTopologyGenerator_{ {},{ &LogicalTopologyGenerator_Out } };
	LogicalTopologyGenerator_.setTransportMode(param.transportMode);
	LogicalTopologyGenerator_.setPhysicalTopologyAdjacencyMatrix(param.physicalTopologyAdjacencyMatrix);
	LogicalTopologyGenerator_.setDistanceMatrix(param.distanceMatrix); // n�o devia estar aqui

	PhysicalTopologyGenerator PhysicalTopologyGenerator_{ {},{ &PhysicalTopologyGenerator_Out } };
	PhysicalTopologyGenerator_.setPhysicalTopologyAdjacencyMatrix(param.physicalTopologyAdjacencyMatrix);
	PhysicalTopologyGenerator_.setDistanceMatrix(param.distanceMatrix);
	PhysicalTopologyGenerator_.setSpan(param.span);
	PhysicalTopologyGenerator_.setNumberOfOMSPerLink(param.numberOfOMSPerLink);
	PhysicalTopologyGenerator_.setNumberOfOpticalChannelsPerOMS(param.numberOfOpticalChannelsPerOMS);
	PhysicalTopologyGenerator_.setInitialWavelength(param.initialWavelength);
	PhysicalTopologyGenerator_.setWavelengthSpacing(param.wavelengthSpacing);
	PhysicalTopologyGenerator_.setOpticalChannelCapacity(param.opticalChannelCapacity);

	LogicalTopologyManager LogicalTopologyManager_{ { &LogicalTopologyGenerator_Out, &Scheduler_Out, &PhysicalTopologyManager_PathRequestRouted },{ &LogicalTopologyManager_PathRequest, &FinalLogicalTopology, &ProcessedDemand } };
	LogicalTopologyManager_.setRoutingCriterionLogicalTopology(param.routingCriterionLogicalTopology);
	LogicalTopologyManager_.setBlockingCriterionLogicalTopology(param.blockingCriterionLogicalTopology);

	PhysicalTopologyManager PhysicalTopologyManager_{ { &PhysicalTopologyGenerator_Out, &LogicalTopologyManager_PathRequest},{ &PhysicalTopologyManager_PathRequestRouted, &FinalPhysicalTopology } };
	PhysicalTopologyManager_.setRoutingCriterionPhysicalTopology(param.routingCriterionPhysicalTopology);
	PhysicalTopologyManager_.setBlockingCriterionPhysicalTopology(param.blockingCriterionPhysicalTopology);

	Sink SinkRoutedOrBlocked_{ {&ProcessedDemand},{} };
	SinkRoutedOrBlocked_.setDisplayNumberOfSamples(true);

	Sink SinkLogicalTopology_{ {&FinalLogicalTopology},{} };
	SinkLogicalTopology_.setDisplayNumberOfSamples(true);

	Sink SinkPhysicalTopology_{ {&FinalPhysicalTopology},{} };
	SinkPhysicalTopology_.setDisplayNumberOfSamples(true);


	//System Declaration and Initialization
	System MainSystem
	{
			// BLOCKS
			&Scheduler_,
			&LogicalTopologyGenerator_,
			&PhysicalTopologyGenerator_,
			&LogicalTopologyManager_,
			&PhysicalTopologyManager_,
			&SinkRoutedOrBlocked_,
			&SinkLogicalTopology_,
			&SinkPhysicalTopology_,		
	};
	
	//System Run
	MainSystem.run();
	MainSystem.terminate();
	MainSystem.writeReport(LogicalTopologyManager_.getLogicalTopology(), PhysicalTopologyManager_.getPhysicalTopology(), param.odu0, param.odu1, param.odu2, param.odu3, param.odu4, param.orderingRule, param.OLTsCost, param.TranspondersCost, param.AmplifiersCost, param.EXCsCost, param.ODU0portsCost, param.ODU1portsCost, param.ODU2portsCost, param.ODU3portsCost, param.ODU4portsCost, param.OTU4portsCost, param.OXCsCost, param.addPortsCost, param.linePortsCost);
	
	system("pause");

	return 0;
}
