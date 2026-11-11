#include <vector> 
struct Individual {
    int size;
    std::vector<int> tour;
    long long int cost;
    int costRank; 
    double diversityContribution;
    int diversityRank;
    double biasedFitness;

    
};