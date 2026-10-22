#ifndef _FOUR_OPT_UTIL_H_
#define _FOUR_OPT_UTIL_H_

#include "../algorithms/4_opt.h"
#include "../data_structures/graph.h"
class FourOptUtil : public FourOpt
{
public:
    explicit FourOptUtil(Graph &g) : FourOpt(g) {}
    using FourOpt::run; 
    std::vector<int> run(std::vector<int> tour);
};


#endif