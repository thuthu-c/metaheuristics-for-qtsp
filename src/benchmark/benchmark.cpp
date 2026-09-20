#include <iostream>
#include <chrono>
#include <sstream>
#include <fstream>
#include "../../include/data_structures/graph.h"
#include "../../include/benchmark/graph_generator.h"
#include "../../include/algorithms/tsp_solver.h"
#include "../../include/helpers/graphio.h"
#include "../../include/benchmark/benchmark.h"
#include "../../include/algorithms/rcl.h"
#include "../../include/algorithms/cheapest_insertion.h"


Benchmark::Benchmark(){};


Benchmark::~Benchmark(){}

// Generates graph instances from 5 vertexes to 24 vertexes
// each instance contains 5 variations.
// It saves the graphs on disk, returning its paths
std::vector<std::string> generateGraphs(int start, int end)
{
    std::vector<std::string> paths;

    for (int j = start; j <= end; j += 1)   
    {
        for (int i = 0; i < 5; i++)
        {
            GraphGenerator graphGenerator(j);
            auto graph = graphGenerator.generate();

            // writing graph on disk if the filename doesn't exists
            GraphIO graphio;
            graphio.setGraph(graph);

            std::ostringstream graphFilename;
            graphFilename << "data/FIS" << j << "-" << i << ".txt";
            std::ifstream infile(graphFilename.str());

            if (!infile.good())
            {
                graphio.write(graphFilename.str());
            }

            paths.push_back(graphFilename.str());
        }
    }

    return paths;
}

int tourLength(std::vector<int> tour, Graph graph)
{
    int tourVal = 0;
    int n = graph.getMaxM();

    tourVal += graph.custo[tour[n - 2]][tour[n - 1]][tour[0]];
    tourVal += graph.custo[tour[n - 1]][tour[0]][tour[1]];

    for (int j = 0; j < (n - 2); j++)
    {
        tourVal += graph.custo[tour[j]][tour[j + 1]][tour[j + 2]];
    }

    return tourVal;
}

void writeResult(
    std::ofstream &file,
    std::string algorithm,
    int execution,
    std::string filename,
    int numVertex,
    long double time,
    std::vector<int> &path,
    int cost,
    std::vector<int> &path_ini,
    int cost_ini)
{
    file << algorithm << ";";
    file << execution << ";";
    file << filename << ";";
    file << numVertex << ";";

    for (int i = 0; i < (int)path_ini.size(); i++)
    {
        file << path_ini[i];
        if (i != (((int)path_ini.size()) - 1))
        {
            file << ",";
        }
    }
    file << ";";
    file << cost_ini << ";";
    file << time << ";";

    for (int i = 0; i < (int)path.size(); i++)
    {
        file << path[i];
        if (i != (((int)path.size()) - 1))
        {
            file << ",";
        }
    }

    file << ';' << cost << '\n';
}

std::string getAlgorithmName(TspSolver *solver)
{

   
    if(dynamic_cast<RCL*>(solver)){
        return "Restricted Candidate List";
    }else if(dynamic_cast<CheapestInsertion*>(solver)){
        return "Cheapest Insertion";
    }
    
    return "AnotherGenetic";
}

void run(TspSolver *solver, std::string graphFilename, std::ofstream &file)
{
    std::string solverName = getAlgorithmName(solver);

    GraphIO graphio;
    graphio.read(graphFilename);
    Graph graph = graphio.getGraph();

    for (int i = 1; i <= 30; i++)
    {
        // std::cout << "Start running algorithm " << solverName << " for " << graph.getMaxM() << "in execution " << i << std::endl;

        // https://en.cppreference.com/w/cpp/chrono/duration/duration_cast
        auto start = std::chrono::high_resolution_clock::now();
        auto minPath = solver->run(graph); // RUN

        auto end = std::chrono::high_resolution_clock::now();

        int cost = tourLength(minPath, graph);

        long double miliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();


        writeResult(
            file,
            getAlgorithmName(solver),
            i,
            graphFilename,
            graph.getMaxM(),
            miliseconds,
            minPath,
            cost,
            solver->b_ini_p,
            solver->b_ini_c
        );

        // std::cout << "End " << solverName << " execution. It took " << miliseconds << " miliseconds" << std::endl;
    }    
    std::cout<<"Done "<<solverName<<" for "<<graphFilename<<std::endl;
}

int Benchmark::evaluate()
{
    std::vector<TspSolver *> algorithms;

    RCL *rcl =  new RCL();
    CheapestInsertion *ci = new CheapestInsertion();
   
    algorithms.push_back(rcl);
    algorithms.push_back(ci);



    // add header to csv
    std::ofstream outputFile;
    outputFile.open("result.csv", std::ios::app);
    outputFile << "algorithm;execution;filename;num_vertex;initial_path;initial_cost;milisec;min_path;cost\n";
    outputFile.close();
// 
//     // run algorithms for every graph instances
    // for (auto algorithm : algorithms)
    // {
    //     for (std::string graphPath : graphsPath)
    //     {
    //         outputFile.open("result.csv", std::ios::app);
    //         run(algorithm, graphPath, outputFile);
    //         outputFile.close();
    //     }
    // }

    // running big graphs for big instances (ignoring brute force)
    std::vector<std::string> graphsPath = generateGraphs(50, 50);


    for (auto generated : generateGraphs(75, 75))
    {

        graphsPath.push_back(generated);
    }

    for (auto generated : generateGraphs(100, 100))
    {

        graphsPath.push_back(generated);
    }

    for (auto algorithm : algorithms)
    {

        for (auto g : graphsPath)
        {
            outputFile.open("result.csv", std::ios::app);

            run(algorithm, g, outputFile);
            outputFile.close();
        }
    }

    return -1;
}

int Benchmark::evaluate(std::string instance, std::string algorithmName)
{

    TspSolver* algorithm = nullptr;

     if (algorithmName.compare("rcl") == 0){
        // std::cout << "genetic" << std::endl;
        algorithm = new RCL();
    } else  if (algorithmName.compare("cheapest_insertion") == 0){
        // std::cout << "genetic" << std::endl;
        algorithm = new CheapestInsertion();
    }
     

    GraphIO graphio;
    std::cout << "A INSTÂNCIA É: " <<instance<< std::endl;
    graphio.read(instance);
    Graph graph = graphio.getGraph();

    if (algorithm == nullptr) {
    throw std::runtime_error("Algoritmo não foi instanciado!"); 
}

    auto cost = tourLength(algorithm->run(graph), graph);
    // irace `target-runner` expects this output
    std::cout << cost << std::endl;
    delete algorithm; 
    return cost; // RUN
}