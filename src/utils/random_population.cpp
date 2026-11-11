#include <vector>
#include <algorithm>
#include <random>
#include "../../include/helpers/random.h"
#include "../../include/data_structures/graph.h"

std::vector<std::vector<int>> initializeRandomPopulation(int populationSize, Graph &graph)
{
    std::vector<std::vector<int>> population;

    std::uniform_int_distribution<int> distribution(0, (int)graph.getMaxM());

    for (int i = 0; i < populationSize; i++)
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

    return population;
}