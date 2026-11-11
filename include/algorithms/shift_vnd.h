#ifndef _SHIFT_VND_H_
#define _SHIFT_VND_H_

#include "../algorithms/tsp_solver.h"
#include "../data_structures/graph.h"

class ShiftVND{

Graph *graph;
int numEvaluations = 0;

public: 
std::vector<int> run(std::vector<int> tour, Graph& graph, int maxEv, int& currentCost);
int getNumEvaluations();

};

#endif