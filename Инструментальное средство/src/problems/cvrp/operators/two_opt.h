#ifndef CVRP_TWO_OPT_H
#define CVRP_TWO_OPT_H

/*






*/

#include "algorithms/local_search/operator.h"

namespace EfOVRP {
namespace cvrp {

class TwoOpt : public ls::Operator {
protected:
  virtual void compute_gain() override;

public:
  TwoOpt(const Input& input,
         const utils::SolutionState& sol_state,
         RawRoute& s_route,
         Index s_vehicle,
         Index s_rank,
         RawRoute& t_route,
         Index t_vehicle,
         Index t_rank);

  virtual bool is_valid() override;

  virtual void apply() override;

  virtual std::vector<Index> addition_candidates() const override;

  virtual std::vector<Index> update_candidates() const override;
};

} // namespace cvrp
} // namespace EfOVRP

#endif
