#ifndef _GRASPSHIFTVND_H_
#define _GRASPSHIFTVND_H_

#include "../algorithms/grasp.h"
#include "../data_structures/graph.h"
class GRASPSHIFTVND : public GRASP
{
public:
    std::vector<int> run(Graph &graph);

};


#endif