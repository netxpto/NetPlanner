# include "..\include\physical_topology_manager_20190421.h"


using namespace std;

void PhysicalTopologyManager::initialize(void) {

}

bool PhysicalTopologyManager::runBlock(void) {

	int process = inputSignals[0]->ready(); // number of samples in the buffer ready to be processed

	if (process == 0) return false;

	for (int i = 0; i < process; i++) {
		inputSignals[0]->bufferGet(&currentPhysicalTopology);
	}
	outputSignals[0]->bufferPut((t_physical_topology)currentPhysicalTopology);
	return true;
};