#include <iostream>
#include "../include/benchmark/benchmark.h"
#include<fstream>
using namespace std;

#include "../include/algorithms/rcl.h"
#include <climits>
#include <string>
#include <random>

// //stdout is not working due to irace, we can debug by printing to a file
// void debug(std::string st){
//     std::ofstream myfile;
//     myfile.open ("debFile.txt",std::ios_base::app);
//     myfile << st<<"\n";
//     myfile.close();
// }




void runRcl(int argc, char *argv[]){
    if(argc == 1){
        Graph graph(3); // Assuming 5 nodes for this example

        // Add edges with costs (example edges)
        graph.addEdge(0, 1, 2, 10);
        graph.addEdge(1, 2, 0, 20);
        graph.addEdge(2, 0, 1, 30);
        graph.addEdge(0, 2, 1, 40);
        graph.addEdge(1, 0, 0, 50);
        graph.addEdge(2, 1, 1, 60);
        // graph.addEdge(2, 0, 3, 70);
        // graph.addEdge(3, 1, 1, 80);

    

        // Initialize  algorithm
        RCL rclAlgo;

        // Run the  algorithm
        std::vector<int> bestTour = rclAlgo.run(graph);
        std::cout<<"Tour size: "<<bestTour.size()<<std::endl;

        // Calculate and print the cost of the best tour
        int bestCost = rclAlgo.eval(bestTour, graph);
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
    runRcl(argc, argv);
    return 0;
}

