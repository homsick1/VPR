/*






*/

#include "structures/EfOVRP/solution/summary.h"

namespace EfOVRP {

Summary::Summary() : cost(0), unassigned(0), service(0) {
}

Summary::Summary(unsigned unassigned, unsigned amount_size)
  : cost(0),
    unassigned(unassigned),
    delivery(amount_size),
    pickup(amount_size),
    service(0),
    duration(0),
    waiting_time(0),
    distance(0) {
}

} // namespace EfOVRP
