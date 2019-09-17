# ifndef DIJKSTRA_ELIO
# define DIJKSTRA_ELIO


# include "..\include\netxpto_20190130.h"

using namespace std;

class DijkstraAlgorithm : public Block {
	void dijkstraComputePaths(vertex_t source, const adjacency_list_t& adjacency_list, std::vector<weight_t>& min_distance, std::vector<vertex_t>& previous);
	std::list<vertex_t> dijkstraGetShortestPathTo(vertex_t vertex, const std::vector<vertex_t>& previous);
};

#endif
