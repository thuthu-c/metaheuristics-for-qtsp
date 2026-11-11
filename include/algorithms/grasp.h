#ifndef _GRASP_H_
#define _GRASP_H_

#include "../algorithms/tsp_solver.h"
#include "../data_structures/graph.h"
class GRASP : public TspSolver
{
    std::tuple<int, int, int> getInitialSubtour(Graph &graph);
    int alpha;
    int maxEvaluations{5000000}; 
    int numEvaluations{0}; 
public:
    int eval(std::vector<int> &tour, Graph &graph);
    std::vector<int> run(Graph &graph);
    

};


#endif