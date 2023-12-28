#ifndef CVRP_INTRA_EXCHANGE_H
#define CVRP_INTRA_EXCHANGE_H

#include "algorithms/local_search/operator.h"

namespace EfOVRP {
namespace cvrp {

class IntraExchange : public ls::Operator {
protected:
  std::vector<Index> _moved_jobs;
  const Index _first_rank;
  const Index _last_rank;

  virtual void compute_gain() override;

public:
  IntraExchange(const Input& input,
                const utils::SolutionState& sol_state,
                RawRoute& s_route,
                Index s_vehicle,
                Index s_rank,
                Index t_rank);

  virtual bool is_valid() override;

  virtual void apply() override;

  virtual std::vector<Index> addition_candidates() const override;

  virtual std::vector<Index> update_candidates() const override;
};

} // namespace cvrp
} // namespace EfOVRP

#endif
