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
	t_matrix odu0;
	t_matrix odu1;
	t_matrix odu2;
	t_matrix odu3;
	t_matrix odu4;
	t_integer orderingRule{ 0 }; //Demands odering rule
	std::string transportMode{ "opaque" };
	t_matrix physicalTopologyAdjacencyMatrix;
	t_integer numberOfOMSPerLink{ 1 };
	t_integer numberOfOpticalChannelsPerOMS{ 2 };
	double initialWavelenght{ 1550 };
	double wavelengthSpacing{ 0.8 };
	t_integer opticalChannelCapacity{ 80 }; //Capacity of each optical channel in ODU0s
	std::string routingCriterionLogicalTopology{ "hops" };
	t_integer blockingCriterionLogicalTopology{ 3 };
	std::string routingCriterionPhysicalTopology{ "hops" };
	t_integer blockingCriterionPhysicalTopology{ 3 };
	
	/* Initializes default input parameters */
	SimulationInputParameters() : SystemInputParameters() 
	{
		initializeInputParameterMap();
	}

	/* Initializes input parameters according to the program arguments */
	/* Usage: .\opaque.exe <input_parameters_0.txt> <output_directory> */
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
		addInputParameter("numberOfOMSPerLink", &numberOfOMSPerLink);
		addInputParameter("numberOfOpticalChannelsPerOMS", &numberOfOpticalChannelsPerOMS);
		addInputParameter("initialWavelength", &initialWavelenght);
		addInputParameter("wavelengthSpacing", &wavelengthSpacing);
		addInputParameter("opticalChannelCapacity", &opticalChannelCapacity);
		addInputParameter("routingCriterionLogicalTopology", &routingCriterionLogicalTopology);
		addInputParameter("blockingCriterionLogicalTopology", &blockingCriterionLogicalTopology);
		addInputParameter("routingCriterionLogicalTopology", &routingCriterionLogicalTopology);
		addInputParameter("blockingCriterionPhysicalTopology", &blockingCriterionPhysicalTopology);
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
	LogicalTopology LogicalTopologyGenerator_Out{ "LogicalTopologyGenerator_Out.sgn", 10 };
	LogicalTopologyGenerator_Out.setSaveInAscii(true);

	PhysicalTopology PhysicalTopologyGenerator_Out{ "PhysicalTopologyGenerator_Out.sgn", 10 };
	PhysicalTopologyGenerator_Out.setSaveInAscii(true);

	DemandRequest Scheduler_Out{ "Scheduler_Out.sgn", 10};
	Scheduler_Out.setSaveInAscii(true);
	
	PathRequest LogicalTopologyManager_PathRequest{ "LogicalTopologyManager_PathRequest.sgn", 10 };
	LogicalTopologyManager_PathRequest.setSaveInAscii(true);

	PathRequestRouted PhysicalTopologyManager_PathRequestRouted{ "PhysicalTopologyManager_PathRequestRouted.sgn", 10 };
	PhysicalTopologyManager_PathRequestRouted.setSaveInAscii(true);

	DemandRequestRouted ProcessedDemand{ "ProcessedDemand.sgn", 10};
	ProcessedDemand.setSaveInAscii(true);

	LogicalTopology FinalLogicalTopology{ "FinalLogicalTopology.sgn", 10 };
	FinalLogicalTopology.setSaveInAscii(true);
	
	PhysicalTopology FinalPhysicalTopology{ "FinalPhysicalTopology.sgn", 10 };
	FinalPhysicalTopology.setSaveInAscii(true);


	/* Blocks Declaration */
	LogicalTopologyGenerator LogicalTopologyGenerator_{ {},{ &LogicalTopologyGenerator_Out } };
	LogicalTopologyGenerator_.setTransportMode(param.transportMode);
	LogicalTopologyGenerator_.setPhysicalTopologyAdjacencyMatrix(param.physicalTopologyAdjacencyMatrix);

	PhysicalTopologyGenerator PhysicalTopologyGenerator_{ {},{ &PhysicalTopologyGenerator_Out } };
	PhysicalTopologyGenerator_.setPhysicalTopologyAdjacencyMatrix(param.physicalTopologyAdjacencyMatrix);
	PhysicalTopologyGenerator_.setNumberOfOMSPerLink(param.numberOfOMSPerLink);
	PhysicalTopologyGenerator_.setNumberOfOpticalChannelsPerOMS(param.numberOfOpticalChannelsPerOMS);
	PhysicalTopologyGenerator_.setInitialWavelength(param.initialWavelenght);
	PhysicalTopologyGenerator_.setWavelengthSpacing(param.wavelengthSpacing);
	PhysicalTopologyGenerator_.setOpticalChannelCapacity(param.opticalChannelCapacity);

	Scheduler Scheduler_{ {},{ &Scheduler_Out} };
	Scheduler_.setODU0(param.odu0);
	Scheduler_.setODU1(param.odu1);
	Scheduler_.setODU2(param.odu2);
	Scheduler_.setODU3(param.odu3);
	Scheduler_.setODU4(param.odu4);
	Scheduler_.setOrderingRule(param.orderingRule);

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
			&LogicalTopologyGenerator_,
			&PhysicalTopologyGenerator_,
			&Scheduler_,
			&LogicalTopologyManager_,
			&PhysicalTopologyManager_,
			&SinkRoutedOrBlocked_,
			&SinkLogicalTopology_,
			&SinkPhysicalTopology_,		
	};
	
	//System Run
	MainSystem.run();
	MainSystem.terminate();

	system("pause");

	return 0;
}
