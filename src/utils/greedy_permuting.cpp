#include <vector> 
#include <algorithm>
#include <climits>
#include "../../include/data_structures/graph.h"
std::vector<int> greedyPermuting(std::vector<int> &seedIndividual, Graph &graph, int n)
{
    std::vector<int> individual = seedIndividual;
    std::vector<int> restCities;
    for (int i = 0; i < n; i++)
    {
        if (std::find(individual.begin(), individual.end(), i) == individual.end()) // valor não existe no vetor)
        {
            restCities.push_back(i);
        }
    }

    while (!restCities.empty())
    {
        if (individual.empty()) 
        {
            int startCity = restCities.front();
            individual.push_back(startCity);
            restCities.erase(restCities.begin());
            continue; // Go to the next iteration safely
        }

        int nextCity = restCities[0];
        int minCost = INT_MAX;
        for (int i = 0; i < n; i++)
        {
            if (graph.custo[individual.back()][nextCity][i] < minCost)
            {
                minCost = graph.custo[individual.back()][nextCity][i];
            }
        }
        for (int city : restCities)
        {
            
            int cost = INT_MAX;
            for (int i = 0; i < n; i++)
            {
                if (graph.custo[individual.back()][city][i] < cost)
                {
                    cost = graph.custo[individual.back()][city][i];
                }
            }
            if (cost < minCost)
            {
                nextCity = city;
                minCost = cost;
            }
        }
        individual.push_back(nextCity);
        restCities.erase(std::remove(restCities.begin(), restCities.end(), nextCity), restCities.end());
    }

    // Remove the initial seed cities from the result to return only the new cities
    for(int city : seedIndividual)
    {
        individual.erase(std::remove(individual.begin(), individual.end(), city), individual.end());
    }

    return individual;
}