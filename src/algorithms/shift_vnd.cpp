#include "../../include/algorithms/shift_vnd.h"
#include <vector>

   

    int evalLocal(const std::vector<int>& tour, Graph& graph) {
        int tourVal = 0;
        int n = tour.size();
        for (int j = 0; j < n; j++) {
            tourVal += graph.custo[tour[j]][tour[(j + 1) % n]][tour[(j + 2) % n]];
        }
        return tourVal;
    }

    int ShiftVND::getNumEvaluations(){
        return this->numEvaluations;
    }
    
    std::vector<int> ShiftVND::run(std::vector<int> tour, Graph& graph, int maxEv, int& currentCost) {
        int n = tour.size();
        bool improved = true;
        
        while (improved && numEvaluations < maxEv) {
            improved = false;
            
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (i == j || j == (i + 1) % n || j == (i + 2) % n) continue; 
                    if (numEvaluations >= maxEv) return tour;
                    
                    std::vector<int> tmpTour = tour;
                    int v = tmpTour[i];
                    
                    tmpTour.erase(tmpTour.begin() + i);
                    
                    int insert_pos = (i < j) ? j - 1 : j;
                    tmpTour.insert(tmpTour.begin() + insert_pos, v);
                    
                    int newCost = evalLocal(tmpTour, graph);
                    int delta = newCost - currentCost;
                    
                    numEvaluations++;
                    
                    if (delta < 0) {
                        tour = std::move(tmpTour);
                        currentCost = newCost;
                        improved = true;
                    }
                }
            }
        }
        return tour;
    }