#include "../../include/algorithms/lkh_solver.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>

// 1. Reduzido para evitar integer overflow no LKH (32-bit signed int)
const long long INF_COST = 999999; 

LKHSolver::LKHSolver(std::string lkh_path, std::string temp_path) 
    : lkh_exec_path(lkh_path), temp_dir(temp_path) {}

void LKHSolver::writePARFile(const std::string &par_filename, const std::string &tsp_filename, const std::string &tour_filename) {
    std::ofstream out(par_filename);
    out << "PROBLEM_FILE = " << tsp_filename << "\n";
    out << "OUTPUT_TOUR_FILE = " << tour_filename << "\n";
    out << "RUNS = 1\n";
    out << "TRACE_LEVEL = 0\n";
    out << "PRECISION = 1\n"; // 2. Força precisão 1 para matrizes com pesos inteiros explícitos
    out.close();
}

void LKHSolver::writeATSPFile(Graph &graph, const std::string &filename, 
                              std::vector<std::pair<int, int>> &id_to_pair) 
{
    int n = graph.getNumVertex();
    id_to_pair.clear();

    std::map<std::pair<int, int>, int> pair_to_id;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                int id = id_to_pair.size();
                pair_to_id[{i, j}] = id;
                id_to_pair.push_back({i, j});
            }
        }
    }

    int N_prime = id_to_pair.size();

    std::ofstream out(filename);
    out << "NAME: qtsp_transformed\n";
    out << "TYPE: ATSP\n";
    out << "DIMENSION: " << N_prime << "\n";
    out << "EDGE_WEIGHT_TYPE: EXPLICIT\n";
    out << "EDGE_WEIGHT_FORMAT: FULL_MATRIX\n";
    out << "EDGE_WEIGHT_SECTION\n";

    for (int u = 0; u < N_prime; ++u) {
        auto [i, j] = id_to_pair[u];

        for (int v = 0; v < N_prime; ++v) {
            auto [k, l] = id_to_pair[v];

            if (u == v) {
                out << "0 ";
            } 
            else if (j == k && i != l) {
                out << graph.custo[i][j][l] << " ";
            } 
            else {
                out << INF_COST << " ";
            }
        }
        out << "\n";
    }

    out << "EOF\n";
    out.close();
}

std::vector<int> LKHSolver::readATSPTourFile(const std::string &tour_filename, 
                                            const std::vector<std::pair<int, int>> &id_to_pair, 
                                            int n_original) 
{
    std::ifstream in(tour_filename);
    std::string line;
    std::vector<int> tour;

    if (!in.is_open()) return tour;

    bool tour_section = false;
    std::vector<int> atsp_nodes;

    while (std::getline(in, line)) {
        if (line.find("TOUR_SECTION") != std::string::npos) {
            tour_section = true;
            continue;
        }
        if (line == "-1" || line.find("EOF") != std::string::npos) {
            break;
        }
        if (tour_section) {
            int node_id = std::stoi(line) - 1;
            atsp_nodes.push_back(node_id);
        }
    }
    in.close();

    for (int atsp_node : atsp_nodes) {
        if (atsp_node >= 0 && atsp_node < (int)id_to_pair.size()) {
            int destination_vertex = id_to_pair[atsp_node].second;
            
            if (tour.empty() || tour.back() != destination_vertex) {
                tour.push_back(destination_vertex);
            }
        }
    }

    if ((int)tour.size() > n_original) {
        tour.resize(n_original);
    }

    return tour;
}

std::vector<int> LKHSolver::run(Graph &graph) {
    std::string atsp_file = temp_dir + "qtsp_inst.atsp";
    std::string par_file  = temp_dir + "qtsp_inst.par";
    std::string tour_file = temp_dir + "qtsp_inst.tour";

    std::vector<std::pair<int, int>> id_to_pair;

    writeATSPFile(graph, atsp_file, id_to_pair);
    writePARFile(par_file, atsp_file, tour_file);

    // Mantenha o redirecionamento desativado ou reative para rodar limpo
    std::string cmd = lkh_exec_path + " " + par_file + " > /dev/null 2>&1";
    int status = std::system(cmd.c_str());

    if (status != 0) {
        std::cerr << "[ERRO] Falha ao executar o LKH para o QTSP!" << std::endl;
        return {};
    }

    return readATSPTourFile(tour_file, id_to_pair, graph.getNumVertex());
}