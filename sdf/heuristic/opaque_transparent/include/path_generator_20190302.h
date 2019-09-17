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
# include <vector>
# include <algorithm> 
# include <random>
# include <chrono>


# include "..\include\netxpto_20190130.h"


using namespace std;



class PathGenerator : public Block {

	// State variables 
	t_logical_topology currentLogicalTopology; // Saves logical topology before exiting this block
	

	// Input Parameters (default values)
	criterion Criterion{ criterion::hops }; 
	t_integer numberOfPaths{ 3 };



public:

	// Methods (Constructors)
	PathGenerator() {};
	PathGenerator(initializer_list<Signal *> inputSig, initializer_list<Signal *> outputSig) :Block(inputSig, outputSig) {};

	void initialize(void);
	bool runBlock(void);

	void setCriterion(criterion crt) { Criterion = crt; }
	criterion getCriterion(void) { return Criterion; };

	void setNumberOfPaths(t_integer numb) { numberOfPaths = numb; }
	t_integer getNumberOfPaths(void) { return numberOfPaths; };

private:
	long int processedSamples{ 0 };
};

# endif
