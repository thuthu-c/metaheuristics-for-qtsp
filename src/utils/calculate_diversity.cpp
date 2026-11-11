#include <algorithm>
#include "individual.cpp"

double dc(Individual& Pi, std::vector<Individual> population, int n, int n_close) {
    double resultSum{0};
    for (auto j{1}; j <= n_close; ++j)
    {
        Individual Pj = population[j];
        resultSum += normalizeBronkenPairsDistance(Pi, Pj, n);
    }
    return (1.0 / n_close) * resultSum;
}


double normalizeBronkenPairsDistance(Individual Pi, Individual Pj, int n)
{
    return (1.0 / n) * numberOfSuccesivesPairsInATourPiWhichAreNotIncludedInPj(Pi, Pj);
}

long int numberOfSuccesivesPairsInATourPiWhichAreNotIncludedInPj(Individual Pi, Individual Pj)
{
    
    long int numberOfSuccesivesPairsInATourPiWhichAreNotIncludedInPj{0};
    std::vector<std::pair<int, int>> pairsPi = generateAllNodesPairs(Pi);
    std::vector<std::pair<int, int>> pairsPj = generateAllNodesPairs(Pj);

    for (auto i : pairsPi)
    {
        const auto search = std::find(pairsPj.begin(), pairsPj.end(), i);
        const auto unsucceeded = pairsPj.end();
        if (search != unsucceeded)
            numberOfSuccesivesPairsInATourPiWhichAreNotIncludedInPj++;
    }
    return numberOfSuccesivesPairsInATourPiWhichAreNotIncludedInPj;
}

std::vector<std::pair<int, int>> generateAllNodesPairs(Individual individual)
{
   
    std::vector<std::pair<int, int>> pairs;

    for (auto i{0}; i < individual.tour.size() - 1; ++i)
    {
        pairs.push_back(std::make_pair(individual.tour[i], individual.tour[i + 1]));
    }

    return pairs;
}