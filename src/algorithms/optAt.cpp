#include "../../include/algorithms/optAt.h"
#include "./utils/population.cpp"

std::vector<int> OptAt::run(Graph& graphInput){
    this->numEvaluation = 0;
    graph = &graphInput;
    if(graph->getNumVertex() < 100){
        populationSize = std::min(graph->getNumVertex(), populationSize);
    }

    Population populationIni;
    population = populationIni.generate_population(graph, populationSize);
    std::vector<std::vector<int>> girConverted;

    // isso aqui pode ser um parametro depois...
    std::vector<std::vector<int>> plasmidBank; 
    int step = 30000; 
    int bankSize = 10;

    auto atualizarGirBiased = [&]() {

       populationIni.individualCostRank(population);
       populationIni.individualDiversityRank(population);
       populationIni.evaluatePopulation(population, 5);
        
        
        // adicionado depois do teste principal;
         std::sort(population.begin(), population.end(), [](const Individual &a, const Individual &b){
            return a.biasedFitness < b.biasedFitness;
        });
       
        int tam_gir = 0.5 * this->populationSize;
        

        // a escolha dos elites eh baseada no biased fitness 
        std::vector<Individual*> elites;

        for (auto& i : population) elites.push_back(&i);
        
        // a ordenacao do gir, eh baseada no custo (funcao objetivo)
        std::sort(elites.begin(), elites.end(), [](const Individual* a, const Individual* b){
            return a->cost < b->cost;
        });

        girConverted.clear();

        for(int i = 0; i < tam_gir; i++) {
            girConverted.push_back(elites[i]->tour);
        }

        plasmidBank.clear();
        // std::cout<< "atualizando o banco de plasmideos "<< contador << "vezes" << std::endl;
        std::uniform_real_distribution<double> distribution_plasmid(this->plasmidMin, this->plasmidMax);
        
       
        if(!girConverted.empty()){
            for(auto i{0}; i < bankSize; ++i){
                plasmidSize =  distribution_plasmid(engine);
                plasmidBank.push_back(generate_plasmid(girConverted, plasmidSize));
            }

        }
    };


    atualizarGirBiased();


    std::vector<int> best_solution = population.begin()->tour;
    long long int best_cost = population.begin()->cost;

    for(const auto& p : population) {
        if(p.cost < best_cost) {
            best_cost = p.cost;
            best_solution = p.tour;
        }
    }

    double probP = 1.0 - probT;
    
    auto ajustarProbsEAtualizarGIR = [&]() {
        
        probT = std::max(probTmin, probT - stepProb);
        probP = 1.0 - probT;

        
    }; 

    std::uniform_real_distribution<double> dist_prob(0.0, 1.0);
    

    while(num_evaluations > numEvaluation){

        for(auto& p: population){
            
            if (numEvaluation >= num_evaluations) break;

            if (numEvaluation >= step) {
           
                ajustarProbsEAtualizarGIR();
                if(step > stepsValueLimit){ 
                    step -= variacaoSteps;
             
                } 
            }

            
            if(numEvaluation >= (step/2)){
                atualizarGirBiased();
            } 

            double chance = dist_prob(engine);
            long long int current_cost = p.cost;
            
            if(chance <= probP){

                
              

                long long int best_candidate_cost = LLONG_MAX;
                std::vector<int> best_candidate_solution;
                   

                std::uniform_int_distribution<int> distribution(0, bankSize);
                
                std::vector<Plasmid> choosedPlasmids;

           

                 for(auto i{0}; i < plasmidsToBeChoosen; ++i){

                     int randIndex = distribution(engine) % bankSize;
           
                    choosedPlasmids.emplace_back(plasmidBank[randIndex]);
                 }


                for(const auto& plasmid : choosedPlasmids){
                 
                    std::vector<int> candidate_solution = m1(plasmid, p.tour.size(), p.tour);

                
                    long long int candidate_cost = cost(candidate_solution);

                    if(candidate_cost < best_candidate_cost) {
                        best_candidate_cost = candidate_cost;
                        best_candidate_solution = candidate_solution;
                    }


                }
                
                if(best_candidate_cost < current_cost){
                    p.tour = best_candidate_solution;
                    p.cost = best_candidate_cost;
                    current_cost = best_candidate_cost; 
                }

                if(current_cost < best_cost) {
                best_solution = p.tour;
                best_cost = current_cost;
            }


            }else{
               
                std::vector<int> transposon = transposon_4OPT(p.tour);
             
                long long int transposon_cost = cost(transposon);
                
                if(transposon_cost < current_cost){
                    p.tour = transposon;
                    p.cost = transposon_cost;
                    current_cost = transposon_cost; 
                }
            }
            
            if(current_cost < best_cost) {
                best_solution = p.tour;
                best_cost = current_cost;
            }
        }
    }

    int gir_best_cost = cost(girConverted[0]); 

    if(gir_best_cost < best_cost){
        best_solution = girConverted[0];
        best_cost = gir_best_cost; 
        taNoGir = true;
    }else taNoPop = true;

    // std::cout << "ta no gir? " << taNoGir << " ou ta no pop? " << taNoPop << std::endl;


    return best_solution;

}