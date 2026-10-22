#ifndef _FOUR_OPT_H_
#define _FOUR_OPT_H_

#include "../algorithms/tsp_solver.h"
#include "../data_structures/graph.h"
class FourOpt : public TspSolver
{
    std::tuple<int, int, int> getInitialSubtour(Graph &graph);
    int alpha;
    int maxEvaluations{100000}; 
    int numEvaluations{0}; 
    Graph *graph;
public:
    FourOpt();
    FourOpt(Graph &graph);
    int eval(std::vector<int> &tour, Graph &graph);
    std::vector<int> run(Graph &graph);
    long long int D2O(int i, int j, std::vector<int> tour);
    std::vector<int> best4opt(std::vector<int> solution);
    int getNumEvaluations();
    
    

};


#endif