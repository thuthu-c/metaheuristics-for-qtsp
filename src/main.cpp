#include <iostream>
#include "../include/benchmark/benchmark.h"
#include<fstream>
#include <chrono>
#include <vector>

#include "../include/algorithms/rcl.h"
#include "../include/algorithms/grasp.h"
#include "../include/algorithms/4_opt.h"
#include <climits>
#include <string>
#include <random>
#include <cstdlib>
#include "../include/helpers/graphio.h"
#include "../include/algorithms/lkh_solver.h"

// int eval(std::vector<int> &tour, Graph &graph)
// {
//     int tourVal = 0;
//     int n = tour.size();

//     tourVal += graph.custo[tour[n - 2]][tour[n - 1]][tour[0]];
//     tourVal += graph.custo[tour[n - 1]][tour[0]][tour[1]];

//     for (int j = 0; j < (n - 2); j++)
//     {
//         tourVal += graph.custo[tour[j]][tour[j + 1]][tour[j + 2]];
//     }

//     return tourVal;
// }


// void runlkh(){
//     // 1. Garante que o diretório temporário exista
//     std::system("mkdir -p temp");

//     // 2. Carrega a sua instância de teste
//     GraphIO graphio;
//     std::string testFile = "data/FIS50-0.txt"; // Altere para a sua instância de teste
    
//     std::cout << "Carregando o grafo de: " << testFile << "..." << std::endl;
//     graphio.read(testFile);
//     Graph graph = graphio.getGraph();

//     std::cout << "Grafo carregado com " << graph.getNumVertex() << " vertices." << std::endl;

//     // 3. Instancia e roda o LKHSolver
//     LKHSolver lkh("./bin/LKH", "./temp/");

//     std::cout << "Executando o LKH-3..." << std::endl;
//     std::vector<int> tour = lkh.run(graph);

//     // 4. Exibe os resultados
//     if (tour.empty()) {
//         std::cerr << "[ERRO] LKH retornou uma rota vazia!" << std::endl;
//     }

//     std::cout << "\n================ RESULTADO LKH ================" << std::endl;
//     std::cout << "Tamanho do vetor de rota: " << tour.size() << std::endl;
//     std::cout << "Custo da rota: " << eval(tour, graph) << std::endl; 
//     std::cout << "Rota: ";
//     for (int v : tour) {
//         std::cout << v << " ";
//     }
//     std::cout << "\n==============================================" << std::endl;
// }

void runRcl(int argc, char *argv[]){
    if(argc == 1){
        Graph graph(5); // Assuming 5 nodes for this example

        // Add edges with costs (example edges)
        graph.addEdge(0, 1, 2, 10);
        graph.addEdge(1, 2, 0, 20);
        graph.addEdge(2, 0, 1, 30);
        graph.addEdge(0, 2, 1, 40);
        graph.addEdge(1, 0, 0, 50);
        graph.addEdge(2, 1, 1, 60);
        graph.addEdge(2, 0, 3, 70);
        graph.addEdge(3, 1, 2, 80);
        graph.addEdge(4, 3, 2, 90);
        graph.addEdge(4, 1, 2, 100);
        graph.addEdge(2, 4, 0, 110);
        graph.addEdge(2, 0, 4, 120);
        graph.addEdge(4, 0, 1, 3);
        graph.addEdge(0, 1, 4, 1);
        graph.addEdge(0, 2, 4, 5);
        graph.addEdge(0, 4, 2, 6);

    

        // Initialize  algorithm
        RCL rclAlgo;

        // Run the  algorithm
        std::vector<int> bestTour = rclAlgo.run(graph);
        std::vector<int> bestTourTwo = rclAlgo.run_two(graph);
        std::cout<<"Tour ONE size: "<<bestTour.size()<<std::endl;
        std::cout<<"Tour TWO size: "<<bestTourTwo.size()<<std::endl;

        // Calculate and print the cost of the best tour
        int bestCost = rclAlgo.eval(bestTour, graph);
        int bestCostTwo = rclAlgo.eval(bestTourTwo, graph);
        if(bestCost < INT_MAX){std::cout << "Best tour cost: " << bestCost << std::endl;
        std::cout << "Best tour two cost: " << bestCostTwo << std::endl;
        }
        else std::cout<<"There is no valid tour for this graph :("<<std::endl;

        // Print the best tour
        std::cout << "Best tour One: ";
        for (int city : bestTour) {
            std::cout << city << " ";
        }
        std::cout << std::endl;

        std::cout << "Best tour Two: ";
        for (int city : bestTourTwo) {
            std::cout << city << " ";
        }
        std::cout << std::endl;

        return;
    }
   
    if(argc == 2) {

        Benchmark benchmark;

        benchmark.evaluate();
    } else {
        
        std::string instance = argv[2];
        std::string algorithm = argv[3];
        
        Benchmark benchmark;

        benchmark.evaluate(instance, algorithm);
    }
}

void runGrasp(int argc, char *argv[]){
    if(argc == 1){
        Graph graph(5); // Assuming 5 nodes for this example
        std::cout<< "OI TO NO GRASP" << std::endl; 
        // Add edges with costs (example edges)
        graph.addEdge(0, 1, 2, 10);
        graph.addEdge(1, 2, 0, 20);
        graph.addEdge(2, 0, 1, 30);
        graph.addEdge(0, 2, 1, 40);
        graph.addEdge(1, 0, 0, 50);
        graph.addEdge(2, 1, 1, 60);
        graph.addEdge(2, 0, 3, 70);
        graph.addEdge(3, 1, 2, 80);
        graph.addEdge(4, 3, 2, 90);
        graph.addEdge(4, 1, 2, 100);
        graph.addEdge(2, 4, 0, 110);
        graph.addEdge(2, 0, 4, 120);
        graph.addEdge(4, 0, 1, 3);
        graph.addEdge(0, 1, 4, 1);
        graph.addEdge(0, 2, 4, 5);
        graph.addEdge(0, 4, 2, 6);

        // Initialize  algorithm
        GRASP graspAlgo;

        // Run the  algorithm
        std::vector<int> bestTour = graspAlgo.run(graph);
        std::cout<<"Tour size: "<<bestTour.size()<<std::endl;

        // Calculate and print the cost of the best tour
        int bestCost = graspAlgo.eval(bestTour, graph);
        if(bestCost < INT_MAX)std::cout << "Best tour cost: " << bestCost << std::endl;
        else std::cout<<"There is no valid tour for this graph :("<<std::endl;

        // Print the best tour
        std::cout << "Best tour: ";
        for (int city : bestTour) {
            std::cout << city << " ";
        }
        std::cout << std::endl;

        return;
    }
   
    if(argc == 2) {

        Benchmark benchmark;

        benchmark.evaluate();
    } else {
        std::cout<<"oi bora rodar" << std::endl; 
        
        std::string instance = argv[2];
        std::string algorithm = argv[3];
        
        Benchmark benchmark;

        benchmark.evaluate(instance, algorithm);
    }
}

void run4opt(int argc, char *argv[]){
    if(argc == 1){
        Graph graph(5); // Assuming 5 nodes for this example

        // Add edges with costs (example edges)
        graph.addEdge(0, 1, 2, 10);
        graph.addEdge(1, 2, 0, 20);
        graph.addEdge(2, 0, 1, 30);
        graph.addEdge(0, 2, 1, 40);
        graph.addEdge(1, 0, 0, 50);
        graph.addEdge(2, 1, 1, 60);
        graph.addEdge(2, 0, 3, 70);
        graph.addEdge(3, 1, 2, 80);
        graph.addEdge(4, 3, 2, 90);
        graph.addEdge(4, 1, 2, 100);
        graph.addEdge(2, 4, 0, 110);
        graph.addEdge(2, 0, 4, 120);
        graph.addEdge(4, 0, 1, 3);
        graph.addEdge(0, 1, 4, 1);
        graph.addEdge(0, 2, 4, 5);
        graph.addEdge(0, 4, 2, 6);

        // Initialize  algorithm
        FourOpt fourOptAlgo(graph);

        // Run the  algorithm
        std::vector<int> bestTour = fourOptAlgo.run(graph);
        std::cout<<"Tour size: "<<bestTour.size()<<std::endl;

        // Calculate and print the cost of the best tour
        int bestCost = fourOptAlgo.eval(bestTour, graph);
        if(bestCost < INT_MAX)std::cout << "Best tour cost: " << bestCost << std::endl;
        else std::cout<<"There is no valid tour for this graph :("<<std::endl;

        // Print the best tour
        std::cout << "Best tour: ";
        for (int city : bestTour) {
            std::cout << city << " ";
        }
        std::cout << std::endl;

        return;
    }
   
    if(argc == 2) {

        Benchmark benchmark;

        benchmark.evaluate();
    } else {
        
        std::string instance = argv[2];
        std::string algorithm = argv[3];
        
        Benchmark benchmark;

        benchmark.evaluate(instance, algorithm);
    }
}

int main(int argc, char *argv[])
{
    // runRcl(argc, argv);
    // runGrasp(argc, argv);
    // run4opt(argc, argv);
    // runlkh();


    std::system("mkdir -p temp");

    GraphIO graphio;
    std::string filename = "data/FIS10-4.txt"; // Ajuste o caminho conforme o seu projeto
    
    std::cout << "Carregando a instancia " << filename << "..." << std::endl;
    graphio.read(filename);
    Graph graph = graphio.getGraph();

    int n = graph.getNumVertex();
    std::cout << "Instancia carregada com " << n << " vertices." << std::endl;
    std::cout << "Matriz ATSP gerada tera dimensao N' = " << n * (n - 1) << " nos.\n" << std::endl;

    GRASP grasp;

    // -------------------------------------------------------------
    // 1. Execução do GRASP
    // -------------------------------------------------------------
    std::cout << "[1/2] Executando GRASP..." << std::endl;
    auto start_grasp = std::chrono::high_resolution_clock::now();
    
    std::vector<int> tour_grasp = grasp.run(graph);
    
    auto end_grasp = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time_grasp = end_grasp - start_grasp;
    
    int cost_grasp = grasp.eval(tour_grasp, graph);

    // -------------------------------------------------------------
    // 2. Execução do LKH-3 (ATSP Transformado)
    // -------------------------------------------------------------
    std::cout << "[2/2] Executando LKH-3 (Transformacao ATSP)..." << std::endl;
    LKHSolver lkh("./bin/LKH", "./temp/");
    
    auto start_lkh = std::chrono::high_resolution_clock::now();
    
    std::vector<int> tour_lkh = lkh.run(graph);
    
    if (tour_lkh.empty()) {
        std::cerr << "[ERRO] Não foi possível obter a rota do LKH-3. Interrompendo avaliação." << std::endl;
        return 1;
    }
    
    auto end_lkh = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time_lkh = end_lkh - start_lkh;

    // Avalia o resultado do LKH usando a MESMA funcao eval do QTSP
    int cost_lkh = grasp.eval(tour_lkh, graph);

    // -------------------------------------------------------------
    // 3. Exibição e Comparação dos Resultados
    // -------------------------------------------------------------
    std::cout << "\n==================================================" << std::endl;
    std::cout << "             RESULTADOS DA COMPARACAO             " << std::endl;
    std::cout << "==================================================" << std::endl;
    
    std::cout << "GRASP:" << std::endl;
    std::cout << "  - Custo QTSP : " << cost_grasp << std::endl;
    std::cout << "  - Tempo      : " << time_grasp.count() << " ms" << std::endl;
    std::cout << "  - Vertices   : " << tour_grasp.size() << std::endl;

    std::cout << "\nLKH-3 (ATSP):" << std::endl;
    std::cout << "  - Custo QTSP : " << cost_lkh << std::endl;
    std::cout << "  - Tempo      : " << time_lkh.count() << " ms" << std::endl;
    std::cout << "  - Vertices   : " << tour_lkh.size() << std::endl;

    if (cost_lkh > 0) {
        double gap = ((double)(cost_grasp - cost_lkh) / cost_lkh) * 100.0;
        std::cout << "\nGAP do GRASP em relacao ao LKH-3: " << gap << "%" << std::endl;
    }
    std::cout << "==================================================" << std::endl;


    return 0;
}

