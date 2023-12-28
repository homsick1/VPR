#ifndef CVRP_H
#define CVRP_H

#include <vector>

#include "problems/vrp.h"

namespace EfOVRP {

class CVRP : public VRP {
private:
  bool empty_cluster(const std::vector<Index>& cluster, Index v) const;

  static const std::vector<HeuristicParameters> homogeneous_parameters;
  static const std::vector<HeuristicParameters> heterogeneous_parameters;

public:
  CVRP(const Input& input);

  virtual Solution
  solve(unsigned exploration_level,
        unsigned nb_threads,
        const std::vector<HeuristicParameters>& h_param) const override;
};

} // namespace EfOVRP

#endif
