#ifndef CVRP_ROUTE_EXCHANGE_H
#define CVRP_ROUTE_EXCHANGE_H

/*






*/

#include "algorithms/local_search/operator.h"

namespace EfOVRP {
namespace cvrp {

class RouteExchange : public ls::Operator {
protected:
  virtual void compute_gain() override;

public:
  RouteExchange(const Input& input,
                const utils::SolutionState& sol_state,
                RawRoute& s_route,
                Index s_vehicle,
                RawRoute& t_route,
                Index t_vehicle);

  virtual bool is_valid() override;

  virtual void apply() override;

  virtual std::vector<Index> addition_candidates() const override;

  virtual std::vector<Index> update_candidates() const override;
};

} // namespace cvrp
} // namespace EfOVRP

#endif
