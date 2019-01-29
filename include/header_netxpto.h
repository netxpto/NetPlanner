# ifndef HEADER_H_
# define HEADER_H_

typedef struct { t_integer matrix[NUMBER_OF_NODES][NUMBER_OF_NODES]; } t_matrix;

class Demand : public Signal {

public:
	Demand() : Signal() { Signal::setBuffer(); }
	Demand(string fileName) : Signal(fileName) { setBuffer(); }
	Demand(string fileName, t_unsigned_long bLength) : Signal(fileName, bLength) { setBuffer(); }
	Demand(string fileName, string folderName) : Signal(fileName, folderName) { setBuffer(); }
	Demand(string fileName, string folderName, t_unsigned_long bLength) : Signal(fileName, folderName, bLength) { setBuffer(); }
	Demand(t_unsigned_long bLength) : Signal(bLength) { setBuffer(); }

	void setDemandIndex(t_integer dIndex) { demandIndex = dIndex; }
	t_integer getDemandIndex() { return demandIndex; }



private:
	t_integer demandIndex{ 0 };
	t_integer sourceNode{ 0 };
	t_integer destinationNode{ 0 };
	t_integer oduType{ 0 };
	t_integer restorationMethod{ 0 };
};





# endif
