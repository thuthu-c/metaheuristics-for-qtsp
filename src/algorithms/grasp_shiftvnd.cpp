#include "../../include/algorithms/grasp_shiftvnd.h"
#include "../../include/algorithms/shift_vnd.h"
#include "../../include/algorithms/rcl.h"
#include "../../include/algorithms/4_opt_util.h"
#include <climits>
#include <algorithm>
#include <iostream>

std::vector<int> GRASPSHIFTVND::run(Graph &graph){

    int numEvaluations = 0; 
    int maxEvaluations = 5000000;
    std::vector<int> best_solution; 
    int best_cost{INT_MAX}; 

    int tam_sol = graph.getNumVertex(); 
    
    RCL rcl;
    ShiftVND shiftVnd;

    
    while(numEvaluations < maxEvaluations){
        std::vector<int> solution_curr = rcl.runCheapestInsertion(graph, 0.15);
        numEvaluations += rcl.getNumEvaluations();
        
        int current_cost = eval(solution_curr, graph);
        bool flag{true};

        while(flag && numEvaluations < maxEvaluations){
            std::vector<int> nn_solution = shiftVnd.run(solution_curr, graph, maxEvaluations - numEvaluations, current_cost);
            numEvaluations += shiftVnd.getNumEvaluations();
            int nn_cost = eval(nn_solution, graph);
            if(current_cost > nn_cost){
                solution_curr = nn_solution;
                current_cost = nn_cost;
                flag = true;
            }else flag = false;
        }
    
        if(current_cost < best_cost){
            best_solution = solution_curr; 
            best_cost = current_cost;

        }
    }

    return best_solution;
}