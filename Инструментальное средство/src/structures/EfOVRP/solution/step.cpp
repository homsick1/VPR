/*






*/

#include "structures/EfOVRP/solution/step.h"

namespace EfOVRP {

// Dummy initialization value for unused job id.
Step::Step(STEP_TYPE type, Location location, const Amount& load)
  : step_type(type),
    job_type(JOB_TYPE::SINGLE), // Dummy init.
    location(location),
    job(0),
    service(0),
    load(load),
    waiting_time(0) {
  assert(step_type == STEP_TYPE::START or step_type == STEP_TYPE::END);
}

Step::Step(const Job& job, const Amount& load)
  : step_type(STEP_TYPE::JOB),
    job_type(job.type),
    location(job.location),
    job(job.id),
    service(job.service),
    load(load),
    waiting_time(0) {
}

} // namespace EfOVRP
