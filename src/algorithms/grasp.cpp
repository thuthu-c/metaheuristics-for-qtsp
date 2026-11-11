#include "../../include/algorithms/grasp.h"
#include "../../include/algorithms/rcl.h"
#include "../../include/algorithms/4_opt_util.h"
#include <climits>
#include <algorithm>
#include <iostream>


int GRASP::eval(std::vector<int> &tour, Graph &graph)
{
    int tourVal = 0;
    int n = tour.size();

    tourVal += graph.custo[tour[n - 2]][tour[n - 1]][tour[0]];
    tourVal += graph.custo[tour[n - 1]][tour[0]][tour[1]];

    for (int j = 0; j < (n - 2); j++)
    {
        tourVal += graph.custo[tour[j]][tour[j + 1]][tour[j + 2]];
    }

    numEvaluations++;

    return tourVal;
}

std::pair<int, int> getInitialEdge(Graph &graph)
{
    int n = graph.getMaxM();

    // choosing the v1 and v2
    int v1 = -1;
    int v2 = -1;

    int bestCost = INT_MAX;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i != j)
            {
                int acumulator = 0;
                for (int x = 0; x < n; x++)
                {
                    acumulator += graph.custo[x][i][j];
                }

                int localBestCost = INT_MAX;
                for (int x = 0; x < n; x++)
                {
                    if (graph.custo[i][j][x] < localBestCost)
                    {
                        localBestCost = graph.custo[i][j][x];
                    }
                }

                acumulator += localBestCost;

                if (acumulator < bestCost)
                {
                    bestCost = acumulator;
                    v1 = i;
                    v2 = j;
                }
            }
        }
    }

    return std::make_pair(v1, v2);
}

std::vector<int> nearestNeighborhoods(Graph &graph){
    std::vector<int> vertexes;
    int n = graph.getMaxM();
    for (int i = 0; i < n; i++)
    {
        vertexes.push_back(i);
    }

    auto initial = getInitialEdge(graph);
    int v1, v2;
    v1 = initial.first;
    v2 = initial.second;

    std::vector<int> tour;
    tour.push_back(v1);
    tour.push_back(v2);

    while ((int) tour.size() < n)
    {
        // remaining vertexes
        std::vector<int> vertexToCheck;

        std::copy_if(
            vertexes.begin(),
            vertexes.end(),
            std::back_inserter(vertexToCheck),
            [&](int needle)
            {
                return std::find(tour.begin(), tour.end(), needle) == tour.end();
            }
        );

        std::pair<int, int> bestNeighbor(INT_MAX, -1);
        for (auto x : vertexToCheck)
        {
            bestNeighbor = std::min(std::make_pair(graph.custo[v1][v2][x], x), bestNeighbor);
        }

        v1 = v2;
        v2 = bestNeighbor.second;

        tour.push_back(bestNeighbor.second);
    }

    return tour;
}



std::vector<int> GRASP::run(Graph &graph){
    numEvaluations = 0; 
    std::vector<int> best_solution; 
    int best_cost{INT_MAX}; 

    int tam_sol = graph.getNumVertex(); 
    
    RCL rcl;
    FourOptUtil fourOpt(graph); 

    
    while(numEvaluations < maxEvaluations){
        std::vector<int> solution_curr = rcl.runCheapestInsertion(graph, 0.15);
        numEvaluations += rcl.getNumEvaluations();
        
        int current_cost = eval(solution_curr, graph);
        bool flag{true};

        while(flag && numEvaluations < maxEvaluations){
            std::vector<int> nn_solution = fourOpt.run(solution_curr);
            numEvaluations += fourOpt.getNumEvaluations();
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