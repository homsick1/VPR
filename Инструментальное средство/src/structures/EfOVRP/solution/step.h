#ifndef STEP_H
#define STEP_H

/*






*/

#include "structures/EfOVRP/job.h"
#include "structures/EfOVRP/location.h"

namespace EfOVRP {

struct Step {
  const STEP_TYPE step_type;
  const JOB_TYPE job_type;
  const Location location;
  const Id job;
  const Duration service;
  const Amount load;

  Duration arrival;
  Duration duration;
  Duration waiting_time;
  Distance distance;

  Step(STEP_TYPE type, Location location, const Amount& load);

  Step(const Job& job, const Amount& load);
};

} // namespace EfOVRP

#endif
