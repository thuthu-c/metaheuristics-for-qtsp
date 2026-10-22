#ifndef _RCL_H_
#define _RCL_H_

#include "../algorithms/tsp_solver.h"
#include "../data_structures/graph.h"
class RCL : public TspSolver
{
    std::tuple<int, int, int> getInitialSubtour(Graph &graph);
    int alpha;
    int maxEvaluations{10000}; 
    int numEvaluations{0}; 
    Graph *graph;

public:
    int eval(std::vector<int> &tour, Graph &graph);
    std::vector<int> run(Graph &graph);
    std::vector<int> run_two(Graph &graph);
    int getNumEvaluations();
    long long int calculateInsertionCost(const std::vector<int> &tour, int pos, int vertex);
    

};


#endif