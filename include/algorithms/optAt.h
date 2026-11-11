#ifndef _OPTAT_H
#define _OPTAT_H 

#include "../algorithms/tsp_solver.h"
#include "../data_structures/graph.h"
#include "./utils/individual.cpp" 
#include <random>


class OptAt : public TspSolver {
    Graph *graph;
    int numEvaluation;
    int populationSize;
    double plasmidSize;
    int num_evaluations;
    int numBestIndividuals;

    int gir_size;    

    
    

    std::mt19937 g;

    std::vector<int> run(Graph& graphInput);

    std::vector<std::vector<int>> gir; 
    std::vector<Individual> population;

    void generate_gir(Graph *graph);

    std::vector<int> getNearestNeighbors(int cityIndex, Graph &graph, int neighborNumber);
    std::vector<std::vector<int>> initializeNearestNeighbors(int populationSize, Graph &graph);
    int eval(std::vector<int> &tour, Graph &graph);
    std::vector<int> greedyPermuting(std::vector<int> &seedIndividual, Graph &graph, int n);
    std::vector<std::vector<int>> initializeRandomPopulation(int populationSize, Graph &graph);


    void generate_population_random(Graph *graph);
    std::vector<std::vector<int>> generate_population(Graph *graph);

    std::vector<int> getVertex();


    
    std::vector<int> m1 (const std::vector<int> &p, int tamanho_solucao, const std::vector<int> &solution);
    std::vector<int> generate_plasmid(std::vector<std::vector<int>>  &gir, double plasmidSize);
    std::vector<int> best4opt(const std::vector<int>& cromossome);
    long long int D2O(int i, int j, const std::vector<int>& tour);
    std::vector<int> fourOptMove(int i1, int i2, int j1, int j2, const std::vector<int>& tour);
    virtual std::vector<int> transposon_4OPT(const std::vector<int>& individual);

};
#endif