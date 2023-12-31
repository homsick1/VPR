#ifndef CVRP_CROSS_EXCHANGE_H
#define CVRP_CROSS_EXCHANGE_H

#include "algorithms/local_search/operator.h"

namespace EfOVRP {
namespace cvrp {

class CrossExchange : public ls::Operator {
private:
  bool _gain_upper_bound_computed;
  Gain _normal_s_gain;
  Gain _reversed_s_gain;
  Gain _normal_t_gain;
  Gain _reversed_t_gain;

protected:
  bool reverse_s_edge;
  bool reverse_t_edge;
  const bool check_s_reverse;
  const bool check_t_reverse;

  bool s_is_normal_valid;
  bool s_is_reverse_valid;
  bool t_is_normal_valid;
  bool t_is_reverse_valid;

  virtual void compute_gain() override;

public:
  CrossExchange(const Input& input,
                const utils::SolutionState& sol_state,
                RawRoute& s_route,
                Index s_vehicle,
                Index s_rank,
                RawRoute& t_route,
                Index t_vehicle,
                Index t_rank,
                bool check_s_reverse,
                bool check_t_reverse);

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
