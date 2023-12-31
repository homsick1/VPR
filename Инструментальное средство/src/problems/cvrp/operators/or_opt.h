#ifndef CVRP_OR_OPT_H
#define CVRP_OR_OPT_H

/*






*/

#include "algorithms/local_search/operator.h"

namespace EfOVRP {
namespace cvrp {

class OrOpt : public ls::Operator {
private:
  bool _gain_upper_bound_computed;
  Gain _s_gain;
  Gain _normal_t_gain;
  Gain _reversed_t_gain;

protected:
  bool reverse_s_edge;
  bool is_normal_valid;
  bool is_reverse_valid;

  virtual void compute_gain() override;

public:
  OrOpt(const Input& input,
        const utils::SolutionState& sol_state,
        RawRoute& s_route,
        Index s_vehicle,
        Index s_rank,
        RawRoute& t_route,
        Index t_vehicle,
        Index t_rank);

  // Compute and store all possible cost depending on whether edges
  // are reversed or not. Return only an upper bound for gain as
  // precise gain requires validity information.
  Gain gain_upper_bound();

  virtual bool is_valid() override;

  virtual void apply() override;

  virtual std::vector<Index> addition_candidates() const override;

  virtual std::vector<Index> update_candidates() const override;
};

} // namespace cvrp
} // namespace EfOVRP

#endif
