#include <vector>
#include <algorithm>
#include "../../include/data_structures/graph.h"

class SwapVND {
public:
    int numEvaluations = 0;

    std::vector<int> run(std::vector<int> tour, Graph& graph, int maxEv, int& currentCost) {
        int n = tour.size();
        bool improved = true;
        
        while (improved && numEvaluations < maxEv) {
            improved = false;
            
            for (int i = 0; i < n - 1; i++) {
                for (int j = i + 1; j < n; j++) {
                    if (numEvaluations >= maxEv) return tour;
                    
                    // 1. Coleta os índices iniciais das trincas afetadas na rota original
                    std::vector<int> idxs = {
                        (i - 2 + n) % n, (i - 1 + n) % n, i,
                        (j - 2 + n) % n, (j - 1 + n) % n, j
                    };
                    
                    // 2. Remove duplicatas para evitar contagem dupla se 'i' e 'j' forem vizinhos próximos
                    std::sort(idxs.begin(), idxs.end());
                    idxs.erase(std::unique(idxs.begin(), idxs.end()), idxs.end());
                    
                    int removedCost = 0;
                    for (int k : idxs) {
                        removedCost += graph.custo[tour[k]][tour[(k + 1) % n]][tour[(k + 2) % n]];
                    }
                    
                    // 3. Lambda que "simula" a rota após a troca, sem precisar modificar o vetor de verdade
                    auto get_node = [&](int idx) {
                        if (idx == i) return tour[j];
                        if (idx == j) return tour[i];
                        return tour[idx];
                    };
                    
                    int addedCost = 0;
                    for (int k : idxs) {
                        addedCost += graph.custo[get_node(k)][get_node((k + 1) % n)][get_node((k + 2) % n)];
                    }
                    
                    int delta = addedCost - removedCost;
                    numEvaluations++; // Custo O(1) registrado
                    
                    // 4. Se a troca melhorar a rota, efetuamos a mudança e reiniciamos a busca
                    if (delta < 0) {
                        std::swap(tour[i], tour[j]);
                        currentCost += delta;
                        improved = true;
                    }
                }
            }
        }
        return tour;
    }
};