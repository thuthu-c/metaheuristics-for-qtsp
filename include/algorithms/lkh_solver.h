#ifndef LKH_SOLVER_H
#define LKH_SOLVER_H

#include "tsp_solver.h"
#include "../helpers/graphio.h"
#include <string>
#include <vector>
#include <utility>

class LKHSolver : public TspSolver {
private:
    std::string lkh_exec_path;
    std::string temp_dir;

    void writeATSPFile(Graph &graph, const std::string &filename, std::vector<std::pair<int, int>> &id_to_pair);
    void writePARFile(const std::string &par_filename, const std::string &tsp_filename, const std::string &tour_filename);
    std::vector<int> readATSPTourFile(const std::string &tour_filename, const std::vector<std::pair<int, int>> &id_to_pair, int n_original);

public:
    LKHSolver(std::string lkh_path = "./bin/LKH", std::string temp_path = "./temp/");
    virtual ~LKHSolver() = default;

    std::vector<int> run(Graph &graph) override;
};

#endif