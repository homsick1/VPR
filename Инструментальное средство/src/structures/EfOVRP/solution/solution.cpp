/*






*/

#include "structures/EfOVRP/solution/solution.h"

namespace EfOVRP {

Solution::Solution(unsigned code, std::string error)
  : code(code), error(error) {
}

Solution::Solution(unsigned code,
                   unsigned amount_size,
                   std::vector<Route>&& routes,
                   std::vector<Job>&& unassigned)
  : code(code),
    summary(unassigned.size(), amount_size),
    routes(std::move(routes)),
    unassigned(std::move(unassigned)) {

  for (const auto& route : this->routes) {
    summary.cost += route.cost;
    summary.delivery += route.delivery;
    summary.pickup += route.pickup;
    summary.service += route.service;
    summary.duration += route.duration;
    summary.waiting_time += route.waiting_time;
  }
}

} // namespace EfOVRP
