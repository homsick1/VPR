#ifndef SUMMARY_H
#define SUMMARY_H

/*






*/

#include "structures/EfOVRP/amount.h"
#include "structures/EfOVRP/solution/computing_times.h"

namespace EfOVRP {

struct Summary {
  Cost cost;
  const unsigned unassigned;
  Amount delivery;
  Amount pickup;
  Duration service;

  Duration duration;
  Duration waiting_time;
  Distance distance;
  ComputingTimes computing_times;

  Summary();

  Summary(unsigned unassigned, unsigned amount_size);
};

} // namespace EfOVRP

#endif
