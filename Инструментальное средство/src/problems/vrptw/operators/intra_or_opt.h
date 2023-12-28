#ifndef VRPTW_INTRA_OR_OPT_H
#define VRPTW_INTRA_OR_OPT_H

#include "problems/cvrp/operators/intra_or_opt.h"
#include "structures/EfOVRP/tw_route.h"

namespace EfOVRP {
namespace vrptw {

class IntraOrOpt : public cvrp::IntraOrOpt {
private:
  TWRoute& _tw_s_route;

public:
  IntraOrOpt(const Input& input,
             const utils::SolutionState& sol_state,
             TWRoute& tw_s_route,
             Index s_vehicle,
             Index s_rank,
             Index t_rank, // rank *after* removal.
             bool check_reverse);

  virtual bool is_valid() override;

  virtual void apply() override;

  virtual std::vector<Index> addition_candidates() const override;
};

} // namespace vrptw
} // namespace EfOVRP

#endif
