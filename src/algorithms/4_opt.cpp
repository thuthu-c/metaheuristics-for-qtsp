#include "../../include/helpers/random.h"
#include <vector>
#include <algorithm>
#include <climits>
#include <iostream>
#include <string>
#include <unordered_set>
#include "../../include/algorithms/4_opt.h"


int FourOpt::getNumEvaluations(){
    return this->numEvaluations;
}

FourOpt::FourOpt (Graph &graph){
    this->graph = &graph; 
}
FourOpt::FourOpt (){}

int FourOpt::eval(std::vector<int> &tour, Graph &graph)
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


std::vector<int> generateRandomTour(Graph &graph){
    
    std::vector<int> solution((int)graph.getMaxM());

    std::uniform_int_distribution<int> distribution(0, (int)graph.getMaxM()); 

    std::vector<int> vertexesToBeRandomInserted;
        for (int j = 0; j < (int)graph.getMaxM(); j++)
        {
            vertexesToBeRandomInserted.push_back(j);
        }


        for (int k = 0; k < (int)graph.getMaxM(); k++)
        {
            int randIndex = distribution(engine) % vertexesToBeRandomInserted.size();
            solution[k] = vertexesToBeRandomInserted[randIndex];

            vertexesToBeRandomInserted.erase(
                std::remove(
                    vertexesToBeRandomInserted.begin(),
                    vertexesToBeRandomInserted.end(),
                    vertexesToBeRandomInserted[randIndex]),
                vertexesToBeRandomInserted.end());
        }

        return solution; 
}

std::vector<int> FourOpt::run(Graph &graphInput){
    graph = &graphInput;
    std::vector<int> initial_solution = generateRandomTour(*graph);

    return best4opt(initial_solution);
}


inline int get_safe(const std::vector<int> &tour, int i)
{
    int n = tour.size();
    return tour[((i % n) + n) % n];
}

long long int FourOpt::D2O(int i, int j, std::vector<int> tour )
{

    int vi_minus_1 = get_safe(tour, i - 1);
    int vi = get_safe(tour, i);
    int vi_plus_1 = get_safe(tour, i + 1);
    int vi_plus_2 = get_safe(tour, i + 2); // Necessário para a aresta (i, i+1, i+2)

    int vj_minus_1 = get_safe(tour, j - 1);
    int vj = get_safe(tour, j);
    int vj_plus_1 = get_safe(tour, j + 1);
    int vj_plus_2 = get_safe(tour, j + 2);
 
    long long int removed =
        graph->custo[vi_minus_1][vi][vi_plus_1] + graph->custo[vi][vi_plus_1][vi_plus_2] + graph->custo[vj_minus_1][vj][vj_plus_1] + graph->custo[vj][vj_plus_1][vj_plus_2];

    long long int added =
        graph->custo[vi_minus_1][vi][vj_plus_1] + graph->custo[vi][vj_plus_1][vj_plus_2] + graph->custo[vj][vi_plus_1][vi_plus_2] + graph->custo[vj_minus_1][vj][vi_plus_1];

    return added - removed;
}

std::vector<int> fourOptMove(int i1, int i2, int j1, int j2, std::vector<int> tour)
{
    // dividir a tour em quatro partes
    std::vector<int> parte_um;
    std::vector<int> parte_dois;
    std::vector<int> parte_tres;
    std::vector<int> parte_quatro;

    int n = static_cast<int>(tour.size());
    if (n == 0)
        return {};

    auto append_range = [&](int start, int end, std::vector<int> &dest)
    {
        int idx = start;
        while (true)
        {
            dest.push_back(tour[idx]);
            if (idx == end)
                break;
            idx = (idx + 1) % n;
        }
    };

    // part1: from j2+1 to i1 (wrapping)
    int s1 = (j2 + 1) % n;
    int e1 = i1 % n;
    append_range(s1, e1, parte_um);

    // part2: from i1+1 to i2
    int s2 = (i1 + 1) % n;
    int e2 = i2 % n;
    append_range(s2, e2, parte_dois);

    // part3: from i2+1 to j1
    int s3 = (i2 + 1) % n;
    int e3 = j1 % n;
    append_range(s3, e3, parte_tres);

    // part4: from j1+1 to j2
    int s4 = (j1 + 1) % n;
    int e4 = j2 % n;
    append_range(s4, e4, parte_quatro);

    // montando a nova tour (example reconnection: 1 + 4 + 3 + 2)
    std::vector<int> solution;
    solution = parte_um;
    solution.insert(solution.end(), parte_quatro.begin(), parte_quatro.end());
    solution.insert(solution.end(), parte_tres.begin(), parte_tres.end());
    solution.insert(solution.end(), parte_dois.begin(), parte_dois.end());

    return solution;
}

std::vector<int> FourOpt::best4opt(std::vector<int> solution)
{
    int n = solution.size();
    if (n < 8)
        return solution;

    // Tabelas da DP
    // F[i2][j1] armazena o melhor custo parcial
    // Parent[i2][j1] armazena qual i1 gerou esse custo (para reconstrução)
    std::vector<std::vector<double>> F(n, std::vector<double>(n, 1e18));
    std::vector<std::vector<int>> parent_i1(n, std::vector<int>(n, -1));

    // Inicialização (Algoritmo 2 linha 3) - Corrigido índice 7 para 2 (base 0) que equivale ao 3 do artigo
    for (int j1 = 4; j1 <= n - 4; ++j1)
    {
        F[2][j1] = D2O(0, j1, solution); // Assume i1 = 0
        parent_i1[2][j1] = 0;
    }


    // Recursão (Algoritmo 2 linhas 4-6)
    for (int i2 = 3; i2 <= n - 5; ++i2)
    {
        for (int j1 = i2 + 2; j1 <= n - 3; ++j1)
        {
            double val_stay = F[i2 - 1][j1];
            double val_new = D2O(i2 - 1, j1, solution); // Novo corte

            // Simplificação da recorrência: escolhe o melhor entre estender ou novo corte
            // Nota: A lógica exata da Eq 13 pode variar, mas a ideia é minimizar custo
            if (val_new < val_stay)
            {
                F[i2][j1] = val_new;
                parent_i1[i2][j1] = i2 - 1;
            }
            else
            {
                F[i2][j1] = val_stay;
                parent_i1[i2][j1] = parent_i1[i2 - 1][j1];
            }
        }
    }


    // Parte 2: Encontrar o melhor Delta* (combinação dos cortes)
    double best_improvement = -0.000001; // Só aceita se melhorar (negativo)
    int best_i1 = -1, best_i2 = -1, best_j1 = -1, best_j2 = -1;

    for (int i2 = 3; i2 <= n - 5; ++i2)
    {
        for (int j2 = i2 + 5; j2 <= n - 1; ++j2)
        {
            // Varredura simplificada para achar o melhor j1 compatível
            for (int j1 = i2 + 2; j1 <= j2 - 2; ++j1)
            {
                double current_delta = D2O(i2, j2, solution) + F[i2][j1];
                numEvaluations++;

                if (current_delta < best_improvement)
                {
                    best_improvement = current_delta;
                    best_i2 = i2;
                    best_j2 = j2;
                    best_j1 = j1;
                    best_i1 = parent_i1[i2][j1];
                }
            }
        }
    }

    if (best_i2 != -1)
    {
        
        return fourOptMove(best_i1, best_i2, best_j1, best_j2, solution);
    }

    return solution; 
}