#ifndef COMPUTING_TIMES_H
#define COMPUTING_TIMES_H

#include "structures/typedefs.h"

namespace EfOVRP {

struct ComputingTimes {
  // Computing times in milliseconds.
  Duration loading;
  Duration solving;
  Duration routing;

  ComputingTimes();
};

} // namespace EfOVRP

#endif
