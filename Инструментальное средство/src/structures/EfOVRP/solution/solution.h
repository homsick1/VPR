#ifndef SOLUTION_H
#define SOLUTION_H

/*






*/

#include <string>

#include "structures/EfOVRP/job.h"
#include "structures/EfOVRP/solution/route.h"
#include "structures/EfOVRP/solution/summary.h"

namespace EfOVRP {

struct Solution {
  unsigned code;
  std::string error;
  Summary summary;
  std::vector<Route> routes;
  std::vector<Job> unassigned;

  Solution(unsigned code, std::string error);

  Solution(unsigned code,
           unsigned amount_size,
           std::vector<Route>&& routes,
           std::vector<Job>&& unassigned);
};

} // namespace EfOVRP

#endif
