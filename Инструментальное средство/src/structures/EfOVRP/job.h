#ifndef JOB_H
#define JOB_H

/*






*/

#include <vector>

#include "structures/typedefs.h"
#include "structures/EfOVRP/amount.h"
#include "structures/EfOVRP/location.h"
#include "structures/EfOVRP/time_window.h"

namespace EfOVRP {

struct Job {
  Location location;
  const Id id;
  const JOB_TYPE type;
  const Duration service;
  const Amount delivery;
  const Amount pickup;
  const Skills skills;
  const Priority priority;
  const std::vector<TimeWindow> tws;
  const Duration tw_length;

  // Constructor for regular one-stop job (JOB_TYPE::SINGLE).
  Job(Id id,
      const Location& location,
      Duration service = 0,
      const Amount& delivery = Amount(0),
      const Amount& pickup = Amount(0),
      const Skills& skills = Skills(),
      Priority priority = 0,
      const std::vector<TimeWindow>& tws =
        std::vector<TimeWindow>(1, TimeWindow()));

  // Constructor for pickup and delivery jobs (JOB_TYPE::PICKUP or
  // JOB_TYPE::DELIVERY).
  Job(Id id,
      JOB_TYPE type,
      const Location& location,
      Duration service = 0,
      const Amount& amount = Amount(0),
      const Skills& skills = Skills(),
      Priority priority = 0,
      const std::vector<TimeWindow>& tws =
        std::vector<TimeWindow>(1, TimeWindow()));

  Index index() const {
    return location.index();
  }

  bool is_valid_start(Duration time) const;

  void check_tws() const;
};

} // namespace EfOVRP

#endif
