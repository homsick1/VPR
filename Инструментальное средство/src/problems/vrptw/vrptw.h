#ifndef VRPTW_H
#define VRPTW_H

/*






*/

#include <vector>

#include "problems/vrp.h"

namespace EfOVRP {

class VRPTW : public VRP {
private:
  static const std::vector<HeuristicParameters> homogeneous_parameters;
  static const std::vector<HeuristicParameters> heterogeneous_parameters;

public:
  VRPTW(const Input& input);

  virtual Solution
  solve(unsigned exploration_level,
        unsigned nb_threads,
        const std::vector<HeuristicParameters>& h_param) const override;
};

} // namespace EfOVRP

#endif
