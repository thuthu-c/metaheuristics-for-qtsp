#include <vector>
#include <algorithm>
#include <random>
#include <climits>
#include "evaluate.cpp"
#include "greedy_permuting.cpp"
#include "../../include/helpers/random.h"
#include "../../include/data_structures/graph.h"

std::vector<int> getNearestNeighbors(int cityIndex, Graph &graph, int neighborNumber)
{
    int n = graph.getMaxM();
    std::vector<int> result;
    std::vector<std::vector<int>> bestResults;

    std::vector<int> vertexes;
    for (int i = 0; i < n; i++)
    {
        vertexes.push_back(i);
    }

    for (int j = 0; j < n; j++)
    {
        if (cityIndex != j)
        {
            std::vector<int> tour;
            int v1 = cityIndex;
            int v2 = j;
            tour.push_back(cityIndex);
            tour.push_back(j);

            // remaining vertexes
            std::vector<int> vertexToCheck;

            std::copy_if(
                vertexes.begin(),
                vertexes.end(),
                std::back_inserter(vertexToCheck),
                [&](int needle)
                {
                    return std::find(tour.begin(), tour.end(), needle) == tour.end();
                });

            std::pair<int, int> nearestNeighbor = std::make_pair(INT_MAX, -1);
            for (auto v3 : vertexToCheck)
            {
                if (v3 != cityIndex)
                {
                    nearestNeighbor = std::min(nearestNeighbor, std::make_pair(graph.custo[v1][v2][v3], v3));
                    v1 = v2;
                    v2 = v3;
                    if (nearestNeighbor.second != cityIndex)
                    {
                        tour.push_back(nearestNeighbor.second);
                    }
                }
            }

            if ((int)bestResults.size() < neighborNumber)
            {
                if (cityIndex != tour[1])
                {
                    bestResults.push_back(tour);
                    result.push_back(tour[1]);
                }
            }
            else
            {
                // calcular onde esse melhor resultado vai ser inserido
                int worstLocalResultIndex = -1;
                int worstLocalResultValue = -1;
                std::vector<int> worstLocalResult;
                for (int k = 0; k < neighborNumber; k++)
                {
                    if (k != cityIndex)
                    {
                        if (worstLocalResult.size() == 0)
                        {
                            worstLocalResultIndex = k;
                            worstLocalResult = bestResults[k];
                        }
                        else
                        {
                            int val = eval(worstLocalResult, graph);
                            if (eval(bestResults[k], graph) < val)
                            {
                                worstLocalResult = bestResults[k];
                                worstLocalResultIndex = k;
                                worstLocalResultValue = val;
                            }
                        }
                    }
                }

                if (eval(tour, graph) < worstLocalResultValue)
                {
                    if (result[worstLocalResultIndex] != cityIndex)
                    {
                        result[worstLocalResultIndex] = j;
                        bestResults[worstLocalResultIndex] = tour;
                    }
                }
            }
        }
    }

    return result;
}

std::vector<std::vector<int>> initializeNearestNeighbors(int populationSize, Graph &graph)
{
    int cityIndex = 0;
    int cityNumber = graph.getMaxM();
    std::vector<std::vector<int>> population;

    std::vector<int> vertexes;
    for (int i = 0; i < graph.getMaxM(); i++)
    {
        vertexes.push_back(i);
    }

    // neighborNumber it's fixed to five according to the paper:
    // Greedy Permuting Method for Genetic Algorithm on TSP
    const int neighborNumber = 5;

    while (cityIndex < cityNumber)
    {
        std::vector<int> neighbors = getNearestNeighbors(
            cityIndex,
            graph,
            neighborNumber);

        for (int neighbor : neighbors)
        {
            std::vector<int> individual = {cityIndex, neighbor};
            std::vector<int> permutedCities = greedyPermuting(individual, graph, cityNumber);
            individual.insert(individual.end(), permutedCities.begin(), permutedCities.end());
            population.push_back(individual);
        }

        cityIndex += 1;
    }

    // load remaining population randomly
    std::uniform_int_distribution<int> distribution(0, (int)graph.getMaxM());

    while ((int)population.size() < populationSize)
    {
        std::vector<int> currentPopulation;

        std::vector<int> vertexesToBeRandomInserted;
        for (int j = 0; j < (int)graph.getMaxM(); j++)
        {
            vertexesToBeRandomInserted.push_back(j);
        }

        for (int k = 0; k < (int)graph.getMaxM(); k++)
        {
            int randIndex = distribution(engine) % vertexesToBeRandomInserted.size();
            currentPopulation.push_back(vertexesToBeRandomInserted[randIndex]);

            // https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom
            vertexesToBeRandomInserted.erase(
                std::remove(
                    vertexesToBeRandomInserted.begin(),
                    vertexesToBeRandomInserted.end(),
                    vertexesToBeRandomInserted[randIndex]),
                vertexesToBeRandomInserted.end());
        }

        population.push_back(currentPopulation);
    }

    // Trim the population to the desired size
    if (population.size() > (unsigned long long)populationSize) {
        population.erase(population.begin() + populationSize, population.end());
    }

    return population;
}