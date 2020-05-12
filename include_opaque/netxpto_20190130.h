# ifndef PROGRAM_INCLUDE_NETXPTO_H_
# define PROGRAM_INCLUDE_NETXPTO_H_

// ####################################################################################################
// #
// # STL header files
// #
// ####################################################################################################

# include <algorithm>	
# include <array>
# include <bitset>
# include <chrono>
# include <cmath>
# include <complex>
# include <cctype> 
# include <ctime>
# include <filesystem>
# include <fstream>
# include <functional>	
# include <iostream>
# include <locale>
# include <map>
# include <random>
# include <sstream>
# include <vector>
//# include <strstream>   // (since it is deprecated in C++17. updated by Romil on 08/05/2020)
# include <string>
# include <set>

#include <list>
#include <limits> // for numeric_limits
#include <utility> // for pair
#include <iterator>

// ####################################################################################################
// #
// # Alias for ISO C++ built-in types
// #
// ####################################################################################################

using t_unsigned_long = unsigned long int;
using t_unsigned = unsigned int;
using t_integer = int;
using t_integer_long = long int;

typedef std::vector<std::vector<int> > t_matrix;

// ####################################################################################################
// #
// # Global constants
// #
// ####################################################################################################

const int MAX_NAME_SIZE = 256;					// Maximum size of names (file names,..)
const t_integer MAX_BUFFER_LENGTH = 10000;		// Maximum Signal buffer length
const int DEFAULT_BUFFER_LENGTH = 512;
const int MAX_TOPOLOGY_SIZE = 100;				// Maximum System topology size 
const int MAX_TAPS = 1000;						// Maximum Taps Number
const double PI = 3.1415926535897932384;		// Value of pi
const double SPEED_OF_LIGHT = 299792458;		// Speed of light in vaccum
const double PLANCK_CONSTANT = 6.626070040e-34; // NIST
const int MAX_NUMBER_OF_PATHS = 2;

//using namespace std;							// to be deleted 4/9/2018 (commented by Romil 08/05/2020)

enum class survivability_method { none, protection_1_plus_1, restoration };
enum class t_ordering_rule { descendingOrder, ascendingOrder };
enum class t_transport_mode { opaque, transparent, translucent };
enum class t_routing_criterion_logical_topology { hops, km };				
enum class t_routing_criterion_physical_topology { hops, km };

typedef int vertex_t;
typedef double weight_t;

const weight_t max_weight = std::numeric_limits<double>::infinity();

struct neighbor {
	vertex_t target;
	weight_t weight;
	neighbor(vertex_t arg_target, weight_t arg_weight)
		: target(arg_target), weight(arg_weight) { }
};

typedef std::vector<std::vector<neighbor> > adjacency_list_t;


// ####################################################################################################
// #
// # Random engine
// #
// ####################################################################################################

using random_engine = std::default_random_engine;

// ####################################################################################################
// #
// # Alias for NetXpto signal value types
// #
// ####################################################################################################

using t_binary = unsigned int;
using t_integer = int;
using t_real = double;
using t_complex = std::complex<t_real>;
using t_complex_xy = struct { t_complex x; t_complex y; };
//using t_photon = struct { t_real probabilityAmplitude;  t_real polarization; }; // isto é para eliminar
//using t_photon_mp = struct { t_photon path[MAX_NUMBER_OF_PATHS]; };				// isto é para eliminar
using t_complex_xy_mp = struct { t_complex x; t_complex y; bool processed{ false }; };
using t_photon_mp_xy = struct { t_complex_xy_mp path[MAX_NUMBER_OF_PATHS]; };
using t_iqValues = std::complex<t_real>;
using t_message = struct {	std::string messageType;	std::string messageDataLength; 	std::string messageData; int size() { return 3; }};

using t_path = struct {
	t_integer pathIndex{ 0 };
	t_integer sourceNode{ 0 };
	t_integer destinationNode{ 0 };
	t_integer capacity{ 0 };
	t_integer numberOfLightPaths{ 0 };
	std::vector<t_integer> lightPathsIndex;
};

using t_light_path = struct {
	t_integer lightPathIndex{ 0 };
	t_integer sourceNode{ 0 };
	t_integer destinationNode{ 0 };
	t_integer capacity{ 0 };
	t_integer numberOfOpticalChannels{ 0 };
	std::vector<t_integer> opticalChannelsIndex;
};

using t_optical_channel = struct {
	t_integer opticalChannelIndex{ 0 };
	t_integer sourceNode{ 0 };
	t_integer destinationNode{ 0 };
	double wavelength{ 0 };
	t_integer capacity{ 0 };
	t_integer numberOfDemands{ 0 };
	std::vector<t_integer> demandsIndex;
};		

using t_logical_topology = struct {
	t_matrix logicalTopologyAdjacencyMatrix{ 0 };
	t_matrix distanceMatrix{ 0 };
	std::vector<t_path> paths;
	std::vector<t_light_path> lightPaths;
	std::vector<t_optical_channel> opticalChannels;
};

using t_optical_multiplexing_system = struct {
	t_integer OMSIndex{ 0 };
	t_integer sourceNode{ 0 };
	t_integer destinationNode{ 0 };
	t_integer maximumNumberOfWavelengths{ 0 };
	std::vector<double> wavelengths;
	std::vector<t_integer> availableWavelengths;
	t_integer numberOfAmplifiers{ 0 };
};

using t_physical_topology = struct {
	t_matrix physicalTopologyAdjacencyMatrix{ 0 };
	t_matrix distanceMatrix{ 0 };
	std::vector<t_optical_multiplexing_system> OMS;
};

using t_demand_request = struct {
	t_integer demandIndex{ 0 };
	t_integer sourceNode{ 0 };
	t_integer destinationNode{ 0 };
	t_integer oduType{ 0 };
	survivability_method survivabilityMethod{ survivability_method::none };

	void setDemandIndex(t_integer dIndex) { demandIndex = dIndex; }
	t_integer getDemandIndex() { return demandIndex; }
};

using t_demand_request_routed = struct {
	t_integer demandIndex{ 0 };
	bool routed;
	t_integer pathIndex;
};

using t_path_request = struct {
	t_integer requestIndex;
	t_integer sourceNode{ 0 };
	t_integer destinationNode{ 0 };
	t_integer numberOfIntermediateNodes{ 0 };
	std::vector<t_integer> intermediateNodes;
	//bool wavelengthContinuity;
};

using t_path_information = struct {
	t_integer requestIndex{ 0 };
	bool routed;
	t_integer numberOfLightPaths{ 0 };
};

using t_light_paths_table = struct {
	t_integer sourceNode{ 0 };
	t_integer destinationNode{ 0 };
	t_integer numberOfIntermediateNodes{ 0 };
	std::vector<t_integer> intermediateNodes;
	double wavelength{ 0 };
};

using t_path_request_routed = struct {
	t_path_information pathInformation;
	std::vector<t_light_paths_table> lightPathsTable;
};


// Existent signals
enum class signal_value_type { t_binary, t_integer, t_real, t_complex, t_complex_xy, t_photon, t_photon_mp, t_photon_mp_xy, t_iqValues, t_message, 
							   t_logical_topology, t_path, t_light_path, t_optical_channel,
							   t_physical_topology, t_optical_multiplexing_system,
							   t_demand_request, t_demand_request_routed,
							   t_path_request, t_path_request_routed
};

// #######################################################################################################
// #
// # Operator << overloading
// #
// #######################################################################################################

template<typename T>
std::ostream& operator<<(std::ostream &out, const std::complex<T> &cx) 
{ 
	out << cx.real() << " + i " << cx.imag();
	return out;
}

template<typename T>
std::ostream& operator<<(std::ostream &out, const t_demand_request &cx)
{
	out << cx.demandIndex;
	return out;
}

// ####################################################################################################
// #
// # NetXpto enuerate class types
// #
// ####################################################################################################

enum class signal_type { Binary, TimeDiscreteAmplitudeContinuousReal, TimeContinuousAmplitudeContinuousReal, PhotonStreamXY, PhotonStreamMP, PhotonStreamMPXY, 
						 LogicalTopology, PhysicalTopology, DemandRequest, DemandRequestRouted, PathRequest, PathRequestRouted };

//enum class signal_write_mode {Binary, Ascii};

//########################################################################################################################################################
//############################################################## SIGNALS DECLARATION #####################################################################
//########################################################################################################################################################

//########################################################## SIGNALS ABSTRACT CLASS ######################################################################
// The hierarchy of the signals are the following: the root is the abstract SIGNAL class, where all comum data and functions are declared,
// the second level is a template where value dependent data and member functions are declared,
// the third level is where signal type specificities are declared.
// All signals have associated a circular buffer (which is used as a temporary storage, before and after processing) and a file (permanente storage)
//#########################################################################################################################################################

// Signal abstract root class

class Signal {

public:

	// Signals constructors
	explicit Signal() {};
	explicit Signal(std::string fileName) : fileName{ fileName }, saveSignal{ true } {};
	explicit Signal(std::string fileName, t_unsigned_long bLength) : fileName{ fileName }, bufferLength{ bLength }, saveSignal{ true } {};
	explicit Signal(std::string fileName, std::string folderName) : fileName{ fileName }, folderName{ folderName }, saveSignal{ true } {};
	explicit Signal(std::string fileName, std::string folderName, t_unsigned_long bLength) : fileName{ fileName }, folderName{ folderName }, bufferLength{ bLength }, saveSignal{ true } {};
	explicit Signal(t_unsigned_long bLength) : bufferLength{ bLength } {};

	// Signal destructors
	//~Signal() { if (!(valueType == signal_value_type::t_message)) { delete[] buffer; }; };	

	// Buffer manipulation funtions
	t_integer ready();										// Returns the number of samples in the buffer ready to be processed
	t_integer space();										// Returns the signal buffer space without originating signal overlap
	template<typename T> void bufferPut(T value);			// Puts a value in the buffer (at the end)
	template<typename T> void bufferGet(T* valueAddr);		// Returns a value from the buffer (from the begining)	
	void bufferGet();

	// File manipulation
	void writeHeader();				                // Opens the signal file in the default signals directory, \signals, and writes the signal header
	void writeHeader(std::string signalPath);			// Opens the signal file in the signalPath directory, and writes the signal header

	// Buffer and File manipulation
	void close();									// Empty the signal buffer and close the signal file

	void reset() { inPosition = 0; outPosition = inPosition; bufferEmpty = true; bufferFull = false; }

	// Change signal parameters
	void setSaveSignal(bool sSignal) { saveSignal = sSignal; };
	bool getSaveSignal() const { return saveSignal; };

	void setType(std::string sType, signal_value_type vType) { type = sType; valueType = vType; };
	void setType(std::string sType) { type = sType; };
	std::string getType() const { return type; };

	void setInPosition(int iPosition) { inPosition = iPosition; };
	int getInPosition() const { return inPosition; };

	void setOutPosition(int oPosition) { outPosition = oPosition; };
	int getOutPosition() const { return outPosition; };

	void setBufferEmpty(bool bEmpty) { bufferEmpty = bEmpty; };
	bool getBufferEmpty() const { return bufferEmpty; };

	void setBufferFull(bool bFull) { bufferFull = bFull; };
	bool getBufferFull() const { return bufferFull; };

	//void setValueType(signal_value_type vType) { valueType = vType; };
	signal_value_type getValueType() const { return valueType; };

	void setFileName(std::string fName) { fileName = fName; };
	std::string getFileName() const { return fileName; };

	void setFolderName(std::string fName) { folderName = fName; };
	std::string getFolderName() const { return folderName; };

	//void setBufferLength(t_unsigned_long bLength) { bufferLength = bLength; };
	int getBufferLength() const { return bufferLength; };

	void setFirstValueToBeSaved(t_unsigned_long fValueToBeSaved) { firstValueToBeSaved = fValueToBeSaved; };
	t_unsigned_long getFirstValueToBeSaved() const { return firstValueToBeSaved; };

	void setNumberOfValuesToBeSaved(t_unsigned_long nOfValuesToBeSaved) { numberOfValuesToBeSaved = nOfValuesToBeSaved; };
	t_unsigned_long getNumberOfValuesToBeSaved() const { return numberOfValuesToBeSaved; };

	void setSymbolPeriod(double sPeriod) { symbolPeriod = sPeriod; samplesPerSymbol = symbolPeriod / samplingPeriod; };
	double getSymbolPeriod() const { return symbolPeriod; };

	void setSamplingPeriod(double sPeriod) { samplingPeriod = sPeriod; samplesPerSymbol = symbolPeriod / samplingPeriod; };
	double getSamplingPeriod() const { return samplingPeriod; };

	void setSamplesPerSymbol(double sPeriod) { samplesPerSymbol = sPeriod; symbolPeriod = samplesPerSymbol * samplingPeriod; samplingPeriod = symbolPeriod / samplesPerSymbol; };
	double getSamplesPerSymbol() const { return samplesPerSymbol; };

	void setCentralFrequency(double cFrequency) { centralFrequency = cFrequency; centralWavelength = SPEED_OF_LIGHT / centralFrequency; }
	double getCentralFrequency() const { return centralFrequency; };

	void setCentralWavelength(double cWavelength) { centralWavelength = cWavelength; centralFrequency = SPEED_OF_LIGHT / centralWavelength; }
	double getCentralWavelength() const { return centralWavelength; }

	void setSaveInAscii(bool sInAscii) { saveInAscii = sInAscii; }
	bool getSaveInAscii() const { return saveInAscii; }

	template<typename t, signal_type sType, signal_value_type vType> friend class BaseSignal;

private:

	/* Circular buffer state variables */
	void* buffer{ nullptr };											// Pointer to buffer
	t_unsigned inPosition{ 0 };									// Next position for the buffer input values
	t_unsigned outPosition{ 0 };									// Next position for the buffer output values
	bool bufferEmpty{ true };											// Flag bufferEmpty
	bool bufferFull{ false };											// Flag bufferFull
	const t_unsigned bufferLength{ DEFAULT_BUFFER_LENGTH };		// Buffer length
	bool finish {false};

	t_unsigned_long numberOfSavedValues{ 0 };							// Number of saved values
	t_unsigned_long count;												// Number of values that have already entered in the buffer

	/* Input Parameters */

	t_unsigned_long firstValueToBeSaved{ 1 };						// First value (>= 1) to be saved
	bool saveSignal{ false };

	std::string type;											// Signal type
	signal_value_type valueType;							// Signal samples type

	std::string fileName{ "" };							// Name of the file where data values are going to be saved
	std::string folderName{ "signals" };					// folder where signals are going to be saved by default
	bool headerWritten{ false };
	
	bool saveInAscii{ false };

	long int numberOfValuesToBeSaved{ -1 };			// Number of values to be saved, if -1 all values are going to be saved

	double symbolPeriod{ 1.0 };						// Signal symbol period (it is the inverse of the symbol rate)
	double samplingPeriod{ 1.0 };					// Signal sampling period (it is the time space between two samples)
	double samplesPerSymbol{ 1.0 };					// samplesPerSymbol = symbolPeriod / samplingPeriod;

	double centralWavelength{ 1550E-9 };
	double centralFrequency{ SPEED_OF_LIGHT / centralWavelength };
};

template<typename T, signal_type sType, signal_value_type vType>
class BaseSignal : public Signal {

public:

	using value_type = T;

	BaseSignal() : Signal() { setBuffer(); }
	BaseSignal(std::string fileName) : Signal(fileName) { setBuffer(); }
	BaseSignal(std::string fileName, t_unsigned_long bLength) : Signal(fileName, bLength) { setBuffer(); }
	BaseSignal(std::string fileName, std::string folderName) : Signal(fileName, folderName) { setBuffer(); }
	BaseSignal(std::string fileName, std::string folderName, t_unsigned_long bLength) : Signal(fileName, folderName, bLength) { setBuffer(); }
	BaseSignal(t_unsigned_long bLength) : Signal(bLength) { setBuffer(); }

	value_type bufferGet()
	{
		if (bufferEmpty) return value_type();

		value_type value = static_cast<value_type *>(buffer)[outPosition];

		bufferFull = false;

		outPosition = (outPosition + 1) % bufferLength;

		bufferEmpty = outPosition == inPosition;

		return value;
	}

	/*
	void bufferPut(value_type value)
	{
		(static_cast<T *>(buffer))[inPosition] = value;
		if (bufferFull)
		{
			outPosition = (outPosition + 1) % bufferLength;
		}
		inPosition = (inPosition + 1) % bufferLength;
		bufferEmpty = false;
		bufferFull = inPosition == outPosition;
		if (bufferFull)
		{
			if (saveSignal)
			{
				if (!headerWritten) writeHeader();
				if (firstValueToBeSaved <= bufferLength)
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
					firstValueToBeSaved = firstValueToBeSaved - bufferLength;
				}
			}
		}
	}
	*/
	value_type& operator[](int index) { return (static_cast<value_type*>(buffer)[(outPosition + index) % bufferLength]); }

private:

	void setBuffer() {

		std::string typeName;
		switch (sType) {
			case signal_type::Binary:
				typeName = "Binary";
				break;
			case signal_type::TimeDiscreteAmplitudeContinuousReal:
				typeName = "TimeDiscreteAmplitudeContinuousReal";
				break;
			case signal_type::TimeContinuousAmplitudeContinuousReal:
				typeName = "TimeContinuousAmplitudeContinuousReal";
				break;
			case signal_type::PhotonStreamXY:
				typeName = "PhotonStreamXY";
				break;
			case signal_type::PhotonStreamMP:
				typeName = "PhotonStreamMP";
				break;
			case signal_type::PhotonStreamMPXY:
				typeName = "PhotonStreamMPXY";
				break;
			case signal_type::LogicalTopology:
				typeName = "LogicalTopology";
				break;
			case signal_type::PhysicalTopology:
				typeName = "PhysicalTopology";
				break;
			case signal_type::DemandRequest:
				typeName = "DemandRequest";
				break;
			case signal_type::DemandRequestRouted:
				typeName = "DemandRequestRouted";
				break;
			case signal_type::PathRequest:
				typeName = "PathRequest";
				break;
			case signal_type::PathRequestRouted:
				typeName = "PathRequestRouted";
				break;
			
			default:
				std::cout << "Error: netxpto_20180830.h - typeName not defined\n";
		}

		setType(typeName, vType);
		if (buffer == nullptr) buffer = (value_type*) (new value_type[getBufferLength()]);
	}

};

using Binary = BaseSignal< t_binary, signal_type::Binary, signal_value_type::t_binary> ;
using TimeDiscreteAmplitudeContinuousReal = BaseSignal<t_real, signal_type::TimeDiscreteAmplitudeContinuousReal, signal_value_type::t_real>;
using TimeContinuousAmplitudeContinuousReal = BaseSignal<t_real, signal_type::TimeContinuousAmplitudeContinuousReal, signal_value_type::t_real>;
using PhotonStreamXY = BaseSignal<t_complex_xy, signal_type::PhotonStreamXY, signal_value_type::t_complex_xy>;
//using PhotonStreamMP = BaseSignal<t_photon_mp, signal_type::PhotonStreamMP, signal_value_type::t_photon_mp>;
using PhotonStreamMPXY = BaseSignal<t_photon_mp_xy, signal_type::PhotonStreamMPXY, signal_value_type::t_photon_mp_xy>;
using LogicalTopology = BaseSignal<t_logical_topology, signal_type::LogicalTopology, signal_value_type::t_logical_topology>;
using PhysicalTopology = BaseSignal<t_physical_topology, signal_type::PhysicalTopology, signal_value_type::t_physical_topology>;
using DemandRequest = BaseSignal<t_demand_request, signal_type::DemandRequest, signal_value_type::t_demand_request>;
using DemandRequestRouted = BaseSignal<t_demand_request_routed, signal_type::DemandRequestRouted, signal_value_type::t_demand_request_routed>;
using PathRequest = BaseSignal<t_path_request, signal_type::PathRequest, signal_value_type::t_path_request>;
using PathRequestRouted = BaseSignal<t_path_request_routed, signal_type::PathRequestRouted, signal_value_type::t_path_request_routed>;

/*
class TimeDiscrete : public Signal {
public:
	TimeDiscrete(std::string fName) { setFileName(fName); }
	TimeDiscrete(std::string fileName,std::string folderName) : Signal(fileName,folderName) { }
	TimeDiscrete(){}
};
class TimeDiscreteAmplitudeDiscrete : public Signal {
public:
	TimeDiscreteAmplitudeDiscrete(std::string fName) { setFileName(fName); }
	TimeDiscreteAmplitudeDiscrete(std::string fileName, std::string folderName) : Signal(fileName, folderName) { }
	TimeDiscreteAmplitudeDiscrete(){}
};
class TimeDiscreteAmplitudeContinuous : public Signal {
public:
	TimeDiscreteAmplitudeContinuous(std::string fName) { setFileName(fName); }
	TimeDiscreteAmplitudeContinuous(std::string fileName, std::string folderName) : Signal(fileName, folderName) { }
	TimeDiscreteAmplitudeContinuous(){}
};
class TimeDiscreteAmplitudeDiscreteReal : public Signal {
public:
	TimeDiscreteAmplitudeDiscreteReal(std::string fName) { setType("TimeDiscreteAmplitudeDiscreteReal", signal_value_type::RealValue); setFileName(fName); if (buffer == nullptr) buffer = new t_real[getBufferLength()]; }
	TimeDiscreteAmplitudeDiscreteReal(std::string fileName, std::string folderName) : Signal(fileName,folderName){ setType("TimeDiscreteAmplitudeDiscreteReal", signal_value_type::RealValue); if (buffer == nullptr) buffer = new t_real[getBufferLength()]; }
	TimeDiscreteAmplitudeDiscreteReal(std::string fName, t_unsigned_long bLength) { setType("TimeDiscreteAmplitudeDiscreteReal", signal_value_type::RealValue); setFileName(fName); setBufferLength(bLength); }
	TimeDiscreteAmplitudeDiscreteReal(t_unsigned_long bLength) { setType("TimeDiscreteAmplitudeDiscreteReal", signal_value_type::RealValue); setBufferLength(bLength); }
	TimeDiscreteAmplitudeDiscreteReal(){ setType("TimeDiscreteAmplitudeDiscreteReal", signal_value_type::RealValue); if (buffer == nullptr) buffer = new t_real[getBufferLength()]; }
//	void setBufferLength(t_unsigned_long bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_real[bLength]; };
};
class TimeDiscreteAmplitudeDiscreteComplex : public Signal {
	
public:
	TimeDiscreteAmplitudeDiscreteComplex(std::string fName) { setFileName(fName); }
	TimeDiscreteAmplitudeDiscreteComplex(std::string fileName, std::string folderName) : Signal(fileName, folderName) { }
	TimeDiscreteAmplitudeDiscreteComplex() {}
};
*/
/*
class Binary : public Signal {
	
public:
	using value_type = t_binary;
	Binary() : Signal() { setBuffer(); }
	Binary(std::string fileName) : Signal(fileName) { setBuffer(); }
	Binary(std::string fileName, t_unsigned_long bLength) : Signal(fileName, bLength) { setBuffer(); }
	Binary(std::string fileName, std::string folderName) : Signal(fileName, folderName) { setBuffer(); }
	Binary(std::string fileName, std::string folderName, t_unsigned_long bLength) : Signal(fileName, folderName, bLength) { setBuffer(); }
	Binary(t_unsigned_long bLength) : Signal(bLength) { setBuffer(); }
	Binary(std::string fileName, bool sSignal) : Signal(fileName, sSignal) { setBuffer(); }
	Binary(std::string fileName, t_unsigned_long bLength, bool sSignal) : Signal(fileName, bLength, sSignal) { setBuffer(); }
	Binary(std::string fileName, std::string folderName, bool sSignal) : Signal(fileName, folderName, sSignal) { setBuffer(); }
	Binary(std::string fileName, std::string folderName, t_unsigned_long bLength, bool sSignal) : Signal(fileName, folderName, bLength, sSignal) { setBuffer(); }
private:
	void setBuffer() { setType("Binary", signal_value_type::BinaryValue); if (buffer == nullptr) buffer = new value_type[getBufferLength()]; }
};
*/
/*
class TimeDiscreteAmplitudeContinuousReal : public Signal {
public:
	TimeDiscreteAmplitudeContinuousReal(std::string fName) { setType("TimeDiscreteAmplitudeContinuousReal", signal_value_type::RealValue); setFileName(fName); if (buffer == nullptr) buffer = new t_real[getBufferLength()]; }
	TimeDiscreteAmplitudeContinuousReal(std::string fileName, std::string folderName) : Signal(fileName,folderName){ setType("TimeDiscreteAmplitudeContinuousReal", signal_value_type::RealValue); if (buffer == nullptr) buffer = new t_real[getBufferLength()];}
	TimeDiscreteAmplitudeContinuousReal(std::string fName, t_unsigned_long bLength) { setType("TimeDiscreteAmplitudeContinuousReal", signal_value_type::RealValue); setFileName(fName); setBufferLength(bLength); }
	TimeDiscreteAmplitudeContinuousReal(t_unsigned_long bLength) { setType("TimeDiscreteAmplitudeContinuousReal", signal_value_type::RealValue); setBufferLength(bLength); }
	TimeDiscreteAmplitudeContinuousReal(){ setType("TimeDiscreteAmplitudeContinuousReal", signal_value_type::RealValue); if (buffer == nullptr) buffer = new t_real[getBufferLength()]; }
	void setBufferLength(t_unsigned_long bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_real[bLength]; };
};
class TimeDiscreteAmplitudeContinuousComplex : public Signal {
public:
	TimeDiscreteAmplitudeContinuousComplex(std::string fName) { setType("TimeDiscreteAmplitudeContinuousComplex", signal_value_type::ComplexValue); setFileName(fName); if (buffer == nullptr) buffer = new t_complex[getBufferLength()]; }
	TimeDiscreteAmplitudeContinuousComplex(std::string fileName, std::string folderName) : Signal(fileName,folderName){ setType("TimeDiscreteAmplitudeContinuousComplex", signal_value_type::ComplexValue); if (buffer == nullptr) buffer = new t_complex[getBufferLength()];}
	TimeDiscreteAmplitudeContinuousComplex(std::string fName, t_unsigned_long bLength) { setType("TimeDiscreteAmplitudeContinuousComplex", signal_value_type::ComplexValue); setFileName(fName); setBufferLength(bLength); }
	TimeDiscreteAmplitudeContinuousComplex(t_unsigned_long bLength) { setType("TimeDiscreteAmplitudeContinuousComplex", signal_value_type::ComplexValue); setBufferLength(bLength); }
	TimeDiscreteAmplitudeContinuousComplex(){ setType("TimeDiscreteAmplitudeContinuousComplex", signal_value_type::ComplexValue); if (buffer == nullptr) buffer = new t_complex[getBufferLength()]; }
	void setBufferLength(t_unsigned_long bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_complex[bLength]; };
};
class TimeContinuous : public Signal {
public:
	TimeContinuous(std::string fileName) : Signal(fileName) { }
	TimeContinuous(std::string fileName, std::string folderName) : Signal(fileName, folderName) { }
	TimeContinuous(){}
};
class TimeContinuousAmplitudeDiscrete : public Signal {
public:
	TimeContinuousAmplitudeDiscrete(std::string fileName) : Signal(fileName) { }
	TimeContinuousAmplitudeDiscrete(std::string fileName, std::string folderName) : Signal(fileName, folderName) { }
	TimeContinuousAmplitudeDiscrete(){}
};
class TimeContinuousAmplitudeContinuous : public Signal {
public:
	TimeContinuousAmplitudeContinuous(std::string fName) { setType("TimeContinuousAmplitudeContinuous", signal_value_type::RealValue);  setFileName(fName); if (buffer == nullptr) buffer = new t_real[getBufferLength()]; }
	TimeContinuousAmplitudeContinuous(std::string fileName, std::string folderName) : Signal(fileName, folderName) { setType("TimeContinuousAmplitudeContinuous", signal_value_type::RealValue); if (buffer == nullptr) buffer = new t_real[getBufferLength()];	}
	TimeContinuousAmplitudeContinuous(std::string fName, t_unsigned_long bLength) { setType("TimeContinuousAmplitudeContinuous", signal_value_type::RealValue);  setFileName(fName); setBufferLength(bLength); }
	TimeContinuousAmplitudeContinuous(t_unsigned_long bLength) { setType("TimeContinuousAmplitudeContinuous", signal_value_type::RealValue);  setBufferLength(bLength); }
	TimeContinuousAmplitudeContinuous() { setType("TimeContinuousAmplitudeContinuous", signal_value_type::RealValue); if (buffer == nullptr) buffer = new t_real[getBufferLength()]; }
	void setBufferLength(t_unsigned_long bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_real[bLength]; };
};
class TimeContinuousAmplitudeDiscreteReal : public Signal {
public:
	TimeContinuousAmplitudeDiscreteReal(std::string fName) { setType("TimeContinuousAmplitudeDiscreteReal", signal_value_type::RealValue);  setFileName(fName); if (buffer == nullptr) buffer = new t_real[getBufferLength()]; }
	TimeContinuousAmplitudeDiscreteReal(std::string fileName, std::string folderName) : Signal(fileName, folderName) { setType("TimeContinuousAmplitudeDiscreteReal", signal_value_type::RealValue); if (buffer == nullptr) buffer = new t_real[getBufferLength()];}
	TimeContinuousAmplitudeDiscreteReal(std::string fName, t_unsigned_long bLength) { setType("TimeContinuousAmplitudeDiscreteReal", signal_value_type::RealValue);  setFileName(fName); setBufferLength(bLength); }
	TimeContinuousAmplitudeDiscreteReal(t_unsigned_long bLength) { setType("TimeContinuousAmplitudeDiscreteReal", signal_value_type::RealValue);  setBufferLength(bLength); }
	TimeContinuousAmplitudeDiscreteReal(){ setType("TimeContinuousAmplitudeDiscreteReal", signal_value_type::RealValue); if (buffer == nullptr) buffer = new t_real[getBufferLength()]; }
	void setBufferLength(t_unsigned_long bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_real[bLength]; };
};
class TimeContinuousAmplitudeDiscreteComplex : public Signal {
public:
	TimeContinuousAmplitudeDiscreteComplex(std::string fName) { setType("TimeContinuousAmplitudeDiscreteComplex", signal_value_type::ComplexValue); setFileName(fName); if (buffer == nullptr) buffer = new t_complex[getBufferLength()]; }
	TimeContinuousAmplitudeDiscreteComplex(std::string fileName, std::string folderName) : Signal(fileName, folderName) { setType("TimeContinuousAmplitudeDiscreteComplex", signal_value_type::ComplexValue); if (buffer == nullptr) buffer = new t_complex[getBufferLength()];}
	TimeContinuousAmplitudeDiscreteComplex(std::string fName, t_unsigned_long bLength) { setType("TimeContinuousAmplitudeDiscreteComplex", signal_value_type::ComplexValue); setFileName(fName); setBufferLength(bLength); }
	TimeContinuousAmplitudeDiscreteComplex(t_unsigned_long bLength) { setType("TimeContinuousAmplitudeDiscreteComplex", signal_value_type::ComplexValue); setBufferLength(bLength); }
	TimeContinuousAmplitudeDiscreteComplex(){ setType("TimeContinuousAmplitudeDiscreteComplex", signal_value_type::ComplexValue); if (buffer == nullptr) buffer = new t_complex[getBufferLength()]; }
	void setBufferLength(t_unsigned_long bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_complex[bLength]; };
};
class TimeContinuousAmplitudeContinuousReal : public Signal {
public:
	TimeContinuousAmplitudeContinuousReal(std::string fName) { setType("TimeContinuousAmplitudeContinuousReal", signal_value_type::RealValue); setFileName(fName); if (buffer == nullptr) buffer = new t_real[getBufferLength()]; }
	TimeContinuousAmplitudeContinuousReal(std::string fileName,std::string folderName) : Signal(fileName, folderName) { setType("TimeContinuousAmplitudeContinuousReal", signal_value_type::RealValue); if (buffer == nullptr) buffer = new t_real[getBufferLength()];}
	TimeContinuousAmplitudeContinuousReal(std::string fName, t_unsigned_long bLength) { setType("TimeContinuousAmplitudeContinuousReal", signal_value_type::RealValue); setFileName(fName); setBufferLength(bLength); }
	TimeContinuousAmplitudeContinuousReal(t_unsigned_long bLength) { setType("TimeContinuousAmplitudeContinuousReal", signal_value_type::RealValue); setBufferLength(bLength); }
	TimeContinuousAmplitudeContinuousReal(){ setType("TimeContinuousAmplitudeContinuousReal", signal_value_type::RealValue); if (buffer == nullptr) buffer = new t_real[getBufferLength()]; }
	void setBufferLength(t_unsigned_long bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_real[bLength]; };
};
class TimeContinuousAmplitudeContinuousComplex : public Signal {
public:
	TimeContinuousAmplitudeContinuousComplex(std::string fName) { setType("TimeContinuousAmplitudeContinuousComplex", signal_value_type::ComplexValue); setFileName(fName); if (buffer == nullptr) buffer = new t_complex[getBufferLength()]; }
	TimeContinuousAmplitudeContinuousComplex(std::string fileName, std::string folderName) : Signal(fileName, folderName) { setType("TimeContinuousAmplitudeContinuousComplex", signal_value_type::ComplexValue); if (buffer == nullptr) buffer = new t_complex[getBufferLength()];}
	TimeContinuousAmplitudeContinuousComplex(std::string fName, t_unsigned_long bLength) { setType("TimeContinuousAmplitudeContinuousComplex", signal_value_type::ComplexValue); setFileName(fName); setBufferLength(bLength); }
	TimeContinuousAmplitudeContinuousComplex(t_unsigned_long bLength) { setType("TimeContinuousAmplitudeContinuousComplex", signal_value_type::ComplexValue); setBufferLength(bLength); }
	TimeContinuousAmplitudeContinuousComplex(){ setType("TimeContinuousAmplitudeContinuousComplex", signal_value_type::ComplexValue); if (buffer == nullptr) buffer = new t_complex[getBufferLength()]; }
	void setBufferLength(t_unsigned_long bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_complex[bLength]; };
};
class BandpassSignal : public Signal {
public:
	BandpassSignal(std::string fName) { setType("BandpassSignal", signal_value_type::ComplexValue); setFileName(fName); if (buffer == nullptr) buffer = new t_complex[getBufferLength()]; }
	BandpassSignal(std::string fileName, std::string folderName) : Signal(fileName, folderName) { setType("BandpassSignal", signal_value_type::ComplexValue); setFileName(fileName); if (buffer == nullptr) buffer = new t_complex[getBufferLength()];}
	BandpassSignal(std::string fName, t_unsigned_long bLength) { setType("BandpassSignal", signal_value_type::ComplexValue); setFileName(fName); setBufferLength(bLength); }
	BandpassSignal(t_unsigned_long bLength) { setType("BandpassSignal", signal_value_type::ComplexValue); setBufferLength(bLength); }
	BandpassSignal(){ setType("BandpassSignal", signal_value_type::ComplexValue); if (buffer == nullptr) buffer = new t_complex[getBufferLength()]; }
	void setBufferLength(t_unsigned_long bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_complex[bLength]; };
};
// the setType is BandpassSignal to garantee the compatibility with the Visualizer
class OpticalSignal : public Signal {
public:
	OpticalSignal(std::string fName) { setType("BandpassSignal", signal_value_type::ComplexValue); setFileName(fName); if (buffer == nullptr) buffer = new t_complex[getBufferLength()]; }
	OpticalSignal(std::string fileName, std::string folderName) : Signal(fileName,folderName) { setType("BandpassSignal", signal_value_type::ComplexValue); if (buffer == nullptr) buffer = new t_complex[getBufferLength()];}
	OpticalSignal(std::string fName, t_unsigned_long bLength) { setType("BandpassSignal", signal_value_type::ComplexValue); setFileName(fName); setBufferLength(bLength); }
	OpticalSignal(t_unsigned_long bLength) { setType("BandpassSignal", signal_value_type::ComplexValue); setBufferLength(bLength); }
	OpticalSignal() { setType("BandpassSignal", signal_value_type::ComplexValue); if (buffer == nullptr) buffer = new t_complex[getBufferLength()]; }
	void setBufferLength(t_unsigned_long bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_complex[bLength]; };
};
class OpticalSignalXY : public Signal {
public:
	OpticalSignalXY(std::string fName) { setType("OpticalSignalXY", signal_value_type::ComplexValueXY); setFileName(fName); if (buffer == nullptr) buffer = new t_complex_xy[getBufferLength()]; }
	OpticalSignalXY(std::string fileName, std::string folderName) : Signal(fileName, folderName) { setType("OpticalSignalXY", signal_value_type::ComplexValueXY); if (buffer == nullptr) buffer = new t_complex_xy[getBufferLength()];}
	OpticalSignalXY(std::string fName, t_unsigned_long bLength) { setType("OpticalSignalXY", signal_value_type::ComplexValueXY); setFileName(fName); setBufferLength(bLength); }
	OpticalSignalXY(t_unsigned_long bLength) { setType("OpticalSignalXY", signal_value_type::ComplexValueXY); setBufferLength(bLength); }
	OpticalSignalXY() { setType("OpticalSignalXY", signal_value_type::ComplexValueXY); if (buffer == nullptr) buffer = new t_complex_xy[getBufferLength()]; }
	void setBufferLength(t_unsigned_long bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_complex_xy[bLength]; };
};
*/

/*class MultiModeBandpassSignal : BandpassSignal {
public:
	MultiModeBandpassSignal(int nBandpassSignals) {
	};
private:
	int numberOfBandpassSignals;
	std::vector<BandpassSignal> bandpasslSignals;
	std::vector<double> centralWavelengths;
	std::vector<double> centralFrequencies;
};*/
/*
class PhotonStream : public Signal {
public:
	PhotonStream(std::string fileName, std::string folderName) : Signal(fileName, folderName) { setType("PhotonStream", signal_value_type::PhotonValue); if (buffer == nullptr) buffer = new t_photon[getBufferLength()];}
	PhotonStream(t_unsigned_long bLength) { setType("PhotonStream", signal_value_type::PhotonValue); setBufferLength(bLength); }
	PhotonStream() { setType("PhotonStream", signal_value_type::PhotonValue); if (buffer == nullptr) buffer = new t_photon[getBufferLength()]; }
	void setBufferLength(t_unsigned_long bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_photon[bLength]; };
};
class PhotonStreamXY : public Signal {
public:
	PhotonStreamXY(std::string fName) { setType("PhotonStreamXY", signal_value_type::ComplexValueXY); setFileName(fName); if (buffer == nullptr) buffer = new t_complex_xy[getBufferLength()]; }
	PhotonStreamXY(std::string fileName, std::string folderName) : Signal(fileName, folderName) { setType("PhotonStreamXY", signal_value_type::ComplexValueXY); if (buffer == nullptr) buffer = new t_complex_xy[getBufferLength()];	}
	PhotonStreamXY(std::string fName, t_unsigned_long bLength) { setType("PhotonStreamXY", signal_value_type::ComplexValueXY); setFileName(fName); setBufferLength(bLength); }
	PhotonStreamXY(t_unsigned_long bLength) { setType("PhotonStreamXY", signal_value_type::ComplexValueXY); setBufferLength(bLength); }
	PhotonStreamXY() { setType("PhotonStreamXY", signal_value_type::ComplexValueXY); if (buffer == nullptr) buffer = new t_complex_xy[getBufferLength()]; }
	void setBufferLength(t_unsigned_long bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_complex_xy[bLength]; };
};
class PhotonStreamMP : public Signal {
public:
	PhotonStreamMP(std::string fileName, std::string folderName) : Signal(fileName, folderName) { setType("PhotonStreamMP", signal_value_type::PhotonValueMP); if (buffer == nullptr) buffer = new t_photon_mp[getBufferLength()]; }
	PhotonStreamMP(t_unsigned_long bLength) { setType("PhotonStreamMP", signal_value_type::PhotonValueMP); setBufferLength(bLength); }
	PhotonStreamMP() { setType("PhotonStreamMP", signal_value_type::PhotonValueMP); if (buffer == nullptr) buffer = new t_photon_mp[getBufferLength()]; }
	void setBufferLength(t_unsigned_long bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_photon_mp[bLength]; };
};
class PhotonStreamMPXY : public Signal {
public:
	PhotonStreamMPXY(std::string fName) { setType("PhotonStreamMPXY", signal_value_type::PhotonValueMPXY); setFileName(fName); if (buffer == nullptr) buffer = new t_photon_mp_xy[getBufferLength()]; }
	PhotonStreamMPXY(std::string fileName, std::string folderName) : Signal(fileName, folderName) { setType("PhotonStreamMPXY", signal_value_type::PhotonValueMPXY); if (buffer == nullptr) buffer = new t_photon_mp_xy[getBufferLength()];}
	PhotonStreamMPXY(std::string fName, t_unsigned_long bLength) { setType("PhotonStreamMPXY", signal_value_type::PhotonValueMPXY); setFileName(fName); setBufferLength(bLength); }
	PhotonStreamMPXY(t_unsigned_long bLength) { setType("PhotonStreamMPXY", signal_value_type::PhotonValueMPXY); setBufferLength(bLength); }
	PhotonStreamMPXY() { setType("PhotonStreamMPXY", signal_value_type::PhotonValueMPXY); if (buffer == nullptr) buffer = new t_photon_mp_xy[getBufferLength()]; }
	void setBufferLength(t_unsigned_long bLength) { setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_photon_mp_xy[bLength]; };
};
class Messages : public Signal {
public:
	Messages(std::string fName) { setType("Message", signal_value_type::Message); setFileName(fName); if (buffer == nullptr) buffer = new t_message[getBufferLength()]; }
	Messages(std::string fileName, std::string folderName) : Signal(fileName, folderName) { setType("Message", signal_value_type::Message); if (buffer == nullptr) buffer = new t_message[getBufferLength()];}
	Messages(std::string fName, t_unsigned_long bLength) { setType("Message", signal_value_type::Message); setFileName(fName); setBufferLength(bLength); }
	Messages(t_unsigned_long bLength) { setType("Message", signal_value_type::Message); setBufferLength(bLength); }
	Messages() { setType("Message", signal_value_type::Message); if (buffer == nullptr) buffer = new t_message[getBufferLength()]; }
	void setBufferLength(t_unsigned_long bLength) { Signal::setBufferLength(bLength); delete[] buffer; if (bLength != 0) buffer = new t_message[bLength]; };
	
	void bufferPut(t_message);
};
*/

//########################################################################################################################################################
//########################################################## GENERIC BLOCK DECLARATIONS AND DEFINITIONS ##################################################
//########################################################################################################################################################

// Descrives a generic Block.
class Block {

public:

	/* Methods */
	Block(){};
	Block(std::vector<Signal*> &InputSig, std::vector<Signal*> &OutputSig);
	Block(std::initializer_list<Signal*> InputSig, std::initializer_list<Signal*> OutputSig);


	//void initializeBlock(std::vector<Signal*> InputSig, std::vector<Signal*> OutputSig);
	void initializeBlock();

	virtual void initialize(void){};

	virtual bool runBlock();

	void terminateBlock();
	virtual void terminate(void){};

	void closeOutputSignals();

	void setNumberOfInputSignals(int nOfInputSignal) { numberOfInputSignals = nOfInputSignal; };
	int getNumberOfInputSignals() { return numberOfInputSignals; };

	void setNumberOfOutputSignals(int nOfOutputSignal) { numberOfOutputSignals = nOfOutputSignal; };
	int getNumberOfOutputSignals() { return numberOfOutputSignals; };

	std::vector<Signal *> inputSignals;
	std::vector<Signal *> outputSignals;

	int numberOfInputSignals;	// passar para private, 2018/07/15
	int numberOfOutputSignals;  // passar para private, 2018/07/15

};


//########################################################################################################################################################
//########################################################## GENERIC SYSTEM DECLARATIONS AND DEFINITIONS #################################################
//########################################################################################################################################################


class System {

public:

	//######################################################################################################

	System() {};
	void setSystem(std::initializer_list<Block *> MainSystem);

	System(std::initializer_list<Block *> MainSystem);
	//System(std::initializer_list<Block> MainSystme);
	//System(std::vector<Block *> MainSystem);
	System(std::initializer_list<Block *> MainSystem, std::string signalsFolderName, std::vector<std::string> list);
	//System(std::vector<Block *> MainSystem, std::string signalsFolderName, std::vector<std::string> list);

	bool run();
	bool run(std::string signalPath);
	void terminate();
	void terminateSuperBlock();
	void writeReport();

	//########################################################################################################

	void setLogValue(bool value);
	bool getLogValue() { return logValue; };
	void setOpenFile(bool value);
	bool getOpenFile() { return openFile; };
	void setLogFileName(std::string newName);
	std::string getLogFileName() { return logFileName; };
	void setSignalsFolderName(std::string newName);
	std::string getSignalsFolderName() { return signalsFolder; };
	void setLoadedInputParameters(std::vector<std::string> loadedInputParams);
	std::vector<std::string> getLoadedInputParameters() { return loadedInputParameters; };
	size_t getSystemBlocksSize() { return SystemBlocks.size(); };
	std::vector<Block *> getSystemBlocks() { return SystemBlocks; };
	
	//########################################################################################################

private:
	std::string signalsFolder{ "signals" };
	char fileName[MAX_NAME_SIZE];  // Name of the file with system description (.sdf)
	char name[MAX_NAME_SIZE];  // System Name
	int numberOfBlocks;  // Number of system Blocks
	int(*topology)[MAX_TOPOLOGY_SIZE];  // Relationship matrix
	std::vector<Block *> SystemBlocks;  // Pointer to an array of pointers to Block objects
								   //Log File Inputs
	std::string logFileName{ "log.txt" }; // The name of the file where the debug info will be written
	bool logValue{ true }; // Will write debug info if true
	bool openFile{ true };
	std::ofstream logFile;
	std::vector<std::string> loadedInputParameters;
};

//########################################################################################################################################################
//#################################################### GENERIC SUPER BLOCK DECLARATIONS AND DEFINITIONS ##################################################
//########################################################################################################################################################


class SuperBlock : public Block {

	/* State Variables */

	std::vector<Block*> moduleBlocks;
	std::vector<Block*> blocks;
	System superBlockSystem;

	/* Input Parameters */

	bool saveInternalSignals{ false };

public:


	/* Methods */
	//SuperBlock() {};
	//SuperBlock(std::vector<Signal *> &inputSignals, std::vector<Signal *> &outputSignal) : Block(inputSignals, outputSignals) { superBlockSystem.setLogValue(false); };
	SuperBlock(std::initializer_list<Signal *> inputSignals, std::initializer_list<Signal *> outputSignals) : Block(inputSignals, outputSignals) { superBlockSystem.setLogValue(false); };

	void initialize(void);

	virtual bool runBlock(std::string signalPath);

	void terminate(void);

	/* Set Methods */

	//void setBlocks(std::vector<Block*> blks) { blocks = blks; };
	void setSuperBlockSystem(std::initializer_list<Block*> blks) { superBlockSystem.setSystem(blks); };

	void setSaveInternalSignals(bool sSignals);
	bool const getSaveInternalSignals(void) { return saveInternalSignals; };

	void setLogValue(bool value) { superBlockSystem.setLogValue(value);	};
	void setOpenFile(bool value) { superBlockSystem.setOpenFile(value); };
	void setLogFileName(std::string newName) { superBlockSystem.setLogFileName(newName); };
	void setSignalsFolderName(std::string newName) { superBlockSystem.setSignalsFolderName(newName); };
	void setLoadedInputParameters(std::vector<std::string> loadedInputParams) { superBlockSystem.setLoadedInputParameters(loadedInputParams); };
private:
	std::ofstream logFileSP;
};

//########################################################################################################################################################
//###################################################################### LINEAR FILTERS ##################################################################
//########################################################################################################################################################


class FIR_Filter : public Block {

	/* State Variable */
	std::vector<t_real> delayLine;

	bool saveImpulseResponse{ true };
	std::string impulseResponseFilename{ "impulse_response.imp" };

	/* Input Parameters */
	bool seeBeginningOfImpulseResponse{ true };

public:

	/* State Variable */
	std::vector<t_real> impulseResponse;

	/* Input Parameters */
	int impulseResponseLength;							// filter order + 1 (filter order = number of delays)


	/* Methods */
	FIR_Filter(std::initializer_list<Signal *> InputSig, std::initializer_list<Signal *> OutputSig) :Block(InputSig, OutputSig) {};
	FIR_Filter(std::vector<Signal *> &InputSig, std::vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};
	FIR_Filter() {};

	void initializeFIR_Filter(void);

	bool runBlock(void);

	void terminate(void){};

	void setSaveImpulseResponse(bool sImpulseResponse) { saveImpulseResponse = sImpulseResponse; };
	bool getSaveImpulseResponse(void){ return saveImpulseResponse; };

	void setImpulseResponseLength(int iResponseLength) { impulseResponseLength = iResponseLength; };
	int const getImpulseResponseLength(){ return impulseResponseLength; }

	void setImpulseResponseFilename(std::string fName) { impulseResponseFilename = fName; };
	std::string getImpulseResponseFilename() { return impulseResponseFilename; };

	void setSeeBeginningOfImpulseResponse(bool sBeginning){ seeBeginningOfImpulseResponse = sBeginning; };
	bool const getSeeBeginningOfImpulseResponse(){ return seeBeginningOfImpulseResponse; };

};


class FD_Filter : public Block {
	
	/* State Variable */
	
	std::vector<t_real> inputBufferTimeDomain;
	std::vector<t_real> outputBufferTimeDomain;

	int inputBufferPointer{ 0 };
	int outputBufferPointer{ 0 };
	
	/* Input Parameters */
	bool saveTransferFunction{ true };
	std::string transferFunctionFilename{ "transfer_function.tfn" };
	int transferFunctionLength{ 128 };

	int inputBufferTimeDomainLength{ transferFunctionLength };
	int outputBufferTimeDomainLength{ transferFunctionLength };

public:
	/* State Variable */
	std::vector<t_complex> transferFunction;

	/* Methods */
	FD_Filter() {};
	FD_Filter(std::vector<Signal *> &InputSig, std::vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};
	FD_Filter(std::initializer_list<Signal *> InputSig, std::initializer_list<Signal *> OutputSig) :Block(InputSig, OutputSig) {};

	void initializeFD_Filter(void);

	bool runBlock(void);

	void terminate(void) {};

	void setInputBufferTimeDomainLength(int iBufferTimeDomainLength) { inputBufferTimeDomainLength = iBufferTimeDomainLength; };
	int const getInputBufferTimeDomainLength() { return inputBufferTimeDomainLength; }

	void setOutputBufferTimeDomainLength(int oBufferTimeDomainLength) { outputBufferTimeDomainLength = oBufferTimeDomainLength; };
	int const getOutputBufferTimeDomainLength() { return outputBufferTimeDomainLength; }

	void setInputBufferPointer(int iBufferPointer) { inputBufferPointer = iBufferPointer; };
	int const getInputBufferPointer() { return inputBufferPointer; }

	void setOutputBufferPointer(int oBufferPointer) { outputBufferPointer = oBufferPointer; };
	int const getOutputBufferPointer() { return outputBufferPointer; }

	void setSaveTransferFunction(bool sTransferFunction) { saveTransferFunction = sTransferFunction; };
	bool getSaveTransferFunction(void) { return saveTransferFunction; };

	void setTransferFunctionLength(int iTransferFunctionLength) { transferFunctionLength = iTransferFunctionLength; };
	int const getTransferFunctionLength() { return transferFunctionLength; };

};


// Generates a complex signal knowing the real part and the complex part.
class RealToComplex : public Block {
 public:
	 RealToComplex(std::vector<Signal *> &InputSig, std::vector<Signal *> &OutputSig);
	 bool runBlock(void);
 //private:
};

// Separates the complex signal into two parts: real and imaginary.
/*class ComplexToReal : public Block {
 public:
	 ComplexToReal(std::vector<Signal *> &InputSig, std::vector<Signal *> &OutputSig);
	 bool runBlock(void);
 //private:
};*/





//########################################################################################################################################################
//############################################################### GENERIC DSP FUNCTIONS ##################################################################
//########################################################################################################################################################


/*class OverlapMethod
{
public:
	void overlapSaveSymComplexIn(std::vector<std::complex <double>> &v_in, std::vector<std::complex <double>> &v_out, std::vector<std::complex <double>> Hf, int NFFT);
	void overlapSaveSyRealIn(std::vector<double> &v_in, std::vector<double> &v_out, std::vector<double> Hf, int NFFT);
	void overlapSaveAsym(std::vector<double> &real_in, std::vector<double> &imag_in, std::vector<double> &real_out, std::vector<double> &imag_out, std::vector<double> h_real, std::vector<double> h_imag, int M, int L, int N);
	void overlapSaveSym(std::vector<double> &real_in, std::vector<double> &imag_in, std::vector<double> &real_out, std::vector<double> &imag_out, std::vector<double> h_real, std::vector<double> h_imag, int NFFT);
	void checkSize(std::vector<double> &real_in, std::vector<double> &imag_in, int L);
};
class Fft
{
public:
	std::vector<std::complex <double>> directTransformInReal(std::vector<double> real);
	std::vector<double> inverseTransformInCP(std::vector<std::complex <double>> &In);
	
	void directTransform(std::vector<double> &real, std::vector<double> &imag);
	void inverseTransform(std::vector<double> &real, std::vector<double> &imag);
	void transformRadix2(std::vector<double> &real, std::vector<double> &imag);
	void transformBluestein(std::vector<double> &real, std::vector<double> &imag);
	void convolve(const std::vector<double> &x, const std::vector<double> &y, std::vector<double> &out);
	void convolve(const std::vector<double> &xreal, const std::vector<double> &ximag, const std::vector<double> &yreal, const std::vector<double> &yimag, std::vector<double> &outreal, std::vector<double> &outimag);
	
	void Radix2(std::vector<double> &real, std::vector<double> &imag, int m);
	void Bluestein(std::vector<double> &real, std::vector<double> &imag, int m);
};*/


class ComplexMult
{
public:
	void CMultVector(std::vector<double> &v1_real, std::vector<double> &v1_imag, std::vector<double> v2_real, std::vector<double> v2_imag);
	void CMultVector_Loop(std::vector<double> &v1_real, std::vector<double> &v1_imag, std::vector<double> v2_real, std::vector<double> v2_imag);
	std::vector<std::complex <double>> CMultVectorInCP(std::vector<std::complex <double>> &v1_in, std::vector<std::complex <double>> &v2_in);
	void ComplexVect2ReImVect(std::vector<std::complex <double>> &v_in, std::vector<double> &v1_real, std::vector<double> &v1_imag);
	void CMultVector_InComplex(std::vector<std::complex <double>> &v1_in, std::vector<std::complex <double>> &v2_in);
	void ReImVect2ComplexVect(std::vector<double> &v1_real, std::vector<double> &v1_imag, std::vector<std::complex <double>> &v_out);

	std::vector<std::complex<double>> ReImVect2ComplexVector(std::vector<double> &v1_real, std::vector<double> &v1_imag);
	std::vector<std::complex <double>> complexVectorMultiplication(std::vector<std::complex <double>> &v1_in, std::vector<std::complex <double>> &v2_in);
};


///////////////////// Fast Fourier Transform ////////////////////////

class FourierTransform {

public:

	std::vector<std::complex<double>> fft(std::vector<std::complex<double> > &vec, int sign);
	std::vector<std::complex<double>> fft(std::vector<std::complex<double> > &vec);
	std::vector<std::complex<double>> ifft(std::vector<std::complex<double> > &vec);
	void transformRadix2(std::vector<std::complex<double> > &vec);
	void transformBluestein(std::vector<std::complex<double> > &vec);
	void convolve(
		const std::vector<std::complex<double> > &vecx,
		const std::vector<std::complex<double> > &vecy,
		std::vector<std::complex<double> > &vecout);
};

///////////////////// System Parameters ////////////////////////

class SystemInputParameters {
private:
	std::vector<std::string> loadedInputParameters;
	std::string inputParametersFileName{ "input_parameters.txt" }; //name of the file from where the input parameters will be read
	std::string outputFolderName{ "signals" };
	enum ParameterType { INT, DOUBLE, BOOL, MATRIX, ORDERING, TRANSPORT, ROUTING_LOGICAL, ROUTING_PHYSICAL }; //types of parameters
											  //A parameter can only be of 1 type
	class Parameter {
	private:
		//Type of the parameter. this type should be hidden from the user
		ParameterType type;
		union { //pointers to the parameter
			int* i;
			double* d;
			bool* b;
			t_matrix* m;
			t_ordering_rule* o;
			t_transport_mode* t;
			t_routing_criterion_logical_topology* rl;
			t_routing_criterion_physical_topology* rp;
		};

	public:
		//Changes the value of the parameter
		void setValue(int value);
		void setValue(double value);
		void setValue(bool value);
		void setValue(t_matrix value);
		void setValue(t_ordering_rule value);
		void setValue(t_transport_mode value);
		void setValue(t_routing_criterion_logical_topology value);
		void setValue(t_routing_criterion_physical_topology value);
		ParameterType getType();
		//Constructor for parameter of type int
		Parameter(int* elem);
		//Constructor for parameter of type double
		Parameter(double* elem);
		//Constructor for parameter of type bool
		Parameter(bool* elem);
		//Constructor for parameter of type matrix
		Parameter(t_matrix* elem);
		//Constructor for parameter of type orderingrule
		Parameter(t_ordering_rule* elem);
		//Constructor for parameter of type transportmode
		Parameter(t_transport_mode* elem);
		//Constructor for parameter of type routingcriterionlogicaltopology
		Parameter(t_routing_criterion_logical_topology* elem);
		//Constructor for parameter of type routingcriterionphysicaltopology
		Parameter(t_routing_criterion_physical_topology* elem);
	};

	int parseInt(std::string str);
	double parseDouble(std::string str);
	bool parseBool(std::string str);
	//t_matrix parseMatrix(std::string str);
	t_matrix parseMatrix(std::ifstream &inputFile );
	t_ordering_rule parseOrderingRule(std::string str);
	t_transport_mode parseTransportMode(std::string str);
	t_routing_criterion_logical_topology parseRoutingCriterionLogicalTopology(std::string str);
	t_routing_criterion_physical_topology parseRoutingCriterionPhysicalTopology(std::string str);

	std::vector<std::string> split(const std::string & text, char sep);
	std::map<std::string, Parameter*> parameters = std::map<std::string, Parameter*>(); //Maps the names of the variables to the addresses of the parameters

public:
	std::string getInputParametersFileName() { return inputParametersFileName; }
	std::string getOutputFolderName() { return outputFolderName; }
	std::vector<std::string> getLoadedInputParameters() { return loadedInputParameters; }
	void readSystemInputParameters();
	//Adds the parameter to the std::map
	void addInputParameter(std::string name, int* variable);
	void addInputParameter(std::string name, double* variable);
	void addInputParameter(std::string name, bool* variable);
	void addInputParameter(std::string name, t_matrix* variable);
	void addInputParameter(std::string name, t_ordering_rule* variable);
	void addInputParameter(std::string name, t_transport_mode* variable);
	void addInputParameter(std::string name, t_routing_criterion_logical_topology* variable);
	void addInputParameter(std::string name, t_routing_criterion_physical_topology* variable);
	/* Default empty constructor. Initializes the std::map */
	SystemInputParameters() {};
	SystemInputParameters(int argc,char*argv[]);
	SystemInputParameters(std::string fName);
	/* Deletes all heap memory occupied by the parameters */
	~SystemInputParameters();
};

//########################################################################################################################################################
//############################################################### RANDOM NUMBER GENERATORS ##################################################################
//########################################################################################################################################################


// Generates integer numbers uniformely distribuited in the interval [lo,hi)
template<typename T = random_engine >
class UniformRandomIntegerNumbers {
	UniformRandomIntegerNumbers(int low, int high) : r(bind(std::uniform_int_distribution<>(low, high), T())) { };
	UniformRandomIntegerNumbers(long int seed, int low, int high) : r(bind(std::uniform_int_distribution<>(low, high), T(seed))) { };
	int operator()() const { return r(); }
private:
	std::function<int()> r;
};

// Generates real numbers (double) uniformely distribuited in the interval [lo,hi)
template<typename T= random_engine >
class UniformRandomRealNumbers {
public:
	UniformRandomRealNumbers(double low, double high) :r(bind(std::uniform_real_distribution<>(low, high), T())) { };
	UniformRandomRealNumbers(long int seed, double low, double high) :r(bind(std::uniform_real_distribution<>(low, high), T(seed))) { };
	double operator()() { return r(); };
private:
	std::function<double()> r;
};


# endif // PROGRAM_INCLUDE_netxpto_H_