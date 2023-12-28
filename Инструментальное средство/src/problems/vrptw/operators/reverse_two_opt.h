#ifndef VRPTW_REVERSE_TWO_OPT_H
#define VRPTW_REVERSE_TWO_OPT_H

/*






*/

#include "problems/cvrp/operators/reverse_two_opt.h"
#include "structures/EfOVRP/tw_route.h"

namespace EfOVRP {
namespace vrptw {

class ReverseTwoOpt : public cvrp::ReverseTwoOpt {
private:
  TWRoute& _tw_s_route;
  TWRoute& _tw_t_route;

public:
  ReverseTwoOpt(const Input& input,
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
