#include "../../include/helpers/random.h"
#include <vector>
#include <algorithm>
#include <climits>
#include <iostream>
#include <string>
#include <unordered_set>
#include "../../include/algorithms/4_opt_util.h"


std::vector<int> FourOptUtil::run(std::vector<int> tour){
    return best4opt(tour);
}

