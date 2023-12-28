#ifndef VRPTW_MIXED_EXCHANGE_H
#define VRPTW_MIXED_EXCHANGE_H

/*






*/

#include "problems/cvrp/operators/mixed_exchange.h"
#include "structures/EfOVRP/tw_route.h"

namespace EfOVRP {
namespace vrptw {

class MixedExchange : public cvrp::MixedExchange {
private:
  TWRoute& _tw_s_route;
  TWRoute& _tw_t_route;

public:
  MixedExchange(const Input& input,
                const utils::SolutionState& sol_state,
                TWRoute& tw_s_route,
                Index s_vehicle,
                Index s_rank,
                TWRoute& tw_t_route,
                Index t_vehicle,
                Index t_rank,
                bool check_t_reverse);

  virtual bool is_valid() override;

  virtual void apply() override;
};

} // namespace vrptw
} // namespace EfOVRP

#endif
