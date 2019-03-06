# include "..\include\path_generator_20190302.h"


using namespace std;

//	Global variables needed to generate a demand signal


void PathGenerator::initialize(void) {

	numberOfOutputSignals = (int)outputSignals.size();
	numberOfOutputSignals = (int)inputSignals.size();

}

bool PathGenerator::runBlock(void) {

	t_integer demandsReady = inputSignals[0]->ready(); // number of demands in the buffer ready to be processed

	if (demandsReady > 0) 
	{
		for (t_integer i = 0; i < demandsReady; i++)
		{
			inputSignals[0]->bufferGet();
			
		}
	}

	//else if (demandsReady == 0) return false;

	/*for (int i = 0; i < process; i++) {
		inputSignals[0]->bufferGet();
		++processedSamples;
	}

	if (numberOfSamples >= 0) numberOfSamples -= process;

	if (displayNumberOfSamples) {
		cout << "Samples to be processed now: " << process << "\n";
		cout << "Samples to be processed later: " << numberOfSamples << "\n";
		cout << "Samples already processed: " << processedSamples << "\n";
	}
	*/
	return true;
};

