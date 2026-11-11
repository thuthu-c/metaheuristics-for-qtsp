#ifndef _GRASPSWAPVND_H_
#define _GRASPSWAPVND_H_

#include "../algorithms/grasp.h"
#include "../data_structures/graph.h"
class GRASPSWAPVND : public GRASP
{
public:
    std::vector<int> run(Graph &graph);

};


#endif