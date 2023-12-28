#ifndef ROUTE_H
#define ROUTE_H

/*






*/

#include <string>
#include <vector>

#include "structures/EfOVRP/solution/step.h"

namespace EfOVRP {

struct Route {
  const Id vehicle;
  std::vector<Step> steps;
  const Cost cost;
  const Duration service;
  const Duration duration;
  const Duration waiting_time;
  const Amount delivery;
  const Amount pickup;

  std::string geometry;
  Distance distance;

  Route(Id vehicle,
        std::vector<Step>&& steps,
        Cost cost,
        Duration service,
        Duration duration,
        Duration waiting_time,
        const Amount& delivery,
        const Amount& pickup);
};

} // namespace EfOVRP

#endif
