# include "..\include\logical_top_Gen_20190216.h"

//using namespace std;

void Logical_Topology_Generator::initialize(void) {

	numberOfOutputSignals = (int)outputSignals.size();
	std::string transportMode = getTransportMode();

}

bool Logical_Topology_Generator::runBlock(void) {
	
	t_logical_topolgy output;
	t_matrix outputTopology;

	if (transportMode == "opaque" || transportMode == "OPAQUE")
	{
		outputTopology == physicalTopology;
		output.logicalConnections = outputTopology;

		outputSignals[0]->bufferPut((t_logical_topolgy)output);
	}	
	else if (transportMode == "transparent" || transportMode == "TRANSPARENT")  // Considering all nodes transparent
	{
		for (t_integer line = 0; line < output.logicalConnections[0].size(); line++)
		{
			for (t_integer column = 0; column < output.logicalConnections[0].size(); column++)
			{
				if (line == column) 
				{ 
					output.logicalConnections[line][column] = 0; 
				}
				else
				{
					output.logicalConnections[line][column] = 1;
				}
			}
		}

		outputSignals[0]->bufferPut((t_logical_topolgy)output);
	}
	else
	{
		cout << "Error: logical_Top_Gen_20190216.h - Transport Mode not defined\n";
	}

	outputSignals[0]->bufferPut((t_logical_topolgy)output);

	return true;
};