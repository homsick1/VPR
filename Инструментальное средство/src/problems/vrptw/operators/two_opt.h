#ifndef VRPTW_TWO_OPT_H
#define VRPTW_TWO_OPT_H

/*






*/

#include "problems/cvrp/operators/two_opt.h"
#include "structures/EfOVRP/tw_route.h"

namespace EfOVRP {
namespace vrptw {

class TwoOpt : public cvrp::TwoOpt {
private:
  TWRoute& _tw_s_route;
  TWRoute& _tw_t_route;

public:
  TwoOpt(const Input& input,
         const utils::SolutionState& sol_state,
         TWRoute& tw_s_route,
         Index s_vehicle,
         Index s_rank,
         TWRoute& tw_t_route,
         Index t_vehicle,
         Index t_rank);

  virtual bool is_valid() override;

  virtual void apply() override;
};

} // namespace vrptw
} // namespace EfOVRP

#endif
