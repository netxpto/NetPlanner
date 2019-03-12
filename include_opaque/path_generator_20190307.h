# ifndef PATH_GENERATOR_H_
# define PATH_GENERATOR_H_

# include <vector>
# include <complex>
# include <fstream>
# include <iostream>
# include <math.h>
# include <stdio.h>
# include <string>
# include <strstream>
# include <algorithm> 
# include <random>
# include <chrono>

# include "netxpto_20190130.h"


class PathGenerator : public Block {

	//Input Parameters
	std::string shortestPathType;
	t_integer numberOfPaths{ 0 };
	
	bool generate{ true };
	t_integer minDistance(t_integer dist[], bool prev[]);
	t_integer dijkstra(t_matrix logicalTopology, t_integer sourceNode);
	//YenKSP

public:
	// Methods (Constructors)
	PathGenerator() {};
	PathGenerator(initializer_list<Signal *> inputSig, initializer_list<Signal *> outputSig) :Block(inputSig, outputSig) {};

	void initialize(void);
	bool runBlock(void);

	void setShortestPathType(std::string sPathType) { shortestPathType = sPathType; }
	std::string getShortestPathType(void) { return shortestPathType; };

	void setNumberOfPaths(t_integer nPaths) { numberOfPaths = nPaths; }
	t_integer getNumberOfPaths(void) { return numberOfPaths; };

};

# endif