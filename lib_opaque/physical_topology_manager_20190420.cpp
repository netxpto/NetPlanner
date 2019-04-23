#include "..\include_opaque\physical_topology_manager_20190420.h"

void PhysicalTopologyManager::initialize(void) {

	numberOfOutputSignals = (int)outputSignals.size();
}

bool PhysicalTopologyManager::runBlock(void) {

	t_integer readyPhysicalTopology = inputSignals[0]->ready();
	t_integer readyPathRequest = inputSignals[1]->ready();

	if (readyPhysicalTopology == 0) return false;

	t_physical_topology physicalTopology;
	t_path_request pathRequest;
	t_path_request_routed pathRequestRouted;

	inputSignals[0]->bufferGet(&physicalTopology);
	
	for (t_integer r = 0; r < readyPathRequest; r++) {

		inputSignals[1]->bufferGet(&pathRequest);

		for (int i = 0; i < (int)physicalTopology.OMS.size(); i++) {

			if ((pathRequest.numberOfIntermediateNodes == 0 && pathRequest.sourceNode == physicalTopology.OMS[i].sourceNode && pathRequest.destinationNode == physicalTopology.OMS[i].destinationNode)
				|| (pathRequest.numberOfIntermediateNodes == 1 && pathRequest.sourceNode == physicalTopology.OMS[i].sourceNode && pathRequest.intermediateNodes[0] == physicalTopology.OMS[i].destinationNode)
				|| (pathRequest.numberOfIntermediateNodes == 1 && pathRequest.intermediateNodes[0] == physicalTopology.OMS[i].sourceNode && pathRequest.destinationNode == physicalTopology.OMS[i].destinationNode)
				|| (pathRequest.numberOfIntermediateNodes >= 2 && pathRequest.sourceNode == physicalTopology.OMS[i].sourceNode && pathRequest.intermediateNodes[0] == physicalTopology.OMS[i].destinationNode)
				|| (pathRequest.numberOfIntermediateNodes >= 2 && pathRequest.intermediateNodes[pathRequest.numberOfIntermediateNodes - 1] == physicalTopology.OMS[i].sourceNode && pathRequest.destinationNode == physicalTopology.OMS[i].destinationNode)) {

				if (physicalTopology.OMS[i].availableWavelengths[0] == 1) {

					physicalTopology.OMS[i].availableWavelengths[0] = 0;

					t_light_paths_table lightPaths;
					lightPaths.sourceNode = physicalTopology.OMS[i].sourceNode;
					lightPaths.destinationNode = physicalTopology.OMS[i].destinationNode;
					lightPaths.numberOfIntermediateNodes = 0;
					lightPaths.wavelength = physicalTopology.OMS[i].wavelengths[0];
					pathRequestRouted.lightPathsTable.push_back(lightPaths);
				}

				for (int k = 1; k < (int)physicalTopology.OMS[i].availableWavelengths.size(); k++) {

					while (physicalTopology.OMS[i].availableWavelengths[k] != 1){

						if (physicalTopology.OMS[i].availableWavelengths[k+1] == 1) {

							physicalTopology.OMS[i].availableWavelengths[k+1] = 0;

							t_light_paths_table lightPaths;
							lightPaths.sourceNode = physicalTopology.OMS[i].sourceNode;
							lightPaths.destinationNode = physicalTopology.OMS[i].destinationNode;
							lightPaths.numberOfIntermediateNodes = 0;
							lightPaths.wavelength = physicalTopology.OMS[i].wavelengths[k+1];
							pathRequestRouted.lightPathsTable.push_back(lightPaths);
						}
					}
				}
			}

			for (int j = 0; j < (int)pathRequest.intermediateNodes.size() - 1; j++) {

				if (pathRequest.numberOfIntermediateNodes >= 2 && pathRequest.intermediateNodes[j] == physicalTopology.OMS[i].sourceNode && pathRequest.intermediateNodes[j + 1] == physicalTopology.OMS[i].destinationNode) {

					for (int k = 0; k < (int)physicalTopology.OMS[i].availableWavelengths.size(); k++) {

						if (physicalTopology.OMS[i].availableWavelengths[k] == 1) {
							
							physicalTopology.OMS[i].availableWavelengths[k] = 0;

							t_light_paths_table lightPaths;
							lightPaths.sourceNode = physicalTopology.OMS[i].sourceNode;
							lightPaths.destinationNode = physicalTopology.OMS[i].destinationNode;
							lightPaths.numberOfIntermediateNodes = 0;
							lightPaths.wavelength = physicalTopology.OMS[i].wavelengths[k];
							pathRequestRouted.lightPathsTable.push_back(lightPaths);
						}
					}
				}
			}
		}
		
		t_path_routed pathRouted;

		pathRouted.requestIndex = r;
		pathRouted.routed = true;
		pathRouted.numberOfLightPaths = pathRequest.numberOfIntermediateNodes + 1;
		pathRequestRouted.pathRouted.push_back(pathRouted);
	}
	
	outputSignals[0]->bufferPut((t_path_request_routed)pathRequestRouted);
	outputSignals[1]->bufferPut((t_physical_topology)physicalTopology);
	
	return true;
};