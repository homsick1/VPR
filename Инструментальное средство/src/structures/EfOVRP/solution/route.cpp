/*






*/

#include "structures/EfOVRP/solution/route.h"

namespace EfOVRP {

Route::Route(Id vehicle,
             std::vector<Step>&& steps,
             Cost cost,
             Duration service,
             Duration duration,
             Duration waiting_time,
             const Amount& delivery,
             const Amount& pickup)
  : vehicle(vehicle),
    steps(std::move(steps)),
    cost(cost),
    service(service),
    duration(duration),
    waiting_time(waiting_time),
    delivery(delivery),
    pickup(pickup),
    distance(0) {
}

} // namespace EfOVRP
