#ifndef VRPTW_ROUTE_EXCHANGE_H
#define VRPTW_ROUTE_EXCHANGE_H

/*






*/

#include "problems/cvrp/operators/route_exchange.h"
#include "structures/EfOVRP/tw_route.h"

namespace EfOVRP {
namespace vrptw {

class RouteExchange : public cvrp::RouteExchange {
private:
  TWRoute& _tw_s_route;
  TWRoute& _tw_t_route;

public:
  RouteExchange(const Input& input,
                const utils::SolutionState& sol_state,
                TWRoute& tw_s_route,
                Index s_vehicle,
                TWRoute& tw_t_route,
                Index t_vehicle);

  virtual bool is_valid() override;

  virtual void apply() override;
};

} // namespace vrptw
} // namespace EfOVRP

#endif
