#ifndef VRPTW_INTRA_RELOCATE_H
#define VRPTW_INTRA_RELOCATE_H

/*






*/

#include "problems/cvrp/operators/intra_relocate.h"
#include "structures/EfOVRP/tw_route.h"

namespace EfOVRP {
namespace vrptw {

class IntraRelocate : public cvrp::IntraRelocate {
private:
  TWRoute& _tw_s_route;

public:
  IntraRelocate(const Input& input,
                const utils::SolutionState& sol_state,
                TWRoute& tw_s_route,
                Index s_vehicle,
                Index s_rank,
                Index t_rank); // relocate rank *after* removal.

  virtual bool is_valid() override;

  virtual void apply() override;

  virtual std::vector<Index> addition_candidates() const override;
};

} // namespace vrptw
} // namespace EfOVRP

#endif
