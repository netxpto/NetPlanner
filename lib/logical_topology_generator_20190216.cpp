# include "..\include\logical_topology_generator_20190216.h"

//using namespace std;

void LogicalTopologyGenerator::initialize(void) {

}

bool LogicalTopologyGenerator::runBlock(void) {
	

	if (!generate)
		return false;
	else 
		generate = false;

	if (transportMode == transport_mode::opaque)
	{
		outputSignals[0]->bufferPut((t_matrix) physicalTopology);
	}	
	else if (transportMode == transport_mode::transparent)  // Considering all nodes transparent
	{
		t_matrix outputTopology;
		outputTopology.resize(physicalTopology[0].size(), vector<int>(physicalTopology[0].size(),1));
		for (size_t k = 0; k < physicalTopology[0].size(); k++) outputTopology[k][k] = 0;
				
		outputSignals[0]->bufferPut((t_matrix) outputTopology);
	}
	else
	{
		cout << "Error: logical_Top_Gen_20190216.h - Transport Mode not defined\n";
	}

	return true;
};