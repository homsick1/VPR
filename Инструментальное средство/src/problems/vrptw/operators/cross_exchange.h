#ifndef VRPTW_CROSS_EXCHANGE_H
#define VRPTW_CROSS_EXCHANGE_H

#include "problems/cvrp/operators/cross_exchange.h"
#include "structures/EfOVRP/tw_route.h"

namespace EfOVRP {
namespace vrptw {

class CrossExchange : public cvrp::CrossExchange {
private:
  TWRoute& _tw_s_route;
  TWRoute& _tw_t_route;

public:
  CrossExchange(const Input& input,
                const utils::SolutionState& sol_state,
                TWRoute& tw_s_route,
                Index s_vehicle,
                Index s_rank,
                TWRoute& tw_t_route,
                Index t_vehicle,
                Index t_rank,
                bool check_s_reverse,
                bool check_t_reverse);

  virtual bool is_valid() override;

  virtual void apply() override;
};

} // namespace vrptw
} // namespace EfOVRP

#endif
