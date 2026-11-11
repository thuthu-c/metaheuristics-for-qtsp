#include <vector> 
#include "individual.cpp"
#include "random_population.cpp"
#include "nearest_neighbors.cpp"
#include "evaluate.cpp"
#include "calculate_diversity.cpp"
#include "random"
#include "../../include/data_structures/graph.h"

class Population {
    public: 


    std::vector<Individual> generate_population(Graph *graph, int populationSize){
    int n = graph->getNumVertex();
    std::vector<Individual> population(populationSize);
    int randomPopulationSize = (int)(0.5 * populationSize);

    std::vector<std::vector<int>> randomPopulation, nearestPopulation;
    randomPopulation = initializeRandomPopulation(randomPopulationSize, *graph);

    int nearestPopulationSize = populationSize - randomPopulationSize;
    nearestPopulation = initializeNearestNeighbors(nearestPopulationSize, *graph);

    for (int i = 0; i < randomPopulationSize; i++) {
        population[i].tour = randomPopulation[i];
        population[i].cost = eval(randomPopulation[i], *graph);
        population[i].size = population[i].tour.size() == n ? n : population[i].tour.size(); 
    }

    for (int i = 0; i < nearestPopulationSize; i++) {
        population[i + randomPopulationSize].tour = nearestPopulation[i];
        population[i + randomPopulationSize].cost = eval(nearestPopulation[i], *graph);
        population[i + randomPopulationSize].size = population[i+ randomPopulationSize].tour.size() == n ? n : population[i+ randomPopulationSize].tour.size(); 
    }



    return population;
}

void evaluatePopulation(std::vector<Individual>& population, double uElite) {
    int popSize = population.size();
    for (auto& i : population) {
        i.biasedFitness = i.costRank + (1.0 - (uElite / popSize)) * i.diversityRank;
    }
}

void individualCostRank(std::vector<Individual>& population){
    std::vector<Individual*> copyPopulation;
    for (auto& i : population) copyPopulation.push_back(&i);

    std::sort(copyPopulation.begin(), copyPopulation.end(),
              [](const Individual* a, const Individual* b) {
                  return a->cost < b->cost; 
              });

    int cont = 0;
    for (auto i : copyPopulation) {
        i->costRank = ++cont;
    }
}

void individualDiversityRank(std::vector<Individual>& population) {
    

    for (auto& ind : population) {
        ind.diversityContribution = dc(ind, population, ind.size, 2);
    }

    std::vector<Individual*> copyPopulation;
    for (auto& i : population) copyPopulation.push_back(&i); 

    std::sort(copyPopulation.begin(), copyPopulation.end(),
              [](const Individual* a, const Individual* b) {
                  return a->diversityContribution > b->diversityContribution; 
              });

    int cont = 0;
    for (auto i : copyPopulation) {
        i->diversityRank = ++cont;
    }
}
};