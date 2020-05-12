#include "..\include_opaque\netxpto_20190130.h"

//using namespace std;     // (commented by Romil 08/05/2020)

#ifndef M_PI
#define M_PI  3.14159265358979323846
#endif

//########################################################################################################################################################
//######################################################### SIGNALS FUNCTIONS IMPLEMENTATION #############################################################
//########################################################################################################################################################

t_integer Signal::ready() {

	auto ready{ bufferLength };

	if (!bufferFull)
	{
		if (inPosition >= outPosition)
		{
			ready = inPosition - outPosition;
		}
		else
		{
			ready = bufferLength + inPosition - outPosition;
		}
	}

	return ready;
}

t_integer Signal::space() {

	return (bufferLength - ready());

}

template<typename T>							
void Signal::bufferPut(T value)
{
	(static_cast<T *>(buffer))[inPosition] = value;

	if (bufferFull)
	{
		outPosition = (outPosition + 1) % bufferLength;
	}

	inPosition = (inPosition + 1) % bufferLength;

	bufferEmpty = false;
	bufferFull = inPosition == outPosition;


	//if (bufferFull)      2019-04-13, de forma a gravar os sinais mesmo quando o buffer n�o enche
	if (inPosition == 0)
	{
		if (saveSignal)
		{
			if (!headerWritten) writeHeader();
			
			if (firstValueToBeSaved <= bufferLength)
			{
				if (!saveInAscii)
				{
					char *ptr = (char *)buffer;
					ptr = ptr + (firstValueToBeSaved - 1) * sizeof(T);
					std::ofstream fileHandler{ "./" + folderName + "/" + fileName, std::ios::out | std::ios::binary | std::ios::app };
					fileHandler.write(ptr, (bufferLength - (firstValueToBeSaved - 1)) * sizeof(T));
					fileHandler.close();
					firstValueToBeSaved = 1;
				}
				else
				{
					if (type == "DemandRequest")
					{
						t_demand_request *ptr = (t_demand_request *)buffer;
						ptr = ptr + (firstValueToBeSaved - 1);
						std::ofstream fileHandler("./" + folderName + "/" + fileName, std::ios::out | std::ios::app);

						for (auto dmd = firstValueToBeSaved; dmd <= bufferLength; dmd++)
						{
							fileHandler << "\t";
							fileHandler << ptr->demandIndex;
							fileHandler << "\t\t";
							fileHandler << ptr->sourceNode;
							fileHandler << "\t\t";
							fileHandler << ptr->destinationNode;
							fileHandler << "\t\t";
							fileHandler << ptr->oduType;
							fileHandler << "\t\t";
							if (ptr->survivabilityMethod == survivability_method::none)
								fileHandler << "none";
							else if (ptr->survivabilityMethod == survivability_method::protection_1_plus_1)
								fileHandler << "protection 1+1";
							else if (ptr->survivabilityMethod == survivability_method::restoration)
								fileHandler << "restoration";
							fileHandler << "\n";
							ptr++;
						}
						fileHandler.close();
						setFirstValueToBeSaved(1);
					}

					else if (type == "PathRequest")
					{
						t_path_request *ptr = (t_path_request *)buffer;
						ptr = ptr + (firstValueToBeSaved - 1);

						std::ofstream fileHandler("./" + folderName + "/" + fileName, std::ios::out | std::ios::app);

						fileHandler << "\n";
						fileHandler << "  requestIndex";
						fileHandler << "  |  ";
						fileHandler << "sourceNode";
						fileHandler << "  |  ";
						fileHandler << "destinationNode";
						fileHandler << " | ";
						fileHandler << "numberOfIntermediateNodes";
						fileHandler << " | ";
						fileHandler << "intermediateNodes";
						fileHandler << "\n\n";

						for (auto dmd = firstValueToBeSaved; dmd <= outPosition; dmd++)
						{
							fileHandler << "\t";
							fileHandler << ptr->requestIndex;
							fileHandler << "\t\t";
							fileHandler << ptr->sourceNode;
							fileHandler << "\t\t";
							fileHandler << ptr->destinationNode;
							fileHandler << "\t\t\t";
							fileHandler << ptr->numberOfIntermediateNodes;
							fileHandler << "\t\t\t";
							fileHandler << "[";
							for (size_t i = 0; i < ptr->intermediateNodes.size(); i++)
							{
								fileHandler << ptr->intermediateNodes[i];
								if (i < ptr->intermediateNodes.size() - 1)
									fileHandler << ",";
							}
							fileHandler << "]\n";
							ptr++;
						}
						fileHandler.close();
						setFirstValueToBeSaved(1);
					}

					else if (type == "PathRequestRouted")
					{
						t_path_request_routed *ptr = (t_path_request_routed *)buffer;
						ptr = ptr + (firstValueToBeSaved - 1);

						std::ofstream fileHandler("./" + folderName + "/" + fileName, std::ios::out | std::ios::app);
						for (auto dmd = firstValueToBeSaved; dmd <= outPosition; dmd++)
						{
							fileHandler << "\n";
							fileHandler << "### pathInformation ###";
							fileHandler << "\n\n";
							fileHandler << "   requestIndex";
							fileHandler << "   |   ";
							fileHandler << "routed";
							fileHandler << "   |   ";
							fileHandler << "numberOfLightPaths";
							fileHandler << "\n\n";

							fileHandler << "\t";
							fileHandler << (*ptr).pathInformation.requestIndex;
							fileHandler << "\t\t";
							if ((*ptr).pathInformation.routed == true)
								fileHandler << "true";
							else
								fileHandler << "false";
							fileHandler << "\t\t";
							fileHandler << (*ptr).pathInformation.numberOfLightPaths;
							fileHandler << "\n";

							fileHandler << "\n\n";

							fileHandler << "### lightPathsTable ###";
							fileHandler << "\n\n";
							fileHandler << "   sourceNode";
							fileHandler << " | ";
							fileHandler << "destinationNode";
							fileHandler << " | ";
							fileHandler << "numberOfIntermediateNodes";
							fileHandler << " | ";
							fileHandler << "intermediateNodes";
							fileHandler << " | ";
							fileHandler << "wavelength";
							fileHandler << "\n\n";

							for (size_t i = 0; i < (*ptr).lightPathsTable.size(); i++)
							{
								fileHandler << "\t";
								fileHandler << (*ptr).lightPathsTable[i].sourceNode;
								fileHandler << "\t\t";
								fileHandler << (*ptr).lightPathsTable[i].destinationNode;
								fileHandler << "\t\t\t";
								fileHandler << (*ptr).lightPathsTable[i].numberOfIntermediateNodes;
								fileHandler << "\t\t";
								fileHandler << "   [";
								for (size_t j = 0; j < (*ptr).lightPathsTable[i].intermediateNodes.size(); j++)
								{
									fileHandler << (*ptr).lightPathsTable[i].intermediateNodes[j];
									if (i < (*ptr).lightPathsTable[i].intermediateNodes.size() - 1)
										fileHandler << ",";
								}
								fileHandler << "]";
								fileHandler << "\t\t     ";
								fileHandler << (*ptr).lightPathsTable[i].wavelength;
								fileHandler << "\n";
							}
							fileHandler << "\n";
							fileHandler << "---------------------------------------------------------------------------------------------\n\n";
							ptr++;
						}
						fileHandler.close();
						setFirstValueToBeSaved(1);
					}

					else if (type == "LogicalTopology")
					{
						t_logical_topology *ptr = (t_logical_topology *)buffer;
						ptr = ptr + (firstValueToBeSaved - 1);

						std::ofstream fileHandler("./" + folderName + "/" + fileName, std::ios::out | std::ios::app);
						for (auto dmd = firstValueToBeSaved; dmd <= outPosition; dmd++)
						{
							fileHandler << "\n";
							fileHandler << "### logicalTopologyAdjacencyMatrix ###";
							fileHandler << "\n\n";

							t_integer nodes = (*ptr).logicalTopologyAdjacencyMatrix[0].size();
							for (t_integer line = 0; line < nodes; line++)
							{
								for (t_integer column = 0; column < nodes; column++)
								{
									fileHandler << (*ptr).logicalTopologyAdjacencyMatrix[line][column];
									fileHandler << "\t";
								}
								fileHandler << "\n";
							}
							fileHandler << "\n\n";

							fileHandler << "### paths ###";
							fileHandler << "\n\n";
							fileHandler << "  pathIndex";
							fileHandler << "   |   ";
							fileHandler << "sourceNode";
							fileHandler << "  |  ";
							fileHandler << "destinationNode";
							fileHandler << "  |  ";
							fileHandler << "capacity (ODU0s)";
							fileHandler << " | ";
							fileHandler << "numberOfLightPaths";
							fileHandler << " | ";
							fileHandler << "lightPathsIndex";
							fileHandler << "\n\n";

							for (size_t path = 0; path < (*ptr).paths.size(); path++)
							{
								fileHandler << "\t";
								fileHandler << (*ptr).paths[path].pathIndex;
								fileHandler << "\t\t";
								fileHandler << (*ptr).paths[path].sourceNode;
								fileHandler << "\t\t";
								fileHandler << (*ptr).paths[path].destinationNode;
								fileHandler << "\t\t";
								fileHandler << (*ptr).paths[path].capacity;
								fileHandler << "\t\t\t";
								fileHandler << (*ptr).paths[path].numberOfLightPaths;
								fileHandler << "\t\t";
								fileHandler << "[";
								for (size_t i = 0; i < (*ptr).paths[path].lightPathsIndex.size(); i++)
								{
									fileHandler << (*ptr).paths[path].lightPathsIndex[i];
									if (i < (*ptr).paths[path].lightPathsIndex.size() - 1)
										fileHandler << ",";
								}
								fileHandler << "]";
								fileHandler << "\n";
							}
							fileHandler << "\n\n";

							fileHandler << "### lightPaths ###";
							fileHandler << "\n\n";
							fileHandler << "lightPathIndex";
							fileHandler << " | ";
							fileHandler << "sourceNode";
							fileHandler << " | ";
							fileHandler << "destinationNode";
							fileHandler << " | ";
							fileHandler << "capacity (ODU0s)";
							fileHandler << " | ";
							fileHandler << "numberOfOpticalChannels";
							fileHandler << " | ";
							fileHandler << "opticalChannelsIndex";
							fileHandler << "\n\n";

							for (size_t lightPath = 0; lightPath < (*ptr).lightPaths.size(); lightPath++)
							{
								fileHandler << "\t";
								fileHandler << (*ptr).lightPaths[lightPath].lightPathIndex;
								fileHandler << "\t\t";
								fileHandler << (*ptr).lightPaths[lightPath].sourceNode;
								fileHandler << "\t\t";
								fileHandler << (*ptr).lightPaths[lightPath].destinationNode;
								fileHandler << "\t\t";
								fileHandler << (*ptr).lightPaths[lightPath].capacity;
								fileHandler << "\t\t";
								fileHandler << (*ptr).lightPaths[lightPath].numberOfOpticalChannels;
								fileHandler << "\t\t\t";
								fileHandler << "[";
								for (size_t i = 0; i < (*ptr).lightPaths[lightPath].opticalChannelsIndex.size(); i++)
								{
									fileHandler << (*ptr).lightPaths[lightPath].opticalChannelsIndex[i];
									if (i < (*ptr).lightPaths[lightPath].opticalChannelsIndex.size() - 1)
										fileHandler << ",";
								}
								fileHandler << "]";
								fileHandler << "\n";
							}
							fileHandler << "\n\n";

							fileHandler << "### opticalChannels ###";
							fileHandler << "\n\n";
							fileHandler << "opticalChannelIndex";
							fileHandler << " | ";
							fileHandler << "sourceNode";
							fileHandler << " | ";
							fileHandler << "destinationNode";
							fileHandler << " | ";
							fileHandler << "wavelength";
							fileHandler << " | ";
							fileHandler << "capacity (ODU0s)";
							fileHandler << " | ";
							fileHandler << "numberOfDemands";
							fileHandler << " | ";
							fileHandler << "demandsIndex";
							fileHandler << "\n\n";

							for (size_t opticalChannel = 0; opticalChannel < (*ptr).opticalChannels.size(); opticalChannel++)
							{
								fileHandler << "\t";
								fileHandler << (*ptr).opticalChannels[opticalChannel].opticalChannelIndex;
								fileHandler << "\t\t";
								fileHandler << (*ptr).opticalChannels[opticalChannel].sourceNode;
								fileHandler << "\t\t";
								fileHandler << (*ptr).opticalChannels[opticalChannel].destinationNode;
								fileHandler << "\t\t";
								fileHandler << (*ptr).opticalChannels[opticalChannel].wavelength;
								fileHandler << "\t\t";
								fileHandler << (*ptr).opticalChannels[opticalChannel].capacity;
								fileHandler << "\t\t";
								fileHandler << (*ptr).opticalChannels[opticalChannel].numberOfDemands;
								fileHandler << "\t\t";
								fileHandler << "[";
								for (size_t i = 0; i < (*ptr).opticalChannels[opticalChannel].demandsIndex.size(); i++)
								{
									fileHandler << (*ptr).opticalChannels[opticalChannel].demandsIndex[i];
									if (i < (*ptr).opticalChannels[opticalChannel].demandsIndex.size() - 1)
										fileHandler << ",";
								}
								fileHandler << "]";
								fileHandler << "\n";
							}
							fileHandler << "\n";
							fileHandler << "--------------------------------------------------------------------------------------------------------------------\n\n";
							ptr++;
						}
						fileHandler.close();
						setFirstValueToBeSaved(1);
					}

					else if (type == "PhysicalTopology")
					{
						t_physical_topology *ptr = (t_physical_topology *)buffer;
						ptr = ptr + (firstValueToBeSaved - 1);

						std::ofstream fileHandler("./" + folderName + "/" + fileName, std::ios::out | std::ios::app);
						for (auto pTopology = firstValueToBeSaved; pTopology <= outPosition; pTopology++)
						{
							fileHandler << "\n";
							fileHandler << "### physicalTopologyAdjacencyMatrix ####";
							fileHandler << "\n\n";
							t_integer nodes = (*ptr).physicalTopologyAdjacencyMatrix[0].size();
							for (t_integer line = 0; line < nodes; line++)
							{
								for (t_integer column = 0; column < nodes; column++)
								{
									fileHandler << (*ptr).physicalTopologyAdjacencyMatrix[line][column];
									fileHandler << "\t";
								}
								fileHandler << "\n";
							}
							fileHandler << "\n\n";

							fileHandler << "### opticalMultiplexSection ####";
							fileHandler << "\n\n";
							fileHandler << "    OMSIndex";
							fileHandler << " | ";
							fileHandler << "sourceNode";
							fileHandler << " | ";
							fileHandler << "destinationNode";
							fileHandler << " | ";
							fileHandler << "maximumNumberOfWavelengths";
							fileHandler << " | ";
							fileHandler << "wavelengths";
							fileHandler << " | ";
							fileHandler << "availableWavelengths";
							fileHandler << "\n\n";

							for (t_optical_multiplexing_system& oms : ptr->OMS)
							{
								fileHandler << "\t";
								fileHandler << oms.OMSIndex;
								fileHandler << "\t\t";
								fileHandler << oms.sourceNode;
								fileHandler << "\t\t";
								fileHandler << oms.destinationNode;
								fileHandler << "\t\t";
								fileHandler << oms.maximumNumberOfWavelengths;
								fileHandler << "\t\t";
								fileHandler << "[";
								for (size_t i = 0; i < oms.wavelengths.size(); i++)
								{
									fileHandler << oms.wavelengths[i];
									if (i < oms.wavelengths.size() - 1)
										fileHandler << ",";
								}
								fileHandler << "]";
								fileHandler << "\t\t";
								fileHandler << "[";
								for (size_t i = 0; i < oms.availableWavelengths.size(); i++)
								{
									fileHandler << oms.availableWavelengths[i];
									if (i < oms.availableWavelengths.size() - 1)
										fileHandler << ",";
								}
								fileHandler << "]";
								fileHandler << "\n";
							}
							fileHandler << "\n";
							fileHandler << "--------------------------------------------------------------------------------------------------------------------\n\n";
							ptr++;
						}
						fileHandler.close();
						setFirstValueToBeSaved(1);
					}

					else if (type == "DemandRequestRouted")
					{
						t_demand_request_routed *ptr = (t_demand_request_routed *)buffer;
						ptr = ptr + (firstValueToBeSaved - 1);

						std::ofstream fileHandler("./" + folderName + "/" + fileName, std::ios::out | std::ios::app);

						fileHandler << "\n";
						fileHandler << "   demandIndex";
						fileHandler << "    |    ";
						fileHandler << "routed";
						fileHandler << "    |    ";
						fileHandler << "pathIndex";
						fileHandler << "\n\n";
					
						for (auto dmd = firstValueToBeSaved; dmd <= outPosition; dmd++)
						{
							fileHandler << "\t";
							fileHandler << ptr->demandIndex;
							fileHandler << "\t\t";
							if (ptr->routed == true)
								fileHandler << "true";
							else if (ptr->routed == false)
								fileHandler << "false";
							fileHandler << "\t\t";
							fileHandler << ptr->pathIndex;
							fileHandler << "\n";
							ptr++;
						}
						fileHandler.close();
						setFirstValueToBeSaved(1);
					}
				}
			}
			else
			{
				firstValueToBeSaved = firstValueToBeSaved - bufferLength;
			}
		}
	}
}

template<typename T>
void Signal::bufferGet(T* valueAddr) {

	if (bufferEmpty) *valueAddr = T();

	*valueAddr = static_cast<T *>(buffer)[outPosition];

	bufferFull = false;

	outPosition = (outPosition + 1) % bufferLength;

	bufferEmpty = outPosition == inPosition;

	return;
};


void Signal::bufferGet() {

	bufferFull = false;

	outPosition = (outPosition + 1) % bufferLength;

	bufferEmpty = outPosition == inPosition;

	return;

}


void Signal::writeHeader(){

	if (headerWritten) return;

	if (saveSignal && (!fileName.empty())) {

		std::ofstream headerFile;

		headerFile.open("./" + folderName + "/" + fileName, std::ios::out);

		headerFile << "Signal type: " << getType() << "\n\n\n";
		headerFile << "--------------------------------------------------------------------------------------------------------------------\n\n";
		
		if (getType() == "DemandRequest")
		{
			headerFile << "\n";
			headerFile << "   demandIndex";
			headerFile << "  |  ";
			headerFile << "sourceNode";
			headerFile << " | ";
			headerFile << "destinationNode";
			headerFile << "  |  ";
			headerFile << "oduType";
			headerFile << "  |  ";
			headerFile << "survivabilityMethod";
			headerFile << "\n\n";
		}
				
		//headerFile << "// ### HEADER TERMINATOR ###\n";

		headerFile.close();

		headerWritten = true;
	}
};


void Signal::writeHeader(std::string signalPath){

	if (headerWritten) return;

	std::ofstream headerFile;

	if (saveSignal && (!fileName.empty())) {

		headerFile.open("./" + signalPath + "/" + fileName, std::ios::out);

		headerFile << "Signal type: " << getType() << "\n";
		headerFile << "Symbol Period (s): " << getSymbolPeriod() << "\n";
		headerFile << "Sampling Period (s): " << getSamplingPeriod() << "\n";

		headerFile << "// ### HEADER TERMINATOR ###\n";

		headerFile.close();
	}
};


/*
void Signal::bufferGet(t_binary *valueAddr) {
	*valueAddr = static_cast<t_binary *>(buffer)[outPosition];
	if (bufferFull) bufferFull = false;
	outPosition++;
	if (outPosition == bufferLength) outPosition = 0;
	if (outPosition == inPosition) bufferEmpty = true;
	return;
};
void Signal::bufferGet(t_integer *valueAddr) {
	*valueAddr = static_cast<t_integer *>(buffer)[outPosition];
	if (bufferFull) bufferFull = false;
	outPosition++;
	if (outPosition == bufferLength) outPosition = 0;
	if (outPosition == inPosition) bufferEmpty = true;
	return;
};
void Signal::bufferGet(t_real *valueAddr) {
	*valueAddr = static_cast<t_real *>(buffer)[outPosition];
	if (bufferFull) bufferFull = false;
	outPosition++;
	if (outPosition == bufferLength) outPosition = 0;
	if (outPosition == inPosition) bufferEmpty = true;
	return;
};
void Signal::bufferGet(t_complex *valueAddr) {
	*valueAddr = static_cast<t_complex *>(buffer)[outPosition];
	if (bufferFull) bufferFull = false;
	outPosition++;
	if (outPosition == bufferLength) outPosition = 0;
	if (outPosition == inPosition) bufferEmpty = true;
	return;
};
void Signal::bufferGet(t_complex_xy *valueAddr) {
	*valueAddr = static_cast<t_complex_xy *>(buffer)[outPosition];
	if (bufferFull) bufferFull = false;
	outPosition++;
	if (outPosition == bufferLength) outPosition = 0;
	if (outPosition == inPosition) bufferEmpty = true;
	return;
};
void Signal::bufferGet(t_photon *valueAddr) {
	*valueAddr = static_cast<t_photon *>(buffer)[outPosition];
	if (bufferFull) bufferFull = false;
	outPosition++;
	if (outPosition == bufferLength) outPosition = 0;
	if (outPosition == inPosition) bufferEmpty = true;
	return;
};
void Signal::bufferGet(t_photon_mp *valueAddr) {
	*valueAddr = static_cast<t_photon_mp *>(buffer)[outPosition];
	if (bufferFull) bufferFull = false;
	outPosition++;
	if (outPosition == bufferLength) outPosition = 0;
	if (outPosition == inPosition) bufferEmpty = true;
	return;
};
void Signal::bufferGet(t_photon_mp_xy *valueAddr) {
	*valueAddr = static_cast<t_photon_mp_xy *>(buffer)[outPosition];
	if (bufferFull) bufferFull = false;
	outPosition++;
	if (outPosition == bufferLength) outPosition = 0;
	if (outPosition == inPosition) bufferEmpty = true;
	return;
};
void Signal::bufferGet(t_message *valueAddr) {
	*valueAddr = static_cast<t_message *>(buffer)[outPosition];
	if (bufferFull) bufferFull = false;
	outPosition++;
	if (outPosition == bufferLength) outPosition = 0;
	if (outPosition == inPosition) bufferEmpty = true;
	return;
};
void Messages::bufferPut(t_message value) {
	(static_cast<t_message*>(buffer))[getInPosition()] = value;
	if (getBufferEmpty()) setBufferEmpty(false);
	setInPosition(getInPosition()+1);
	if (getInPosition() == getBufferLength()) {
		setInPosition(0);
		if (getSaveSignal()) {
			int fValueToBeSaved = getFirstValueToBeSaved();
			int bLength = getBufferLength();
			if (fValueToBeSaved <= getBufferLength()) {
				t_message *ptr = (t_message *)buffer;
				ptr = ptr + (fValueToBeSaved - 1);
				std::ofstream fileHandler("./" + getFolderName() + "/" + getFileName(), std::ios::out | std::ios::app);
				for (int msg = fValueToBeSaved; msg <= bLength; msg++) {
						for (auto fld = 0; fld < value.size(); fld++) {
							fileHandler << ptr->messageType + "\t" + ptr->messageDataLength + "\t" + ptr->messageData + "\n";
						}
						ptr++;
				}
				fileHandler.close();
				setFirstValueToBeSaved(1);
			}
			else {
				setFirstValueToBeSaved(fValueToBeSaved - bLength);
			}
		}
	}
	if (getInPosition() == getOutPosition()) setBufferFull(true);
};
*/

void Signal::close() {

	if (saveSignal && (inPosition >= firstValueToBeSaved)) 
	{

		if (!headerWritten) writeHeader();
		
		if (!(type == "Message")) 
		{
			char* ptr = (char *)buffer;

			std::ofstream fileHandler;
			fileHandler.open("./" + folderName + "/" + fileName, std::ios::out | std::ios::binary | std::ios::app);

			if (type == "DemandRequest")
			{
				t_demand_request *ptr = (t_demand_request *)buffer;
				ptr = ptr + (firstValueToBeSaved - 1);
				
				std::ofstream fileHandler("./" + folderName + "/" + fileName, std::ios::out | std::ios::app);
			
				fileHandler << "\n";
				fileHandler << "   demandIndex";
				fileHandler << "  |  ";
				fileHandler << "sourceNode";
				fileHandler << " | ";
				fileHandler << "destinationNode";
				fileHandler << "  |  ";
				fileHandler << "oduType";
				fileHandler << "  |  ";
				fileHandler << "survivabilityMethod";
				fileHandler << "\n\n";

				for (auto dmd = firstValueToBeSaved; dmd <= bufferLength; dmd++)
				{
					fileHandler << "\t";
					fileHandler << ptr->demandIndex;
					fileHandler << "\t\t";
					fileHandler << ptr->sourceNode;
					fileHandler << "\t\t";
					fileHandler << ptr->destinationNode;
					fileHandler << "\t\t";
					fileHandler << ptr->oduType;
					fileHandler << "\t\t";
					if (ptr->survivabilityMethod == survivability_method::none)
						fileHandler << "none";
					else if (ptr->survivabilityMethod == survivability_method::protection_1_plus_1)
						fileHandler << "protection 1+1";
					else if (ptr->survivabilityMethod == survivability_method::restoration)
						fileHandler << "restoration";
					fileHandler << "\n";
					ptr++;
				}
				setFirstValueToBeSaved(1);
			}

			else if (type == "PathRequest")
			{
				t_path_request *ptr = (t_path_request *)buffer;
				ptr = ptr + (firstValueToBeSaved - 1);

				std::ofstream fileHandler("./" + folderName + "/" + fileName, std::ios::out | std::ios::app);

				fileHandler << "\n";
				fileHandler << "  requestIndex";
				fileHandler << "  |  ";
				fileHandler << "sourceNode";
				fileHandler << "  |  ";
				fileHandler << "destinationNode";
				fileHandler << " | ";
				fileHandler << "numberOfIntermediateNodes";
				fileHandler << " | ";
				fileHandler << "intermediateNodes";
				fileHandler << "\n\n";

				for (auto dmd = firstValueToBeSaved; dmd <= outPosition; dmd++)
				{
					fileHandler << "\t";
					fileHandler << ptr->requestIndex;
					fileHandler << "\t\t";
					fileHandler << ptr->sourceNode;
					fileHandler << "\t\t";
					fileHandler << ptr->destinationNode;
					fileHandler << "\t\t\t";
					fileHandler << ptr->numberOfIntermediateNodes;
					fileHandler << "\t\t\t";
					fileHandler << "[";
					for (size_t i = 0; i < ptr->intermediateNodes.size(); i++) 
					{
						fileHandler << ptr->intermediateNodes[i];
						if (i < ptr->intermediateNodes.size() - 1)
							fileHandler << ",";
					}
					fileHandler << "]\n";
					ptr++;
				}
				setFirstValueToBeSaved(1);
			}

			else if (type == "PathRequestRouted") 
			{
				t_path_request_routed *ptr = (t_path_request_routed *)buffer;
				ptr = ptr + (firstValueToBeSaved - 1);

				std::ofstream fileHandler("./" + folderName + "/" + fileName, std::ios::out | std::ios::app);
				for (auto dmd = firstValueToBeSaved; dmd <= outPosition; dmd++) 
				{
					fileHandler << "\n";
					fileHandler << "### pathInformation ###";
					fileHandler << "\n\n";
					fileHandler << "   requestIndex";
					fileHandler << "   |   ";
					fileHandler << "routed";
					fileHandler << "   |   ";
					fileHandler << "numberOfLightPaths";
					fileHandler << "\n\n";

					fileHandler << "\t";
					fileHandler << (*ptr).pathInformation.requestIndex;
					fileHandler << "\t\t";
					if ((*ptr).pathInformation.routed == true)
						fileHandler << "true";
					else 
						fileHandler << "false";
					fileHandler << "\t\t";
					fileHandler << (*ptr).pathInformation.numberOfLightPaths;
					fileHandler << "\n";
				
					fileHandler << "\n\n";

					fileHandler << "### lightPathsTable ###";
					fileHandler << "\n\n";
					fileHandler << "   sourceNode";
					fileHandler << " | ";
					fileHandler << "destinationNode";
					fileHandler << " | ";
					fileHandler << "numberOfIntermediateNodes";
					fileHandler << " | ";
					fileHandler << "intermediateNodes";
					fileHandler << " | ";
					fileHandler << "wavelength";
					fileHandler << "\n\n";

					for (size_t i = 0; i < (*ptr).lightPathsTable.size(); i++)
					{
						fileHandler << "\t";
						fileHandler << (*ptr).lightPathsTable[i].sourceNode;
						fileHandler << "\t\t";
						fileHandler << (*ptr).lightPathsTable[i].destinationNode;
						fileHandler << "\t\t\t";
						fileHandler << (*ptr).lightPathsTable[i].numberOfIntermediateNodes;
						fileHandler << "\t\t";
						fileHandler << "   [";
						for (size_t j = 0; j < (*ptr).lightPathsTable[i].intermediateNodes.size(); j++)
						{
							fileHandler << (*ptr).lightPathsTable[i].intermediateNodes[j];
							if (i < (*ptr).lightPathsTable[i].intermediateNodes.size() - 1)
								fileHandler << ",";
						}
						fileHandler << "]";
						fileHandler << "\t\t     ";
						fileHandler << (*ptr).lightPathsTable[i].wavelength;
						fileHandler << "\n";
					}
					fileHandler << "\n";
					fileHandler << "---------------------------------------------------------------------------------------------\n\n";
					ptr++;
				}
				setFirstValueToBeSaved(1);
			}

			else if (type == "LogicalTopology") 
			{
				t_logical_topology *ptr = (t_logical_topology *)buffer;
				ptr = ptr + (firstValueToBeSaved - 1);
				
				std::ofstream fileHandler("./" + folderName + "/" + fileName, std::ios::out | std::ios::app);
				for (auto dmd = firstValueToBeSaved; dmd <= outPosition; dmd++) 
				{	
					fileHandler << "\n";
					fileHandler << "### logicalTopologyAdjacencyMatrix ###";
					fileHandler << "\n\n";

					t_integer nodes = (*ptr).logicalTopologyAdjacencyMatrix[0].size();
					for (t_integer line = 0; line < nodes ; line++) 
					{
						for (t_integer column = 0; column < nodes; column++) 
						{
							fileHandler << (*ptr).logicalTopologyAdjacencyMatrix[line][column];
							fileHandler << "\t";
						}
						fileHandler << "\n";
					}
					fileHandler << "\n\n";
					
					fileHandler << "### paths ###";
					fileHandler << "\n\n";
					fileHandler << "  pathIndex";
					fileHandler << "   |   ";
					fileHandler << "sourceNode";
					fileHandler << "  |  ";
					fileHandler << "destinationNode";
					fileHandler << "  |  ";
					fileHandler << "capacity (ODU0s)";
					fileHandler << " | ";
					fileHandler << "numberOfLightPaths";
					fileHandler << " | ";
					fileHandler << "lightPathsIndex";
					fileHandler << "\n\n";

					for (size_t path = 0; path < (*ptr).paths.size(); path++)
					{
						fileHandler << "\t";
						fileHandler << (*ptr).paths[path].pathIndex;
						fileHandler << "\t\t";
						fileHandler << (*ptr).paths[path].sourceNode;
						fileHandler << "\t\t";
						fileHandler << (*ptr).paths[path].destinationNode;
						fileHandler << "\t\t";
						fileHandler << (*ptr).paths[path].capacity;
						fileHandler << "\t\t\t";
						fileHandler << (*ptr).paths[path].numberOfLightPaths;
						fileHandler << "\t\t";
						fileHandler << "[";
						for (size_t i = 0; i < (*ptr).paths[path].lightPathsIndex.size(); i++)
						{
							fileHandler << (*ptr).paths[path].lightPathsIndex[i];
							if (i < (*ptr).paths[path].lightPathsIndex.size() - 1)
								fileHandler << ",";
						}
						fileHandler << "]";
						fileHandler << "\n";
					}
					fileHandler << "\n\n";

					fileHandler << "### lightPaths ###";
					fileHandler << "\n\n";
					fileHandler << "lightPathIndex";
					fileHandler << " | ";
					fileHandler << "sourceNode";
					fileHandler << " | ";
					fileHandler << "destinationNode";
					fileHandler << " | ";
					fileHandler << "capacity (ODU0s)";
					fileHandler << " | ";
					fileHandler << "numberOfOpticalChannels";
					fileHandler << " | ";
					fileHandler << "opticalChannelsIndex";
					fileHandler << "\n\n";

					for (size_t lightPath = 0; lightPath < (*ptr).lightPaths.size(); lightPath++)
					{
						fileHandler << "\t";
						fileHandler << (*ptr).lightPaths[lightPath].lightPathIndex;
						fileHandler << "\t\t";
						fileHandler << (*ptr).lightPaths[lightPath].sourceNode;
						fileHandler << "\t\t";
						fileHandler << (*ptr).lightPaths[lightPath].destinationNode;
						fileHandler << "\t\t";
						fileHandler << (*ptr).lightPaths[lightPath].capacity;
						fileHandler << "\t\t";
						fileHandler << (*ptr).lightPaths[lightPath].numberOfOpticalChannels;
						fileHandler << "\t\t\t";
						fileHandler << "[";
						for (size_t i = 0; i < (*ptr).lightPaths[lightPath].opticalChannelsIndex.size(); i++)
						{
							fileHandler << (*ptr).lightPaths[lightPath].opticalChannelsIndex[i];
							if (i < (*ptr).lightPaths[lightPath].opticalChannelsIndex.size() - 1)
								fileHandler << ",";
						}
						fileHandler << "]";
						fileHandler << "\n";
					}
					fileHandler << "\n\n";

					fileHandler << "### opticalChannels ###";
					fileHandler << "\n\n";
					fileHandler << "opticalChannelIndex";
					fileHandler << " | ";
					fileHandler << "sourceNode";
					fileHandler << " | ";
					fileHandler << "destinationNode";
					fileHandler << " | ";
					fileHandler << "wavelength";
					fileHandler << " | ";
					fileHandler << "capacity (ODU0s)";
					fileHandler << " | ";
					fileHandler << "numberOfDemands";
					fileHandler << " | ";
					fileHandler << "demandsIndex";
					fileHandler << "\n\n";

					for (size_t opticalChannel = 0; opticalChannel < (*ptr).opticalChannels.size(); opticalChannel++)
					{
						fileHandler << "\t";
						fileHandler << (*ptr).opticalChannels[opticalChannel].opticalChannelIndex;
						fileHandler << "\t\t";
						fileHandler << (*ptr).opticalChannels[opticalChannel].sourceNode;
						fileHandler << "\t\t";
						fileHandler << (*ptr).opticalChannels[opticalChannel].destinationNode;
						fileHandler << "\t\t";
						fileHandler << (*ptr).opticalChannels[opticalChannel].wavelength;
						fileHandler << "\t\t";
						fileHandler << (*ptr).opticalChannels[opticalChannel].capacity;
						fileHandler << "\t\t";
						fileHandler << (*ptr).opticalChannels[opticalChannel].numberOfDemands;
						fileHandler << "\t\t";
						fileHandler << "[";
						for (size_t i = 0; i < (*ptr).opticalChannels[opticalChannel].demandsIndex.size(); i++)
						{
							fileHandler << (*ptr).opticalChannels[opticalChannel].demandsIndex[i];
							if (i < (*ptr).opticalChannels[opticalChannel].demandsIndex.size() - 1)
								fileHandler << ",";
						}
						fileHandler << "]";
						fileHandler << "\n";
					}
					fileHandler << "\n";
					fileHandler << "--------------------------------------------------------------------------------------------------------------------\n\n";
					ptr++;
				}
				setFirstValueToBeSaved(1);
			}
			
			else if (type == "PhysicalTopology") 
			{
				t_physical_topology *ptr = (t_physical_topology *)buffer;
				ptr = ptr + (firstValueToBeSaved - 1);

				std::ofstream fileHandler("./" + folderName + "/" + fileName, std::ios::out | std::ios::app);
				for (auto pTopology = firstValueToBeSaved; pTopology <= outPosition; pTopology++) 
				{
					fileHandler << "\n";
					fileHandler << "### physicalTopologyAdjacencyMatrix ####";
					fileHandler << "\n\n";
					t_integer nodes = (*ptr).physicalTopologyAdjacencyMatrix[0].size();
					for (t_integer line = 0; line < nodes; line++) 
					{
						for (t_integer column = 0; column < nodes; column++) 
						{
							fileHandler << (*ptr).physicalTopologyAdjacencyMatrix[line][column];
							fileHandler << "\t";
						}
						fileHandler << "\n";
					}
					fileHandler << "\n\n";

					fileHandler << "### opticalMultiplexSection ####";
					fileHandler << "\n\n";
					fileHandler << "    OMSIndex";
					fileHandler << " | ";
					fileHandler << "sourceNode";
					fileHandler << " | ";
					fileHandler << "destinationNode";
					fileHandler << " | ";
					fileHandler << "maximumNumberOfWavelengths";
					fileHandler << " | ";
					fileHandler << "wavelengths";
					fileHandler << " | ";
					fileHandler << "availableWavelengths";
					fileHandler << " | ";
					fileHandler << "numberOfAmplifiers";
					fileHandler << "\n\n";

					for (t_optical_multiplexing_system& oms : ptr->OMS) 
					{
						fileHandler << "\t";
						fileHandler << oms.OMSIndex;
						fileHandler << "\t\t";
						fileHandler << oms.sourceNode;
						fileHandler << "\t\t";
						fileHandler << oms.destinationNode;
						fileHandler << "\t\t";
						fileHandler << oms.maximumNumberOfWavelengths;
						fileHandler << "\t\t";
						fileHandler << "[";
						for (size_t i = 0; i < oms.wavelengths.size(); i++) 
						{
							fileHandler << oms.wavelengths[i];
							if (i < oms.wavelengths.size() - 1)
								fileHandler << ",";
						}
						fileHandler << "]";
						fileHandler << "\t\t";
						fileHandler << "[";
						for (size_t i = 0; i < oms.availableWavelengths.size(); i++) 
						{
							fileHandler << oms.availableWavelengths[i];
							if (i <  oms.availableWavelengths.size() - 1)
								fileHandler << ",";
						}
						fileHandler << "]";
						fileHandler << "\t\t\t";
						fileHandler << oms.numberOfAmplifiers;
						fileHandler << "\n";
					}
					fileHandler << "\n";
					fileHandler << "-----------------------------------------------------------------------------------------------------------------------------------\n\n";
					ptr++;
				}
				setFirstValueToBeSaved(1);
			}

			else if (type == "DemandRequestRouted") 
			{
				t_demand_request_routed *ptr = (t_demand_request_routed *)buffer;
				ptr = ptr + (firstValueToBeSaved - 1);

				std::ofstream fileHandler("./" + folderName + "/" + fileName, std::ios::out | std::ios::app);

				fileHandler << "\n";
				fileHandler << "   demandIndex";
				fileHandler << "    |    ";
				fileHandler << "routed";
				fileHandler << "    |    ";
				fileHandler << "pathIndex";
				fileHandler << "\n\n";
			
				for (auto dmd = firstValueToBeSaved; dmd <= outPosition; dmd++) 
				{
					fileHandler << "\t";
					fileHandler << ptr->demandIndex;
					fileHandler << "\t\t";
					if (ptr->routed == true) 
						fileHandler << "true";
					else if (ptr->routed == false)
						fileHandler << "false";
					fileHandler << "\t\t";
					fileHandler << ptr->pathIndex;
					fileHandler << "\n";
					ptr++;
				}
				setFirstValueToBeSaved(1);
			}

			else if (type == "Binary") {
				ptr = ptr + (firstValueToBeSaved - 1) * sizeof(t_binary);
				fileHandler.write((char *)ptr, (inPosition - (firstValueToBeSaved - 1)) * sizeof(t_binary));
			}
			else if (type == "TimeContinuousAmplitudeContinuousComplex" || type == "BandpassSignal") {
				ptr = ptr + (firstValueToBeSaved - 1) * sizeof(t_complex);
				fileHandler.write((char *)ptr, (inPosition - (firstValueToBeSaved - 1)) * sizeof(t_complex));
			}
			else {
				ptr = ptr + (firstValueToBeSaved - 1) * sizeof(t_real);
				fileHandler.write((char *)ptr, (inPosition - (firstValueToBeSaved - 1)) * sizeof(t_real));
			}
			inPosition = 0;
			fileHandler.close();
		}
		else if (type == "Message") {
			auto fValueToBeSaved = getFirstValueToBeSaved();
			int bLength = getBufferLength();

			if (fValueToBeSaved < (inPosition + 1)) {
				t_message *ptr = (t_message *)buffer;
				ptr = ptr + (fValueToBeSaved - 1);

				std::ofstream fileHandler("./" + folderName + "/" + fileName, std::ios::out | std::ios::app);

				for (auto msg = fValueToBeSaved; msg < (inPosition + 1); msg++) {
					for (auto fld = 0; fld < (*ptr).size(); fld++) {
						fileHandler << ptr->messageType + "\t" + ptr->messageDataLength + "\t" + ptr->messageData + "\n";
					}
					ptr++;
				}
				fileHandler.close();
				setFirstValueToBeSaved(1);
			}
		}
	}
};


//########################################################################################################################################################
//###################################################### GENERAL BLOCKS FUNCTIONS IMPLEMENTATION #########################################################
//########################################################################################################################################################

Block::Block(std::initializer_list<Signal*> InputSig, std::initializer_list<Signal*> OutputSig) {

	numberOfInputSignals = (int) InputSig.size();
	numberOfOutputSignals = (int) OutputSig.size();

	inputSignals = InputSig;
	outputSignals = OutputSig;
}

Block::Block(std::vector<Signal*> &InputSig, std::vector<Signal*> &OutputSig) {

	numberOfInputSignals = (int)InputSig.size();
	numberOfOutputSignals = (int)OutputSig.size();

	inputSignals = InputSig;
	outputSignals = OutputSig;
}

void Block::initializeBlock(void) {

	initialize();

}

bool Block::runBlock(void) {

	return false;

}

void Block::terminateBlock(void) {

  terminate();

  for (auto i = 0; i < numberOfInputSignals; i++)
    inputSignals[i]->close();

}

void Block::closeOutputSignals(void) {

	for (auto i = 0; i < numberOfOutputSignals; i++)
		outputSignals[i]->close();

}


//########################################################################################################################################################
//################################################ GENERAL SUPER BLOCKS FUNCTIONS IMPLEMENTATION #########################################################
//########################################################################################################################################################

void SuperBlock::initialize(void){
/*
	for (auto k : blocks) moduleBlocks.push_back(&k);
	for (int unsigned i = 0; i < moduleBlocks.size(); i++) {
		moduleBlocks[i]->initializeBlock();
	}
	for (int unsigned j = 0; j<(moduleBlocks[moduleBlocks.size() - 1]->outputSignals).size(); j++)
		moduleBlocks[moduleBlocks.size() - 1]->outputSignals[j]->writeHeader();
	
	for (unsigned int i = 0; i < outputSignals.size(); i++) {
		outputSignals[i]->setSymbolPeriod(moduleBlocks[moduleBlocks.size() - 1]->outputSignals[i]->getSymbolPeriod());
		outputSignals[i]->setSamplingPeriod(moduleBlocks[moduleBlocks.size() - 1]->outputSignals[i]->getSamplingPeriod());
		outputSignals[i]->setFirstValueToBeSaved(moduleBlocks[moduleBlocks.size() - 1]->outputSignals[i]->getFirstValueToBeSaved());
	}
	*/
};

bool SuperBlock::runBlock(std::string signalPath) {

//	bool alive = superBlockSystem.run();

	/////////////////////////////////
	/*2018-04-18*/
	//Creates the signals folder if it doesn't exist
	if (!std::filesystem::is_directory(signalPath) || !std::filesystem::exists(signalPath)) {
		std::filesystem::create_directory(signalPath);
	}
	//Debug information
	clock_t start;
	std::string separator = "|";
	if (superBlockSystem.getLogValue() && superBlockSystem.getOpenFile())
		logFileSP.open("./" + signalPath + "/" + superBlockSystem.getLogFileName());
	//Writes which input parameters have been 
	logFileSP << "The following input parameters were loaded from the configuration file:" << std::endl;
	for (std::string p : superBlockSystem.getLoadedInputParameters()) {
		logFileSP << p << std::endl;
	}
	//logFile << "-------------------------------------------------------" << std::endl;

	bool systemAlive{ false };
	std::vector<Block *> SystemBlocks = superBlockSystem.getSystemBlocks();

	for (unsigned int i = 0; i < SystemBlocks.size(); i++) {
			// Writes debug information
			if (superBlockSystem.getLogValue()) {
				time_t t_now = time(0);
				struct tm now;
				localtime_s(&now, &t_now);
				char buffer[20];
				snprintf(buffer, 20, "%04d-%02d-%02d %02d:%02d:%02d", 1900 + now.tm_year, now.tm_mon + 1, now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec);
				logFileSP << "-----------------------------------------------------------------\n";
				logFileSP << "########## " << std::string(typeid(*SystemBlocks[i]).name()).substr(6) << " ##########" << std::endl; // Prints block name e.g. "Add"
				logFileSP << "Block start time: " << buffer << std::endl;
				// Prints line for each input signal in the current block being executed
				logFileSP << "Input Signals: " << std::endl;
				for (Signal *b : SystemBlocks[i]->inputSignals) {
					std::string filename = (*b).getFileName(); // Gets filename e.g: "S8.sgn"
					logFileSP << std::string(typeid(*SystemBlocks[i]).name()).substr(6) << separator // Prints block name e.g. "Add"
						<< filename.substr(0, filename.find(".")) << separator // Prints the formated filename e.g. "S8.sgn" becomes "S8"
						<< "ready=" << (*b).ready() << std::endl; // Prints the amount of bits ready to be processed 
				}
				// Prints line for each output signal in the current block being executed
				logFileSP << "Output Signals: " << std::endl;
				for (Signal *b : SystemBlocks[i]->outputSignals) {
					std::string filename = (*b).getFileName(); // Gets filename e.g: "S8.sgn"
					logFileSP << std::string(typeid(*SystemBlocks[i]).name()).substr(6) << separator // Prints block name e.g. "Add"
						<< filename.substr(0, filename.find(".")) << separator // Prints the formated filename e.g. "S8.sgn" becomes "S8"
						<< "space=" << (*b).space() << std::endl; // Prints the amount of bits ready to be processed 
				}
				start = clock(); //Counts the time taken to run the current block
			}

			bool aux = SystemBlocks[i]->runBlock();
			systemAlive = (systemAlive || aux);
			if (systemAlive) systemAlive = true;

			if (superBlockSystem.getLogValue())

			{
				logFileSP << "-----------------------------------------------------------------\n";
				logFileSP << "Elapsed time: " << (float)(clock() - start) << " milliseconds" << std::endl;
				logFileSP << "-----------------------------------------------------------------\n";
				// Prints line for each input signal in the current block being executed
				logFileSP << "Input Signals: " << std::endl;
				for (Signal *b : SystemBlocks[i]->inputSignals) {
					std::string filename = (*b).getFileName(); // Gets filename e.g: "S8.sgn"
					logFileSP << std::string(typeid(*SystemBlocks[i]).name()).substr(6) << separator // Prints block name e.g. "Add"
						<< filename.substr(0, filename.find(".")) << separator // Prints the formated filename e.g. "S8.sgn" becomes "S8"
						<< "ready=" << (*b).ready() << std::endl; // Prints the amount of bits ready to be processed 
				}
				// Prints line for each output signal in the current block being executed
				logFileSP << "Output Signals: " << std::endl;
				for (Signal *b : SystemBlocks[i]->outputSignals) {
					std::string filename = (*b).getFileName(); // Gets filename e.g: "S8.sgn"
					logFileSP << std::string(typeid(*SystemBlocks[i]).name()).substr(6) << separator // Prints block name e.g. "Add"
						<< filename.substr(0, filename.find(".")) << separator // Prints the formated filename e.g. "S8.sgn" becomes "S8"
						<< "space=" << (*b).space() << std::endl; // Prints the amount of bits ready to be processed 
				}
				logFileSP << std::endl << std::endl;
			}
		}

	////////////////////////////////

	// isto \E9 para ser limpo mas por qualquer motivo sem isto n\E3o compila!
	bool proceed{ true };

	do {

		proceed = false;

		//for (unsigned int i = 0; i < moduleBlocks.size(); i++) {
			//bool aux = moduleBlocks[i]->runBlock();
		/*for (auto k: blocks)
		{
			bool aux = k.runBlock();
			proceed = (proceed || aux);
			alive = (alive || proceed);
		}
		*/
		for (unsigned int i = 0; i < 0; i++) {
			//int ready = blocks[blocks.size() - 1].outputSignals[i]->ready();
			int ready = 0;
			int space = outputSignals[i]->space();
			int length = (ready <= space) ? ready : space;

			signal_value_type sType = moduleBlocks[moduleBlocks.size() - 1]->outputSignals[i]->getValueType();
			switch (sType) {
			case signal_value_type::t_binary:
				for (int j = 0; j < length; j++) {
					t_binary signalValue;
					moduleBlocks[moduleBlocks.size() - 1]->outputSignals[i]->bufferGet(&signalValue);
					outputSignals[i]->bufferPut(signalValue);
				}
				break;

			case signal_value_type::t_real:
				for (int j = 0; j < length; j++) {
					t_real signalValue;
					moduleBlocks[moduleBlocks.size() - 1]->outputSignals[i]->bufferGet(&signalValue);
					outputSignals[i]->bufferPut(signalValue);
				}
				break;

			case signal_value_type::t_complex:
				for (int j = 0; j < length; j++) {
					t_complex signalValue;
					moduleBlocks[moduleBlocks.size() - 1]->outputSignals[i]->bufferGet(&signalValue);
					outputSignals[i]->bufferPut(signalValue);
				}
				break;
				case signal_value_type::t_complex_xy:
				for (int j = 0; j < length; j++) {
					t_complex_xy signalValueXY;
					moduleBlocks[moduleBlocks.size() - 1]->outputSignals[i]->bufferGet(&signalValueXY);
					outputSignals[i]->bufferPut(signalValueXY);
				}
				break;
				case signal_value_type::t_photon_mp_xy:
					for (int j = 0; j < length; j++) {
						t_photon_mp_xy signalValueXY;
						moduleBlocks[moduleBlocks.size() - 1]->outputSignals[i]->bufferGet(&signalValueXY);
						outputSignals[i]->bufferPut(signalValueXY);
					}
				break;
/*			case signal_value_type::t_message:
				for (int j = 0; j < length; j++) {
					t_message signalValue;
					moduleBlocks[moduleBlocks.size() - 1]->outputSignals[i]->bufferGet(&signalValue);
					outputSignals[i]->bufferPut(signalValue);
				}
				break;*/
				case signal_value_type::t_logical_topology:
					for (int j = 0; j < length; j++) {
						t_logical_topology signalLogicalTopology;
						moduleBlocks[moduleBlocks.size() - 1]->outputSignals[i]->bufferGet(&signalLogicalTopology);
						outputSignals[i]->bufferPut(signalLogicalTopology);
					}
					break;
				case signal_value_type::t_physical_topology:
					for (int j = 0; j < length; j++) {
						t_physical_topology signalPhysicalTopology;
						moduleBlocks[moduleBlocks.size() - 1]->outputSignals[i]->bufferGet(&signalPhysicalTopology);
						outputSignals[i]->bufferPut(signalPhysicalTopology);
					}
					break;
				case signal_value_type::t_demand_request:
					for (int j = 0; j < length; j++) {
						t_demand_request signalDemandRequest;
						moduleBlocks[moduleBlocks.size() - 1]->outputSignals[i]->bufferGet(&signalDemandRequest);
						outputSignals[i]->bufferPut(signalDemandRequest);
					}
					break;
				case signal_value_type::t_demand_request_routed:
					for (int j = 0; j < length; j++) {
						t_demand_request_routed signalDemandRequestRouted;
						moduleBlocks[moduleBlocks.size() - 1]->outputSignals[i]->bufferGet(&signalDemandRequestRouted);
						outputSignals[i]->bufferPut(signalDemandRequestRouted);
					}
					break;
				case signal_value_type::t_path_request:
					for (int j = 0; j < length; j++) {
						t_path_request signalPathRequest;
						moduleBlocks[moduleBlocks.size() - 1]->outputSignals[i]->bufferGet(&signalPathRequest);
						outputSignals[i]->bufferPut(signalPathRequest);
					}
					break;
				case signal_value_type::t_path_request_routed:
					for (int j = 0; j < length; j++) {
						t_path_request_routed signalPathRequestRouted;
						moduleBlocks[moduleBlocks.size() - 1]->outputSignals[i]->bufferGet(&signalPathRequestRouted);
						outputSignals[i]->bufferPut(signalPathRequestRouted);
					}
					break;
			default:
				std::cerr << "ERRO: netxpto_20190130.cpp (SuperBlock)" << "\n";
				return false;
			}
		}
		
	} while (proceed);
	
	return systemAlive;
}


void SuperBlock::terminate() {

	/*
	for (int unsigned i = 0; i < moduleBlocks.size(); i++) {
		moduleBlocks[i]->terminateBlock();
	}
	for (int unsigned j = 0; j<(moduleBlocks[moduleBlocks.size() - 1]->outputSignals).size(); j++)
		moduleBlocks[moduleBlocks.size() - 1]->outputSignals[j]->close();
    */
	superBlockSystem.terminateSuperBlock();
}

void SuperBlock::setSaveInternalSignals(bool sInternalSignals) {

	for (int unsigned i = 0; i < moduleBlocks.size(); i++) {
		for (int unsigned j = 0; j < (moduleBlocks[i]->inputSignals).size(); j++)
			moduleBlocks[i]->inputSignals[j]->setSaveSignal(sInternalSignals);
		for (int unsigned j = 0; j < (moduleBlocks[i]->outputSignals).size(); j++)
			moduleBlocks[i]->outputSignals[j]->setSaveSignal(sInternalSignals);
	}
}

//########################################################################################################################################################
//###################################################################### LINEAR FILTERS ##################################################################
//########################################################################################################################################################

void FIR_Filter::initializeFIR_Filter(void) {

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setSamplesPerSymbol(inputSignals[0]->getSamplesPerSymbol());

	if (!getSeeBeginningOfImpulseResponse()) {
		int aux = (int)(((double)impulseResponseLength) / 2) + 1;
		outputSignals[0]->setFirstValueToBeSaved(aux);
	}

	delayLine.resize(impulseResponseLength, 0);

	if (saveImpulseResponse) {
		std::ofstream fileHandler("./signals/" + impulseResponseFilename, std::ios::out);
		fileHandler << "// ### HEADER TERMINATOR ###\n";

		t_real t;
		double samplingPeriod = inputSignals[0]->getSamplingPeriod();
		for (int i = 0; i < impulseResponseLength; i++) {
			//t = -impulseResponseLength / 2 * samplingPeriod + i * samplingPeriod;
			t = i * samplingPeriod;
			fileHandler << t << " " << impulseResponse[i] << "\n";
		}
		fileHandler.close();
	}

};

bool FIR_Filter::runBlock(void) {

	
	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();

	int process = std::min(ready, space);

	if (process == 0) return false;

	for (int i = 0; i < process; i++) {
		t_real val;
		(inputSignals[0])->bufferGet(&val);
		/*if (val != 0) {
			std::vector<t_real> aux(impulseResponseLength, 0.0);
			transform(impulseResponse.begin(), impulseResponse.end(), aux.begin(), bind1st(std::multiplies<t_real>(), val));
			transform(aux.begin(), aux.end(), delayLine.begin(), delayLine.begin(), std::plus<t_real>());
		}*/
		if (val != 0)
		{
			std::vector<t_real> aux(impulseResponseLength, 0.0);
			transform(impulseResponse.begin(), impulseResponse.end(), aux.begin(), std::bind(std::multiplies<t_real>(), val, std::placeholders::_1));
			transform(aux.begin(), aux.end(), delayLine.begin(), delayLine.begin(), std::plus<t_real>());
		}
		outputSignals[0]->bufferPut((t_real)(delayLine[0]));
		rotate(delayLine.begin(), delayLine.begin() + 1, delayLine.end());
		delayLine[impulseResponseLength - 1] = 0.0;
	}

	return true;
};


void FD_Filter::initializeFD_Filter(void) {

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setSamplesPerSymbol(inputSignals[0]->getSamplesPerSymbol());

/*	if (!getSeeBeginningOfImpulseResponse()) {
		int aux = (int)(((double)impulseResponseLength) / 2) + 1;
		outputSignals[0]->setFirstValueToBeSaved(aux);
	}*/

/*	delayLine.resize(impulseResponseLength, 0);*/

	
	inputBufferTimeDomain.resize(inputBufferTimeDomainLength);
	outputBufferTimeDomain.resize(inputBufferTimeDomainLength);

	inputBufferPointer = inputBufferTimeDomainLength / 2 ;
	outputBufferPointer = inputBufferTimeDomainLength;

	//raisedCosineTF(transferFunction, transferFunctionLength, rollOffFactor, samplingPeriod, symbolPeriod);

	if (saveTransferFunction) {
		std::ofstream fileHandler("./signals/" + transferFunctionFilename, std::ios::out);
		fileHandler << "// ### HEADER TERMINATOR ###\n";

		double samplingPeriod = inputSignals[0]->getSamplingPeriod();
		t_real fWindow = 1/samplingPeriod;
		t_real df = fWindow / transferFunctionLength;

		t_real f;
		for (int k = 0; k < transferFunctionLength; k++) {
			f = -fWindow / 2 + k * df;
			fileHandler << f << " " << transferFunction[k] << "\n";
		}
		fileHandler.close();
	}

};

/*void FD_Filter::OverlapSaveMethod(void) {
	int NFFT = transferFunctionLength;
	int Nblocks = 2 * (inputBufferTimeDomain.size() / NFFT);
	std::vector<double> real_temp(NFFT, 0);
	std::vector<double> imag_temp(NFFT, 0);
	std::vector<double> real_temp_copy(NFFT, 0);
	std::vector<double> imag_temp_copy(NFFT, 0);
	std::vector<t_real> H_real(NFFT, 0);
	std::vector<t_real> H_imag(NFFT, 0);
	Fft fft;
	ComplexMult CMult;
	CMult.ComplexVect2ReImVect(transferFunction, H_real, H_imag);
	//fft.directTransform(h_real, h_imag);
	for (int k = 0; k < Nblocks; k++) {
		if (k == Nblocks - 1) {
			copy(inputBufferTimeDomain.begin(), inputBufferTimeDomain.begin() + (NFFT / 2), real_temp.begin() + (NFFT / 2));
			//copy(imag_in.begin(), imag_in.begin() + (NFFT / 2), imag_temp.begin() + (NFFT / 2));
			copy(inputBufferTimeDomain.end() - (NFFT / 2), inputBufferTimeDomain.end(), real_temp.begin());
			//copy(imag_in.end() - (NFFT / 2), imag_in.end(), imag_temp.begin());
		}
		else {
			copy(inputBufferTimeDomain.begin() + k*(NFFT / 2), inputBufferTimeDomain.begin() + ((k + 1)*NFFT - k*(NFFT / 2)), real_temp.begin());
			//copy(imag_in.begin() + k*(NFFT / 2), imag_in.begin() + ((k + 1)*NFFT - k*(NFFT / 2)), imag_temp.begin());
		}
		// coping real_temp/imag_temp into real_temp_copy/imag_temp_copy vectors
		real_temp_copy = real_temp;
		imag_temp_copy = imag_temp;
		// Computation of FFT of each block
		fft.directTransform(real_temp_copy, imag_temp_copy);
		// Multiplication with the transfer std::function
		CMult.CMultVector_Loop(real_temp_copy, imag_temp_copy, H_real, H_imag);
		// Computation of IFFT of each block
		fft.inverseTransform(real_temp_copy, imag_temp_copy);
		// Removing the samples symetrically and assign to the output
		if (k == Nblocks - 1) {
			copy(real_temp_copy.begin() + (NFFT / 4), real_temp_copy.begin() + (NFFT / 2), outputBufferTimeDomain.end() - (NFFT / 4));
			//copy(imag_temp_copy.begin() + (NFFT / 4), imag_temp_copy.begin() + (NFFT / 2), imag_out.end() - (NFFT / 4));
			copy(real_temp_copy.begin() + (NFFT / 2), real_temp_copy.begin() + NFFT, outputBufferTimeDomain.begin());
			//copy(imag_temp_copy.begin() + (NFFT / 2), imag_temp_copy.begin() + NFFT, imag_out.begin());
		}
		else {
			copy(real_temp_copy.begin() + (NFFT / 4), real_temp_copy.end() - (NFFT / 4), outputBufferTimeDomain.begin() + ((NFFT / 4) + k*(NFFT / 2)));
			//copy(imag_temp_copy.begin() + (NFFT / 4), imag_temp_copy.end() - (NFFT / 4), imag_out.begin() + ((NFFT / 4) + k*(NFFT / 2)));
		}
	}
}
void FD_Filter::overlapSaveZPRealIn(void) {
	int NFFT = transferFunctionLength;
	int Nblocks = 2 * (inputBufferTimeDomain.size() / NFFT);
	std::vector<double> var_temp(NFFT, 0);
	std::vector<double> real_temp_copy(NFFT, 0);
	std::vector<double> imag_temp_copy(NFFT, 0);
	std::vector<double> H_real(NFFT, 0);
	std::vector<double> H_imag(NFFT, 0);
	std::vector<double> v_in_temp(NFFT / 2, 0);
	std::vector<double> v_out_temp(NFFT / 2, 0);
	Fft fft;
	ComplexMult CMult;
	CMult.ComplexVect2ReImVect(transferFunction, H_real, H_imag);
	for (int k = 0; k < Nblocks; k++) {
		copy(inputBufferTimeDomain.begin() + k*(NFFT / 2), inputBufferTimeDomain.begin() + ((k + 1)*(NFFT / 2)), v_in_temp.begin());
		copy(v_in_temp.begin(), v_in_temp.end(), var_temp.begin() + (NFFT / 2));
		real_temp_copy = var_temp;
		rotate(var_temp.begin(), var_temp.begin() + NFFT / 2, var_temp.end());
		// Computation of FFT of each block
		fft.directTransform(real_temp_copy, imag_temp_copy);
		// Multiplication with the transfer std::function
		CMult.CMultVector_Loop(real_temp_copy, imag_temp_copy, H_real, H_imag);
		// Computation of IFFT of each block
		fft.inverseTransform(real_temp_copy, imag_temp_copy);
		// Removing the samples symetrically and assign to the output
		copy(real_temp_copy.begin() + (NFFT / 2), real_temp_copy.end(), v_out_temp.begin());
		copy(v_out_temp.begin(), v_out_temp.end(), outputBufferTimeDomain.begin() + k*(NFFT / 2));
	}
}*/

/*bool FD_Filter::runBlock(void) {
	
	Fft fft;
	ComplexMult CMult;
	bool alive{ false };
	int ready = inputSignals[0]->ready();
	//inputBufferTimeDomain.resize(ready);
	//outputBufferTimeDomain.resize(ready);
	int space = (int) inputBufferTimeDomain.size() - inputBufferPointer;
	int process1 = std::min(ready, space);
	//int process1 = ready;
	if (process1 > 0) alive = true;
	// read incoming samples to the input buffer
	for (int k = 0; k < process1; k++) {
		t_real val;
		inputSignals[0]->bufferGet(&val);
		inputBufferTimeDomain[inputBufferPointer] = val;
		inputBufferPointer++;
	}
	//overlapSaveZPRealIn();
	//OverlapSaveMethod();
	if ((inputBufferPointer == inputBufferTimeDomain.size()) && (outputBufferPointer == outputBufferTimeDomain.size()))
	{
		outputBufferTimeDomain = fft.inverseTransformInCP(CMult.CMultVectorInCP(fft.directTransformInReal(inputBufferTimeDomain), transferFunction));
		rotate(inputBufferTimeDomain.begin(), inputBufferTimeDomain.begin() + inputBufferTimeDomain.size()/2, inputBufferTimeDomain.end());
		inputBufferPointer = (int) inputBufferTimeDomain.size()/2;
		outputBufferPointer = (int) outputBufferTimeDomain.size()/2;
	};
	//space = outputSignals[0]->space();
	//ready = outputBufferTimeDomain.size() - outputBufferPointer;
	int process2 = std::min(ready, space);
	if (process2 > 0) alive = true;
	//int process2 = outputBufferTimeDomain.size();
	for (int k = 0; k < process2; k++) {
		t_real val = outputBufferTimeDomain[outputBufferPointer];
		(outputSignals[0])->bufferPut(&val);
		outputBufferPointer++;
	}
	//alive = false;
	return alive;
};
*/    

/*2016-08-03
DiscreteToContinuousTime::DiscreteToContinuousTime(std::vector<Signal *> &InputSig, std::vector<Signal *> &OutputSig) {
  numberOfInputSignals = InputSig.size();
  numberOfOutputSignals = OutputSig.size();
  inputSignals = InputSig;
  outputSignals = OutputSig;
  outputSignals[0]->symbolPeriod = (inputSignals[0]->symbolPeriod);
  outputSignals[0]->samplingPeriod = (inputSignals[0]->samplingPeriod) / numberOfSamplesPerSymbol;
  
}
*/



RealToComplex::RealToComplex(std::vector <Signal *> &InputSig, std::vector <Signal *> &OutputSig) {

	setNumberOfInputSignals((int) InputSig.size());
	setNumberOfOutputSignals((int) OutputSig.size());

	inputSignals = InputSig;
	outputSignals = OutputSig;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
}

bool RealToComplex::runBlock(void) {

	int ready0 = inputSignals[0]->ready();
	int ready1 = inputSignals[1]->ready();
	int ready = std::min(ready0, ready1);

	int space = outputSignals[0]->space();

	int process = std::min(ready, space);

	if (process == 0) return false;

	t_real re;
	t_real im;
	for (int i = 0; i < process; i++) {

		inputSignals[0]->bufferGet(&re);
		inputSignals[1]->bufferGet(&im);

		std::complex<t_real> myComplex( re, im);

		outputSignals[0]->bufferPut(myComplex);

	}

	return true;
}
//
//ComplexToReal::ComplexToReal(std::vector<Signal *> &InputSig, std::vector<Signal *> &OutputSig) {
//
//  numberOfInputSignals = 1;
//  numberOfOutputSignals = 2;
//  InputSignals = InputSig;
//  OutputSignals = OutputSig;
//
//  outputSignals[0]->period = 2*InputSignals[0]->period;
//  outputSignals[1]->period = 2*InputSignals[0]->period;
//  static_cast<TimeContinuousAmplitudeContinuousReal*>(outputSignals[0])->samplingTime =
//  2*static_cast<TimeContinuousAmplitudeContinuousComplex*>(InputSignals[0])->samplingTime;
//  static_cast<TimeContinuousAmplitudeContinuousReal*>(outputSignals[1])->samplingTime =
//  2 * static_cast<TimeContinuousAmplitudeContinuousComplex*>(InputSignals[0])->samplingTime;
//
//}
//
//bool ComplexToReal::runBlock(void){
//
//  FILE *file = fopen(outputSignals[0]->fileName, "a");
//  FILE *file2 = fopen(outputSignals[1]->fileName, "a");
//
//  int length_in = InputSignals[0]->bufferLength;
//  int length_out1 = outputSignals[0]->bufferLength;
//  int length_out2 = outputSignals[1]->bufferLength;
//
//  double *auxil1 = static_cast<double*>(outputSignals[0]->buffer);
//  double *auxil2 = static_cast<double*>(outputSignals[1]->buffer);
//
//  if (InputSignals[0]->outPosition >= length_in ||
//      outputSignals[0]->inPosition >= length_out1 ||
//      outputSignals[1]->inPosition >= length_out2) {
//
//    fclose(file);
//    fclose(file2);
//    return false;
//
//  } else {
//    for (int i = InputSignals[0]->outPosition; i < length_in; i ++) {
//      if (outputSignals[0]->inPosition < length_out1 &&
//          outputSignals[1]->inPosition < length_out2) {
//
//        auxil1[i] = static_cast<std::complex<double>*>(InputSignals[0]->buffer)[i].real();
//        auxil2[i] = static_cast<std::complex<double>*>(InputSignals[0]->buffer)[i].imag();
//
//        InputSignals[0]->outPosition++;
//        outputSignals[0]->inPosition++;
//        outputSignals[1]->inPosition++;
//        outputSignals[0]->count++;
//        outputSignals[1]->count++;
//
//        // File 1
//        if ((outputSignals[0]->count >= outputSignals[0]->firstValueToBeSaved) &&
//            (outputSignals[0]->numberOfSavedValues <
//             outputSignals[0]->numberOfValuesToBeSaved)) {
//
//          fprintf(file, "%7.4f,",
//                 (static_cast<double*>(outputSignals[0]->buffer))[outputSignals[0]->inPosition - 1]);
//
//          outputSignals[0]->numberOfSavedValues++;
//
//          if (outputSignals[0]->numberOfSavedValues ==
//              outputSignals[0]->numberOfValuesToBeSaved)
//            fprintf(file, "\0");
//        }
//
//        // File 2
//        if ((outputSignals[1]->count >= outputSignals[1]->firstValueToBeSaved) &&
//            (outputSignals[1]->numberOfSavedValues <
//             outputSignals[1]->numberOfValuesToBeSaved)) {
//
//          fprintf(file2, "%7.4f,",
//                 (static_cast<double*>(outputSignals[1]->buffer))[outputSignals[1]->inPosition - 1]);
//
//          outputSignals[1]->numberOfSavedValues++;
//
//          if (outputSignals[1]->numberOfSavedValues ==
//              outputSignals[1]->numberOfValuesToBeSaved)
//            fprintf(file2, "\0");
//        }
//
//      } else {
//        break;
//      }
//    }
//
//    outputSignals[0]->outPosition = 0;
//    outputSignals[1]->outPosition = 0;
//
//    if (InputSignals[0]->outPosition == length_in) {
//      InputSignals[0]->inPosition = 0;
//      fclose(file);
//      fclose(file2);
//      return true;
//    } else {
//      fclose(file);
//      fclose(file2);
//      return false;
//    }
//  }
//
//}
//
//ContinuousToDiscreteTime::ContinuousToDiscreteTime(std::vector<Signal *> &InputSig, std::vector<Signal *> &OutputSig, double time){
//
//  numberOfInputSignals = 1;
//  numberOfOutputSignals = 1;
//  InputSignals = InputSig;
//  OutputSignals = OutputSig;
//
//  timeSampling = time;
//  outputSignals[0]->period = timeSampling;
//
//  static_cast<TimeDiscreteAmplitudeContinuousReal*>(outputSignals[0])->samplingPeriod = timeSampling;
//
//}
//
//bool ContinuousToDiscreteTime::runBlock(void){
//
//  FILE *file = fopen(outputSignals[0]->fileName, "a");
//
//  int length_in = InputSignals[0]->bufferLength;
//  int length_out = outputSignals[0]->bufferLength;
//  double nS = timeSampling / 
//              static_cast<TimeContinuousAmplitudeContinuousReal*>(InputSignals[0])->samplingTime;
//
//  int nSamples = static_cast<int>(nS);
//
//  double *auxil = static_cast<double*>(outputSignals[0]->buffer);
//
//  if (InputSignals[0]->outPosition >= length_in || outputSignals[0]->inPosition >= length_out) {
//    fclose(file);
//    return false;
//  } else {
//
//    for (int i = InputSignals[0]->outPosition; i < length_in; i += nSamples) {
//
//      if (outputSignals[0]->inPosition < length_out) {
//
//        auxil[i/nSamples] = (static_cast<double*>(InputSignals[0]->buffer))[i];
//
//        outputSignals[0]->inPosition++;
//        outputSignals[0]->count++;
//
//        // File 
//        if ((outputSignals[0]->count >= outputSignals[0]->firstValueToBeSaved) &&
//            (outputSignals[0]->numberOfSavedValues <
//             outputSignals[0]->numberOfValuesToBeSaved)) {
//
//            fprintf(file, "%6.3f,",
//            (static_cast<double*>(outputSignals[0]->buffer))[outputSignals[0]->inPosition - 1]);
//
//            outputSignals[0]->numberOfSavedValues++;
//
//            if (outputSignals[0]->numberOfSavedValues ==
//                outputSignals[0]->numberOfValuesToBeSaved)
//              fprintf(file, "\0");
//         }
//
//         InputSignals[0]->outPosition += nSamples;
//
//       } else {
//        break;
//      }
//    }
//
//    outputSignals[0]->outPosition = 0;
//
//    if (InputSignals[0]->outPosition == length_in) {
//      InputSignals[0]->inPosition = 0;
//      fclose(file);
//      return true;
//    } else {
//      fclose(file);
//      return false;
//    }
//  }
//
//}
//


//########################################################################################################################################################
//########################################################## GENERIC SYSTEM DECLARATIONS AND DEFINITIONS #################################################
//########################################################################################################################################################



System::System(std::initializer_list<Block *> Blocks) 
{
	SystemBlocks = Blocks;

	for (int unsigned i = 0; i < SystemBlocks.size(); i++) 
	{
		SystemBlocks[i]->initializeBlock();
	}
}

System::System(std::initializer_list<Block *> Blocks, std::string signalsFolderName, std::vector<std::string> list)
{
	SystemBlocks = Blocks;
	for (int unsigned i = 0; i < SystemBlocks.size(); i++) {
		SystemBlocks[i]->initializeBlock();
	}
	setSignalsFolderName(signalsFolderName);
	setLoadedInputParameters(list);
}

void System::setSystem(std::initializer_list<Block *> Blocks)
{
	SystemBlocks = Blocks;

	for (int unsigned i = 0; i < SystemBlocks.size(); i++)
	{
		SystemBlocks[i]->initializeBlock();
	}
}

bool System::run() {
	return run(signalsFolder);
}

bool System::run(std::string signalPath) {

	bool systemAlive{ false };

	/*2018-04-18*/
	//Creates the signals folder if it doesn't exist
	if (!std::filesystem::is_directory(signalPath) || !std::filesystem::exists(signalPath)) {
		std::filesystem::create_directory(signalPath);
	}
	//Debug information
	clock_t start;
	std::string separator = "|";
	if (logValue && openFile)
		logFile.open("./" + signalPath + "/" + logFileName);
	//Writes which input parameters have been 
	logFile << "The following input parameters were loaded from the configuration file:" << std::endl;
	for (std::string p : loadedInputParameters) {
		logFile << p << std::endl;
	}
	//logFile << "-------------------------------------------------------" << std::endl;

	int l = 0;
	bool Alive;
	do {
		Alive = false;
		for (unsigned int i = 0; i < SystemBlocks.size(); i++) {
			// Writes debug information
			if (logValue) {
				time_t t_now = time(0);
				struct tm now;
				localtime_s(&now, &t_now);
				char buffer[20];
				snprintf(buffer, 20, "%04d-%02d-%02d %02d:%02d:%02d", 1900 + now.tm_year, now.tm_mon + 1, now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec);
				logFile << "-----------------------------------------------------------------\n";
				logFile << "########## " << std::string(typeid(*SystemBlocks[i]).name()).substr(6) << " ##########" << std::endl; // Prints block name e.g. "Add"
				logFile << "Block start time: " << buffer << std::endl; 
				// Prints line for each input signal in the current block being executed
				logFile << "Input Signals: " << std::endl;
				for (Signal *b : SystemBlocks[i]->inputSignals) {
					std::string filename = (*b).getFileName(); // Gets filename e.g: "S8.sgn"
					logFile << std::string(typeid(*SystemBlocks[i]).name()).substr(6) << separator // Prints block name e.g. "Add"
						<< filename.substr(0, filename.find(".")) << separator // Prints the formated filename e.g. "S8.sgn" becomes "S8"
						<< "ready=" << (*b).ready() << std::endl; // Prints the amount of bits ready to be processed 
				}
				// Prints line for each output signal in the current block being executed
				logFile << "Output Signals: " << std::endl;
				for (Signal *b : SystemBlocks[i]->outputSignals) {
					std::string filename = (*b).getFileName(); // Gets filename e.g: "S8.sgn"
					logFile << std::string(typeid(*SystemBlocks[i]).name()).substr(6) << separator // Prints block name e.g. "Add"
						<< filename.substr(0, filename.find(".")) << separator // Prints the formated filename e.g. "S8.sgn" becomes "S8"
						<< "space=" << (*b).space() << std::endl; // Prints the amount of bits ready to be processed 
				}
				start = clock(); //Counts the time taken to run the current block
			}

			bool aux = SystemBlocks[i]->runBlock();
			Alive = (Alive || aux);
			if (Alive) systemAlive = true;

			if (logValue)
			{
				logFile << "-----------------------------------------------------------------\n";
				logFile << "Elapsed time: " << (float)(clock() - start) << " milliseconds" << std::endl;
				logFile << "-----------------------------------------------------------------\n";
				// Prints line for each input signal in the current block being executed
				logFile << "Input Signals: " << std::endl;
				for (Signal *b : SystemBlocks[i]->inputSignals) {
					std::string filename = (*b).getFileName(); // Gets filename e.g: "S8.sgn"
					logFile << std::string(typeid(*SystemBlocks[i]).name()).substr(6) << separator // Prints block name e.g. "Add"
						<< filename.substr(0, filename.find(".")) << separator // Prints the formated filename e.g. "S8.sgn" becomes "S8"
						<< "ready=" << (*b).ready() << std::endl; // Prints the amount of bits ready to be processed 
				}
				// Prints line for each output signal in the current block being executed
				logFile << "Output Signals: " << std::endl;
				for (Signal *b : SystemBlocks[i]->outputSignals) {
					std::string filename = (*b).getFileName(); // Gets filename e.g: "S8.sgn"
					logFile << std::string(typeid(*SystemBlocks[i]).name()).substr(6) << separator // Prints block name e.g. "Add"
						<< filename.substr(0, filename.find(".")) << separator // Prints the formated filename e.g. "S8.sgn" becomes "S8"
						<< "space=" << (*b).space() << std::endl; // Prints the amount of bits ready to be processed 
				}
				logFile << std::endl << std::endl;
			}
		}
		l++;
	} while (Alive);

	return systemAlive;
}

void System::writeReport()
{
	//SystemBlocks[3]

	std::ofstream reportFile;
	reportFile.open("report.txt");

	reportFile << "RESULTS: Reference Network\n\n";
	reportFile << "Scenario: Opaque\n\n";
	reportFile << "--------------------------------------------------------\n";
	reportFile << "Information regarding LINKS:\n";
	reportFile << "--------------------------------------------------------\n";
	reportFile << "| Link between Node: | Optical channels: | Amplifiers: |\n";
	reportFile << "--------------------------------------------------------\n\n\n";
	reportFile << "--------------------------------------------------------\n";
	reportFile << "Information regarding NODES:\n\n";
	reportFile << "--------------------------------------------------------\n";
	reportFile << "Node:  | Connections: | Line Ports: | Tributary Ports: |\n";
	reportFile << "--------------------------------------------------------\n\n\n";
	reportFile << "Detailed description of each node:\n\n";
	reportFile << "-------------------------------------------------------------\n";
	reportFile << "Information regarding PATHS:\n";
	reportFile << "-------------------------------------------------------------\n\n\n";
	reportFile << "-------------------------------------------------------------\n";
	reportFile << "Information regarding CAPEX:\n";
	reportFile << "-------------------------------------------------------------\n\n\n";
	reportFile << "-------------------------------------------------------------\n";
	reportFile << "---------------------- Link Cost: ---------------------------\n";
	reportFile << "-------------------------------------------------------------\n";
	
	reportFile << "-------------------------------------------------------------\n";
	
	reportFile << "-------------------------------------------------------------\n";
	reportFile << "-------------------------------------------------------------\n";
	reportFile << "---------------------- Node Cost: ---------------------------\n";
	reportFile << "-------------------------------------------------------------\n";
	
	reportFile << "-------------------------------------------------------------\n";
	
	reportFile << "-------------------------------------------------------------\n";

	reportFile << "-------------------------------------------------------------\n";
	
	reportFile << "-------------------------------------------------------------\n";
	
	reportFile << "-------------------------------------------------------------\n";

	reportFile << "-------------------------------------------------------------\n";

	reportFile.close();
}

void System::terminate()
{

	for (int unsigned i = 0; i < SystemBlocks.size(); i++) {
		SystemBlocks[i]->terminateBlock();
	}
	//Closes debug file
	if (logValue)
		logFile.close();

}

void System::terminateSuperBlock()
{

	for (int unsigned i = 0; i < SystemBlocks.size(); i++) {
		SystemBlocks[i]->terminateBlock();
	}

	SystemBlocks[SystemBlocks.size() - 1]->closeOutputSignals();

	//Closes debug file
	if (logValue)
		logFile.close();

}

void System::setOpenFile(bool value) {
	openFile = value;
}

void System::setLogValue(bool value){
	logValue = value;
}

void System::setLogFileName(std::string newName){
	logFileName = newName;
}

void System::setSignalsFolderName(std::string newName)
{
	signalsFolder = newName;

	for (auto b : SystemBlocks) {
		for (auto s : b->inputSignals)
		{
			s->setFolderName(signalsFolder);
		}
		for (auto s : b->outputSignals)
		{
			s->setFolderName(signalsFolder);
		}
	}
}

void System::setLoadedInputParameters(std::vector<std::string> loadedInputParams)
{
	loadedInputParameters = loadedInputParams;
}


//########################################################################################################################################################
//############################################################### GENERIC DSP FUNCTIONS ##################################################################
//########################################################################################################################################################

/*
void OverlapMethod::overlapSaveSyRealIn(std::vector<double> &v_in, std::vector<double> &v_out, std::vector<double> Hf, int NFFT) {
	int Nblocks = 2 * ((int) v_in.size() / NFFT);
	std::vector<double> var_temp(NFFT, 0);
	std::vector<double> real_temp_copy(NFFT, 0);
	std::vector<double> imag_temp_copy(NFFT, 0);
	std::vector<double> H_real(NFFT, 0);
	std::vector<double> H_imag(NFFT, 0);
	Fft fft;
	ComplexMult CMult;
	H_real = Hf;
	fft.directTransform(H_real, H_imag);
	for (int k = 0; k < Nblocks; k++) {
		if (k == Nblocks - 1) 
		{
			copy(v_in.begin(), v_in.begin() + (NFFT / 2), var_temp.begin() + (NFFT / 2));
			copy(v_in.end() - (NFFT / 2), v_in.end(), var_temp.begin());
		}
		else
		{
			copy(v_in.begin() + k*(NFFT / 2), v_in.begin() + ((k + 1)*NFFT - k*(NFFT / 2)), var_temp.begin());
		}
		real_temp_copy = var_temp;
		// Computation of FFT of each block
		fft.directTransform(real_temp_copy, imag_temp_copy);
		// Multiplication with the transfer std::function
		CMult.CMultVector_Loop(real_temp_copy, imag_temp_copy, H_real, H_imag);
		// Computation of IFFT of each block
		fft.inverseTransform(real_temp_copy, imag_temp_copy);
		// Removing the samples symetrically and assign to the output
		if (k == Nblocks - 1) {
			copy(real_temp_copy.begin() + (NFFT / 4), real_temp_copy.begin() + (NFFT / 2), v_out.end() - (NFFT / 4));
			copy(real_temp_copy.begin() + (NFFT / 2), real_temp_copy.begin() + 3 * NFFT / 4, v_out.begin());
		}
		else {
			copy(real_temp_copy.begin() + (NFFT / 4), real_temp_copy.end() - (NFFT / 4), v_out.begin() + ((NFFT / 4) + k*(NFFT / 2)));
		}
	}
}
*/

/*
// Private std::function prototypes
static size_t reverseBits(size_t x, unsigned int n);
std::vector<std::complex <double>> Fft::directTransformInReal(std::vector<double> real)
{
	//if (In.real.size() != imag.size())
	//throw "Mismatched lengths";
	ComplexMult CMult;
	std::vector<double> im(real.size(), 0);
	std::vector<std::complex <double>> v_out(real.size(), 0);
	size_t n = real.size();
	if (n == 0)
		return v_out;
	else if ((n & (n - 1)) == 0)  // Is power of 2
		transformRadix2(real, im);
	else  // More complicated algorithm for arbitrary sizes
		transformBluestein(real, im);
	CMult.ReImVect2ComplexVect(real, im, v_out);
	return v_out;
}
std::vector<double> Fft::inverseTransformInCP(std::vector<std::complex <double>> &In)
{
	ComplexMult CMult;
	std::vector<double> real(In.size(), 0);
	std::vector<double> im(In.size(), 0);
	CMult.ComplexVect2ReImVect(In, real, im);
	directTransform(im, real);
	for (int x = 0; x != real.size(); ++x)
	{
		real[x] = real[x] / real.size();
		  im[x] =   im[x] / real.size();
	}
	std::vector<double> v_out(real.size(), 0);
	v_out = real;
	//CMult.ReImVect2ComplexVect(real, imag, v_out);
	return v_out;
}
void Fft::directTransform(std::vector<double> &real, std::vector<double> &imag)
{
	if (real.size() != imag.size())
		throw "Mismatched lengths";
	size_t n = real.size();
	if (n == 0)
		return;
	else if ((n & (n - 1)) == 0)			// Is power of 2
		transformRadix2(real, imag);
	else									// More complicated algorithm for arbitrary sizes
		transformBluestein(real, imag);
}
void Fft::inverseTransform(std::vector<double> &real, std::vector<double> &imag)
{
	directTransform(imag, real);					// Inverse std::function
	for (int x = 0; x != real.size(); ++x)
	{
		real[x] = real[x] / real.size();			// Normalize
		imag[x] = imag[x] / real.size();
	}
}
void Fft::transformRadix2(std::vector<double> &real, std::vector<double> &imag) 
{
	// Compute levels = floor(log2(n))
	if (real.size() != imag.size())
		throw "Mismatched lengths";
	size_t n = real.size();
	unsigned int levels;
	{
		size_t temp = n;
		levels = 0;
		while (temp > 1)
		{
			levels++;
			temp >>= 1;			// temp = temp >> 1 (bit wise right shift)
		}
		if (1u << levels != n)
			throw "Length is not a power of 2";
	}
	// Trignometric tables
	std::vector<double> cosTable(n / 2);
	std::vector<double> sinTable(n / 2);
	for (size_t i = 0; i < n / 2; i++)
	{
		cosTable[i] = cos(2 * M_PI * i / n);
		sinTable[i] = sin(2 * M_PI * i / n);
	}
	// Bit-reversed addressing permutation
	for (size_t i = 0; i < n; i++) 
	{
		size_t j = reverseBits(i, levels);
		if (j > i) 
		{
			double temp = real[i];
			real[i] = real[j];
			real[j] = temp;
			temp = imag[i];
			imag[i] = imag[j];
			imag[j] = temp;
		}
	}
	// Cooley-Tukey decimation-in-time radix-2 FFT
	for (size_t size = 2; size <= n; size *= 2) 
	{
		size_t halfsize = size / 2;
		size_t tablestep = n / size;
		for (size_t i = 0; i < n; i += size) 
		{
			for (size_t j = i, k = 0; j < i + halfsize; j++, k += tablestep)
			{
				double tpre = real[j + halfsize] * cosTable[k] + imag[j + halfsize] * sinTable[k];
				double tpim = -real[j + halfsize] * sinTable[k] + imag[j + halfsize] * cosTable[k];
				real[j + halfsize] = real[j] - tpre;
				imag[j + halfsize] = imag[j] - tpim;
				real[j] += tpre;
				imag[j] += tpim;
			}
		}
		if (size == n)  // Prevent overflow in 'size *= 2'
			break;
	}
}
void Fft::Radix2(std::vector<double> &real, std::vector<double> &imag, int s)
{
	// Compute levels = floor(log2(n))
	if (real.size() != imag.size())
		throw "Mismatched lengths";
	size_t n = real.size();
	unsigned int levels;
	{
		size_t temp = n;
		levels = 0;
		while (temp > 1)
		{
			levels++;
			temp >>= 1;			// temp = temp >> 1 (bit wise right shift)
		}
		if (1u << levels != n)
			throw "Length is not a power of 2";
	}
	// Trignometric tables
	std::vector<double> cosTable(n / 2);
	std::vector<double> sinTable(n / 2);
	for (size_t i = 0; i < n / 2; i++)
	{
		cosTable[i] = cos(-s*2 * M_PI * i / n);
		sinTable[i] = sin(-s*2 * M_PI * i / n);
	}
	// Bit-reversed addressing permutation
	for (size_t i = 0; i < n; i++)
	{
		size_t j = reverseBits(i, levels);
		if (j > i)
		{
			double temp = real[i];
			real[i] = real[j];
			real[j] = temp;
			temp = imag[i];
			imag[i] = imag[j];
			imag[j] = temp;
		}
	}
	// Cooley-Tukey decimation-in-time radix-2 FFT
	for (size_t size = 2; size <= n; size *= 2)
	{
		size_t halfsize = size / 2;
		size_t tablestep = n / size;
		for (size_t i = 0; i < n; i += size)
		{
			for (size_t j = i, k = 0; j < i + halfsize; j++, k += tablestep)
			{
				double tpre = real[j + halfsize] * cosTable[k] + imag[j + halfsize] * sinTable[k];
				double tpim = -real[j + halfsize] * sinTable[k] + imag[j + halfsize] * cosTable[k];
				real[j + halfsize] = real[j] - tpre;
				imag[j + halfsize] = imag[j] - tpim;
				real[j] += tpre;
				imag[j] += tpim;
			}
		}
		if (size == n)  // Prevent overflow in 'size *= 2'
			break;
	}
}
void Fft::transformBluestein(std::vector<double> &real, std::vector<double> &imag) 
{
	// Find a power-of-2 convolution length m such that m >= n * 2 + 1
	if (real.size() != imag.size())
		throw "Mismatched lengths";
	size_t n = real.size();
	size_t m;
	{
		size_t target;
		if (n > (SIZE_MAX - 1) / 2)
			throw "std::vector too large";
		target = n * 2 + 1;
		for (m = 1; m < target; m *= 2) 
		{
			if (SIZE_MAX / 2 < m)
				throw "std::vector too large";
		}
	}
	// Trignometric tables
	std::vector<double> cosTable(n), sinTable(n);
	for (size_t i = 0; i < n; i++) {
		double temp = M_PI * (size_t)((unsigned long long)i * i % ((unsigned long long)n * 2)) / n;
		// Less accurate version if long long is unavailable: double temp = M_PI * i * i / n;
		cosTable[i] = cos(temp);
		sinTable[i] = sin(temp);
	}
	// Temporary vectors and preprocessing
	std::vector<double> areal(m), aimag(m);
	for (size_t i = 0; i < n; i++) {
		areal[i] = real[i] * cosTable[i] + imag[i] * sinTable[i];
		aimag[i] = -real[i] * sinTable[i] + imag[i] * cosTable[i];
	}
	std::vector<double> breal(m), bimag(m);
	breal[0] = cosTable[0];
	bimag[0] = sinTable[0];
	for (size_t i = 1; i < n; i++) {
		breal[i] = breal[m - i] = cosTable[i];
		bimag[i] = bimag[m - i] = sinTable[i];
	}
	// Convolution
	std::vector<double> creal(m), cimag(m);
	convolve(areal, aimag, breal, bimag, creal, cimag);
	// Postprocessing
	for (size_t i = 0; i < n; i++) {
		real[i] = creal[i] * cosTable[i] + cimag[i] * sinTable[i];
		imag[i] = -creal[i] * sinTable[i] + cimag[i] * cosTable[i];
	}
}
void Fft::Bluestein(std::vector<double> &real, std::vector<double> &imag, int s)
{
	// Find a power-of-2 convolution length m such that m >= n * 2 + 1
	if (real.size() != imag.size())
		throw "Mismatched lengths";
	size_t n = real.size();
	size_t m;
	{
		size_t target;
		if (n > (SIZE_MAX - 1) / 2)
			throw "std::vector too large";
		target = n * 2 + 1;
		for (m = 1; m < target; m *= 2)
		{
			if (SIZE_MAX / 2 < m)
				throw "std::vector too large";
		}
	}
	// Trignometric tables
	std::vector<double> cosTable(n), sinTable(n);
	for (size_t i = 0; i < n; i++) {
		double temp = M_PI * (size_t)((unsigned long long)i * i % ((unsigned long long)n * 2)) / n;
		// Less accurate version if long long is unavailable: double temp = M_PI * i * i / n;
		cosTable[i] =    cos(-s*temp);
		sinTable[i] =    sin(-s*temp);
	}
	// Temporary vectors and preprocessing
	std::vector<double> areal(m), aimag(m);
	for (size_t i = 0; i < n; i++) {
		areal[i] = real[i] * cosTable[i] + imag[i] * sinTable[i];
		aimag[i] = -real[i] * sinTable[i] + imag[i] * cosTable[i];
	}
	std::vector<double> breal(m), bimag(m);
	breal[0] = cosTable[0];
	bimag[0] = sinTable[0];
	for (size_t i = 1; i < n; i++) {
		breal[i] = breal[m - i] = cosTable[i];
		bimag[i] = bimag[m - i] = sinTable[i];
	}
	// Convolution
	std::vector<double> creal(m), cimag(m);
	convolve(areal, aimag, breal, bimag, creal, cimag);
	// Postprocessing
	for (size_t i = 0; i < n; i++) {
		real[i] = creal[i] * cosTable[i] + cimag[i] * sinTable[i];
		imag[i] = -creal[i] * sinTable[i] + cimag[i] * cosTable[i];
	}
}
*/

/*
void Fft::convolve(const std::vector<double> &x, const std::vector<double> &y, std::vector<double> &out) {
	if (x.size() != y.size() || x.size() != out.size())
		throw "Mismatched lengths";
	size_t n = x.size();
	std::vector<double> ximag(n), yimag(n), zimag(n);
	convolve(x, ximag, y, yimag, out, zimag);
}
void Fft::convolve(const std::vector<double> &xreal, const std::vector<double> &ximag, const std::vector<double> &yreal, const std::vector<double> &yimag, std::vector<double> &outreal, std::vector<double> &outimag) {
	if (xreal.size() != ximag.size() || xreal.size() != yreal.size() || yreal.size() != yimag.size() || xreal.size() != outreal.size() || outreal.size() != outimag.size())
		throw "Mismatched lengths";
	size_t n = xreal.size();
	std::vector<double> xr(xreal);
	std::vector<double> xi(ximag);
	std::vector<double> yr(yreal);
	std::vector<double> yi(yimag);
	directTransform(xr, xi);
	directTransform(yr, yi);
	for (size_t i = 0; i < n; i++) {
		double temp = xr[i] * yr[i] - xi[i] * yi[i];
		xi[i] = xi[i] * yr[i] + xr[i] * yi[i];
		xr[i] = temp;
	}
	inverseTransform(xr, xi);
	for (size_t i = 0; i < n; i++) {  // Scaling (because this FFT implementation omits it)
		outreal[i] = xr[i] / n;
		outimag[i] = xi[i] / n;
	}
}*/

void ComplexMult::CMultVector(std::vector<double> &v1_real, std::vector<double> &v1_imag, std::vector<double> v2_real, std::vector<double> v2_imag) {


	std::vector<double> ac_v(v1_real.size(), 0);
	std::vector<double> bd_v(v1_real.size(), 0);
	std::vector<double> bc_v(v1_real.size(), 0);
	std::vector<double> ad_v(v1_real.size(), 0);

	transform(v1_real.begin(), v1_real.end(), v2_real.begin(), ac_v.begin(), std::multiplies<double>()); // ac

	transform(v1_imag.begin(), v1_imag.end(), v2_imag.begin(), bd_v.begin(), std::multiplies<double>()); // bd

	transform(v1_imag.begin(), v1_imag.end(), v2_real.begin(), bc_v.begin(), std::multiplies<double>()); // bc

	transform(v1_real.begin(), v1_real.end(), v2_imag.begin(), ad_v.begin(), std::multiplies<double>()); // ad

	transform(ac_v.begin(), ac_v.end(), bd_v.begin(), v1_real.begin(), std::minus<double>()); // ac - bd

	transform(bc_v.begin(), bc_v.end(), ad_v.begin(), v1_imag.begin(), std::plus<double>()); // bc + ad

}

void ComplexMult::CMultVector_Loop(std::vector<double> &v1_real, std::vector<double> &v1_imag, std::vector<double> v2_real, std::vector<double> v2_imag) {


	double Real_temp = 0.0;
	double Imag_temp = 0.0;

	for (size_t k = 0; k < v1_real.size(); ++k)
	{

		Real_temp = v1_real.at(k)*v2_real.at(k) - v1_imag.at(k)*v2_imag.at(k);
		Imag_temp = v1_imag.at(k)*v2_real.at(k) + v1_real.at(k)*v2_imag.at(k);

		v1_real.at(k) = Real_temp;
		v1_imag.at(k) = Imag_temp;
	}

}

void ComplexMult::CMultVector_InComplex(std::vector<std::complex <double>> &v1_in, std::vector<std::complex <double>> &v2_in) {

	for (size_t k = 0; k < v1_in.size(); ++k)
	{
		v1_in.at(k) = v1_in.at(k)*v2_in.at(k);

	}

}

std::vector<std::complex <double>> ComplexMult::CMultVectorInCP(std::vector<std::complex <double>> &v1_in, std::vector<std::complex <double>> &v2_in) {

	std::vector<std::complex <double>> v_out(v1_in.size(), 0);
	for (unsigned int k = 0; k < v1_in.size(); ++k)
	{
		v_out.at(k) = v1_in.at(k)*v2_in.at(k);
	}
	return v_out;
}

void ComplexMult::ComplexVect2ReImVect(std::vector<std::complex <double>> &v_in, std::vector<double> &v1_real, std::vector<double> &v1_imag) {

	for (size_t k = 0; k < v_in.size(); k++) {
		v1_real.at(k) = real(v_in.at(k));
		v1_imag.at(k) = imag(v_in.at(k));
	}

}

void ComplexMult::ReImVect2ComplexVect(std::vector<double> &v1_real, std::vector<double> &v1_imag, std::vector<std::complex <double>> &v_out) {

	for (size_t i = 0; i < v1_real.size(); ++i) {
		std::complex<double> iNum(v1_real[i], v1_imag[i]);
		v_out.at(i) = iNum;
	}

}


std::vector<std::complex<double>> ComplexMult::ReImVect2ComplexVector(std::vector<double> &v1_real, std::vector<double> &v1_imag)
{
	std::vector<std::complex<double>> v_out(v1_real.size());

	for (size_t i = 0; i < v1_real.size(); ++i)
	{
		std::complex<double> iNum(v1_real[i], v1_imag[i]);
		v_out.at(i) = iNum;
	}

	return v_out;
}

std::vector<std::complex <double>> ComplexMult::complexVectorMultiplication(std::vector<std::complex <double>> &v1_in, std::vector<std::complex <double>> &v2_in)
{

	std::vector<std::complex <double>> v_out(v1_in.size(), 0);
	for (unsigned int k = 0; k < v1_in.size(); ++k)
	{
		v_out.at(k) = v1_in.at(k)*v2_in.at(k);
	}
	return v_out;
}



////////////  Fast FourierTransform  /////////////// 

// Private std::function prototypes

ComplexMult C;

static size_t reverseBits(size_t x, unsigned int n)
{
	size_t result = 0;
	unsigned int i;
	for (i = 0; i < n; i++, x >>= 1)
		result = (result << 1) | (x & 1);
	return result;
}

std::vector<std::complex<double>> FourierTransform::fft(std::vector<std::complex<double> > &vec)
{
	size_t n = vec.size();

	if (n == 0)
		return vec;
	else if ((n & (n - 1)) == 0)  // Is power of 2
		transformRadix2(vec);
	else  // More complicated algorithm for arbitrary sizes
		transformBluestein(vec);

	/*t_complex sqrtN = (sqrt(n),0);
	for (unsigned int i = 0; i < n; i++)
	{
	vec[i] = vec[i] / sqrtN;
	}*/

	return vec;
}


std::vector<std::complex<double>> FourierTransform::ifft(std::vector<std::complex<double> > &vec)
{
	std::vector<std::complex<double>> OUT(vec.size());
	std::vector<std::complex<double>> OUT_intermediate(vec.size());
	std::vector<double> temp_real(vec.size());
	std::vector<double> temp_imag(vec.size());


	std::transform(vec.cbegin(), vec.cend(), vec.begin(), static_cast<std::complex<double>(*)(const std::complex<double> &)>(std::conj));
	fft(vec);
	std::transform(vec.cbegin(), vec.cend(), vec.begin(), static_cast<std::complex<double>(*)(const std::complex<double> &)>(std::conj));

	for (int i = 0; i != vec.size(); ++i)
	{
		temp_real[i] = vec[i].real() / vec.size();
		temp_imag[i] = vec[i].imag() / vec.size();
	}

	OUT_intermediate = C.ReImVect2ComplexVector(temp_real, temp_imag);

	/*t_complex sqrtN = (sqrt(n),0);
	for (unsigned int i = 0; i < n; i++)
	{
	OUT_intermediate = vec[i] / sqrtN;
	}*/

	OUT = OUT_intermediate;

	return OUT;
}


void FourierTransform::transformRadix2(std::vector<std::complex<double> > &vec)
{
	// Length variables
	size_t n = vec.size();
	int levels = 0;  // Compute levels = floor(log2(n))
	for (size_t temp = n; temp > 1U; temp >>= 1)
		levels++;
	if (static_cast<size_t>(1U) << levels != n)
		throw "Length is not a power of 2";

	// Trignometric table
	std::vector<std::complex<double> > expTable(n / 2);
	for (size_t i = 0; i < n / 2; i++)
		expTable[i] = std::exp(std::complex<double>(0, -2 * M_PI * i / n));

	// Bit-reversed addressing permutation
	for (size_t i = 0; i < n; i++)
	{
		size_t j = reverseBits(i, levels);
		if (j > i)
			std::swap(vec[i], vec[j]);
	}

	// Cooley-Tukey decimation-in-time radix-2 FFT
	for (size_t size = 2; size <= n; size *= 2)
	{
		size_t halfsize = size / 2;
		size_t tablestep = n / size;

		for (size_t i = 0; i < n; i += size)	// Loop for intermediate stage
		{
			for (size_t j = i, k = 0; j < i + halfsize; j++, k += tablestep)
			{

				std::complex<double> temp = vec[j + halfsize] * expTable[k];
				vec[j + halfsize] = vec[j] - temp;
				vec[j] += temp;
			}
		}
		if (size == n)  // Prevent overflow in 'size *= 2'
			break;
	}
}


void FourierTransform::transformBluestein(std::vector<std::complex<double> > &vec) {
	// Find a power-of-2 convolution length m such that m >= n * 2 + 1
	size_t n = vec.size();
	size_t m = 1;
	while (m / 2 <= n) {
		if (m > SIZE_MAX / 2)
			throw "std::vector too large";
		m *= 2;
	}

	// Trignometric table
	std::vector<std::complex<double> > expTable(n);
	for (size_t i = 0; i < n; i++)
	{
		unsigned long long temp = static_cast<unsigned long long>(i) * i;
		temp %= static_cast<unsigned long long>(n) * 2;				// Modulo operation ==> temp = temp % static_cast<unsigned long long>(n) * 2
		double angle = M_PI * temp / n;
		// Less accurate alternative if long long is unavailable: double angle = M_PI * i * i / n;
		expTable[i] = std::exp(std::complex<double>(0, angle));
	}

	// Temporary vectors and preprocessing
	std::vector<std::complex<double> > av(m);
	for (size_t i = 0; i < n; i++)
		av[i] = vec[i] * expTable[i];
	std::vector<std::complex<double> > bv(m);
	bv[0] = expTable[0];
	for (size_t i = 1; i < n; i++)
		bv[i] = bv[m - i] = std::conj(expTable[i]);

	// Convolution
	std::vector<std::complex<double> > cv(m);
	convolve(av, bv, cv);					// Here size of cv equal to m

	// Postprocessing
	for (size_t i = 0; i < n; i++)
		vec[i] = cv[i] * expTable[i];
}


void FourierTransform::convolve(const std::vector<std::complex<double> > &xvec, const std::vector<std::complex<double> > &yvec, std::vector<std::complex<double> > &outvec)
{
	size_t n = xvec.size();
	if (n != yvec.size() || n != outvec.size())
		throw "Mismatched lengths";
	std::vector<std::complex<double> > xv = xvec;
	std::vector<std::complex<double> > yv = yvec;
	fft(xv);
	fft(yv);
	for (size_t i = 0; i < n; i++)
		xv[i] *= yv[i];
	ifft(xv);
	for (size_t i = 0; i < n; i++)  // Scaling (because this FFT implementation omits it)
		outvec[i] = xv[i] / static_cast<double>(n);
}


static size_t reverseBits(size_t x, int n)
{
	size_t result = 0;
	for (int i = 0; i < n; i++, x >>= 1)
		result = (result << 1) | (x & 1U);
	return result;
}



std::vector<std::complex<double>> FourierTransform::fft(std::vector<std::complex<double> > &vec, int sign)
{
	std::vector<std::complex<double>> OUT(vec.size());
	std::vector<std::complex<double>> fftData(vec.size());
	std::vector<std::complex<double>> ifftData(vec.size());

	
	if (sign == -1)
	{
		fftData = fft(vec);
		OUT = fftData;
	}

	else

	{
		ifftData = ifft(vec);
		OUT = ifftData;
	}

	return OUT;
}

// #####################################################################################################
// ###################################        Parameters       #########################################
// #####################################################################################################

/* Auxiliary method to split std::string by a delimiter. Returns a std::vector of std::string */
/*static inline std::string &ltrim(std::string &s) { 
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), 
    std::not1(std::ptr_fun<int, int>(std::isspace)))); 
  return s; 
} 
 
static inline std::string &rtrim(std::string &s) { 
  s.erase(std::find_if(s.rbegin(), s.rend(), 
    std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end()); 
  return s; 
}*/
 
static inline std::string& ltrim(std::string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
		[](int c) {return !std::isspace(c); }));
	return s;
}

static inline std::string& rtrim(std::string& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(),
		[](int c) {return !std::isspace(c); }).base(), s.end());
	return s;
}

static inline std::string &trim(std::string &s) { 
  return ltrim(rtrim(s)); 
} 

std::vector<std::string> SystemInputParameters::split(const std::string &text, char sep) {
	std::vector<std::string> tokens;
	size_t start = 0, end = 0;
	while ((end = text.find(sep, start)) != std::string::npos) {
		tokens.push_back(text.substr(start, end - start));
		start = end + 1;
	}
	tokens.push_back(text.substr(start));
	return tokens;
}

void SystemInputParameters::readSystemInputParameters()
{
	if (inputParametersFileName == "") return;
	std::ifstream inputFile("./" + inputParametersFileName);
	if (!inputFile) {
		std::cerr << "ERROR: Could not open " << inputParametersFileName;
		exit(1);
	}
	int errorLine = 1;
	//Reads each line
	std::string line;
	while (getline(inputFile, line)) {
		line = trim(line); 
		if (line != "")
		{
			try {
				//If the line is a comment, it just skips to the next one
				if (std::string(line).substr(0, 2) != "//") { //Lines that start by // are comments
					std::vector<std::string> splitline = split(line, '=');
					splitline[0] = trim(splitline[0]);
					splitline[1] = trim(splitline[1]);
					if (parameters.find(splitline[0]) != parameters.end()) { //if parameter exists
						if (parameters[splitline[0]]->getType() == INT) //If parameter is an int
							parameters[splitline[0]]->setValue(parseInt(splitline[1]));
						else if (parameters[splitline[0]]->getType() == DOUBLE)
							parameters[splitline[0]]->setValue(parseDouble(splitline[1]));
						else if (parameters[splitline[0]]->getType() == BOOL)
							parameters[splitline[0]]->setValue(parseBool(splitline[1]));
					//	else if (parameters[splitline[0]]->getType() == MATRIX)
						//	parameters[splitline[0]]->setValue(parseMatrix(splitline[1]));
						else if (parameters[splitline[0]]->getType() == MATRIX)
							parameters[splitline[0]]->setValue(parseMatrix(inputFile));
						else if (parameters[splitline[0]]->getType() == ORDERING)
							parameters[splitline[0]]->setValue(parseOrderingRule(splitline[1]));
						else if (parameters[splitline[0]]->getType() == TRANSPORT)
							parameters[splitline[0]]->setValue(parseTransportMode(splitline[1]));
						else if (parameters[splitline[0]]->getType() == ROUTING_LOGICAL)
							parameters[splitline[0]]->setValue(parseRoutingCriterionLogicalTopology(splitline[1]));
						else if (parameters[splitline[0]]->getType() == ROUTING_PHYSICAL)
							parameters[splitline[0]]->setValue(parseRoutingCriterionPhysicalTopology(splitline[1]));
						//Logs that a given parameter has been loaded from a file
						loadedInputParameters.push_back(splitline[0] + " = " + splitline[1]);
					}
				}
				errorLine++;
			}
			catch (const std::exception& e) {
				(void)e;
				std::cerr << "ERROR: Invalid input in line " << errorLine << " of " << inputParametersFileName;
				exit(1);
			}
		}
	}
	inputFile.close();
}

void SystemInputParameters::addInputParameter(std::string name, int * variable)
{
	parameters[name] = new Parameter(variable);
}

void SystemInputParameters::addInputParameter(std::string name, double * variable)
{
	parameters[name] = new Parameter(variable);
}

void SystemInputParameters::addInputParameter(std::string name, bool * variable)
{
	parameters[name] = new Parameter(variable);
}

void SystemInputParameters::addInputParameter(std::string name, t_matrix * variable)
{
	parameters[name] = new Parameter(variable);
}

void SystemInputParameters::addInputParameter(std::string name, t_ordering_rule * variable)
{
	parameters[name] = new Parameter(variable);
}

void SystemInputParameters::addInputParameter(std::string name, t_transport_mode * variable)
{
	parameters[name] = new Parameter(variable);
}

void SystemInputParameters::addInputParameter(std::string name, t_routing_criterion_logical_topology * variable)
{
	parameters[name] = new Parameter(variable);
}

void SystemInputParameters::addInputParameter(std::string name, t_routing_criterion_physical_topology * variable)
{
	parameters[name] = new Parameter(variable);
}

SystemInputParameters::SystemInputParameters(int argc, char * argv[])
{
	if (argc == 1) {
		SystemInputParameters();
	}
	else if (argc == 2) { //Name of the file from where the input parameters will be read as been passed
		inputParametersFileName = argv[1];
	}
	else if (argc == 3) {//Name of input parameters' file and output directory passed
		inputParametersFileName = argv[1];
		outputFolderName = argv[2];
	}
}

SystemInputParameters::SystemInputParameters(std::string fName)
{
	inputParametersFileName = fName;
}

SystemInputParameters::~SystemInputParameters()
{	
	for (std::map<std::string, Parameter*>::iterator itr = parameters.begin(); itr != parameters.end();itr++) {
		delete (itr->second);
	}
}

/* Allows for the recognition of scientific notation. Ex: parseDouble("6.23e+1") will return 62*/
int SystemInputParameters::parseInt(std::string str)
{
	return (int) parseDouble(str);
}
/* Allows for the recognition of scientific notation. Ex: parseDouble("1.83e-2") will return 0.0183*/
double SystemInputParameters::parseDouble(std::string str) {
	std::istringstream os(str);
	double d;
	os >> d;
	return d;
}

bool SystemInputParameters::parseBool(std::string str)
{
	if (str == "true")
		return true;
	else if (str == "false")
		return false;
	else //Incorrect input
		throw std::exception();
}
/*
t_matrix SystemInputParameters::parseMatrix(std::string str)
{
	stringstream ss(str);
	std::string indivLine;
	char space = ',';
	int n;
	std::vector<int> line;
	t_matrix m;
	while (getline(ss, indivLine, space)) {
		stringstream iss(indivLine);
		while (iss >> n) {
			line.push_back(n);
		}
		m.push_back(line);
		line.clear();
	}
	return m;
}
*/
t_matrix SystemInputParameters::parseMatrix(std::ifstream &inputFile)
{
	std::string line;
	std::string number;
	std::vector<t_integer> lines;
	t_matrix m;
	
	t_integer n = 0;
	t_integer k = 0;
	
	do
	{
		getline(inputFile, line);
		std::stringstream s(line);

		k = 0;
		while (getline(s, number, ' '))
		{
			lines.push_back(stoi(number));
			k++;
		}
		
		m.push_back(lines);
		lines.clear();
		n++;

	} while (n < k);

	return m;
}

t_ordering_rule SystemInputParameters::parseOrderingRule(std::string str)
{
	t_ordering_rule o;

	if (str == "descendingOrder")
		return o = t_ordering_rule::descendingOrder;
	else if (str == "ascendingOrder")
		return o = t_ordering_rule::ascendingOrder;
	else //Incorrect input
		throw std::exception();
}

t_transport_mode SystemInputParameters::parseTransportMode(std::string str)
{
	t_transport_mode t;

	if (str == "opaque")
		return t = t_transport_mode::opaque;
	else if (str == "transparent")
		return t = t_transport_mode::transparent;
	else if (str == "translucent")
		return t = t_transport_mode::translucent;
	else //Incorrect input
		throw std::exception();
}

t_routing_criterion_logical_topology SystemInputParameters::parseRoutingCriterionLogicalTopology(std::string str)
{
	t_routing_criterion_logical_topology rl;

	if (str == "hops")
		return rl = t_routing_criterion_logical_topology::hops;
	else if (str == "km")
		return rl = t_routing_criterion_logical_topology::km;
	else //Incorrect input
		throw std::exception();
}

t_routing_criterion_physical_topology SystemInputParameters::parseRoutingCriterionPhysicalTopology(std::string str)
{
	t_routing_criterion_physical_topology rp;

	if (str == "hops")
		return rp = t_routing_criterion_physical_topology::hops;
	else if (str == "km")
		return rp = t_routing_criterion_physical_topology::km;
	else //Incorrect input
		throw std::exception();
}

void SystemInputParameters::Parameter::setValue(int value)
{
	if (type != INT) throw std::invalid_argument("Parameter is not of type INT");
	*i = value;
}

void SystemInputParameters::Parameter::setValue(double value)
{
	if (type != DOUBLE) throw std::invalid_argument("Parameter is not of type DOUBLE");
	*d = value;
}

void SystemInputParameters::Parameter::setValue(bool value)
{
	if (type != BOOL) throw std::invalid_argument("Parameter is not of type BOOL");
	*b = value;
}

void SystemInputParameters::Parameter::setValue(t_matrix value)
{
	if (type != MATRIX) throw std::invalid_argument("Parameter is not of type MATRIX");
	*m = value;
}

void SystemInputParameters::Parameter::setValue(t_ordering_rule value)
{
	if (type != ORDERING) throw std::invalid_argument("Parameter is not of type ORDERING");
	*o = value;
}

void SystemInputParameters::Parameter::setValue(t_transport_mode value)
{
	if (type != TRANSPORT) throw std::invalid_argument("Parameter is not of type TRANSPORT");
	*t = value;
}

void SystemInputParameters::Parameter::setValue(t_routing_criterion_logical_topology value)
{
	if (type != ROUTING_LOGICAL) throw std::invalid_argument("Parameter is not of type ROUTING_LOGICAL");
	*rl = value;
}

void SystemInputParameters::Parameter::setValue(t_routing_criterion_physical_topology value)
{
	if (type != ROUTING_PHYSICAL) throw std::invalid_argument("Parameter is not of type ROUTING_PHYSICAL");
	*rp = value;
}

SystemInputParameters::ParameterType SystemInputParameters::Parameter::getType()
{
	return type;
}

SystemInputParameters::Parameter::Parameter(int * elem)
{
	type = INT;
	i = elem;
}

SystemInputParameters::Parameter::Parameter(double * elem)
{
	type = DOUBLE;
	d = elem;
}

SystemInputParameters::Parameter::Parameter(bool * elem)
{
	type = BOOL;
	b = elem;
}

SystemInputParameters::Parameter::Parameter(t_matrix * elem)
{
	type = MATRIX;
	m = elem;
}

SystemInputParameters::Parameter::Parameter(t_ordering_rule * elem)
{
	type = ORDERING;
	o = elem;
}

SystemInputParameters::Parameter::Parameter(t_transport_mode * elem)
{
	type = TRANSPORT;
	t = elem;
}

SystemInputParameters::Parameter::Parameter(t_routing_criterion_logical_topology * elem)
{
	type = ROUTING_LOGICAL;
	rl = elem;
}

SystemInputParameters::Parameter::Parameter(t_routing_criterion_physical_topology * elem)
{
	type = ROUTING_PHYSICAL;
	rp = elem;
}