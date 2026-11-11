#include <iostream>
#include "../include/benchmark/benchmark.h"
#include<fstream>
using namespace std;

#include "../include/algorithms/rcl.h"
#include "../include/algorithms/grasp.h"
#include "../include/algorithms/4_opt.h"
#include <climits>
#include <string>
#include <random>

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
    runGrasp(argc, argv);
    // run4opt(argc, argv);
    return 0;
}

