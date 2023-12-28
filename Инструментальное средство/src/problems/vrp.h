#ifndef VRP_H
#define VRP_H

/*






*/

#include "structures/EfOVRP/solution/solution.h"

namespace EfOVRP {

class Input;

class VRP {
  // Abstract class describing a VRP (vehicle routing problem).
protected:
  const Input& _input;

public:
  VRP(const Input& input);

  virtual ~VRP();

  virtual Solution
  solve(unsigned exploration_level,
        unsigned nb_threads,
        const std::vector<HeuristicParameters>& h_param) const = 0;
};

} // namespace EfOVRP

#endif
