#ifndef VEHICLE_H
#define VEHICLE_H

/*






*/

#include "structures/typedefs.h"
#include "structures/EfOVRP/amount.h"
#include "structures/EfOVRP/location.h"
#include "structures/EfOVRP/time_window.h"

namespace EfOVRP {

struct Vehicle {
  const Id id;
  boost::optional<Location> start;
  boost::optional<Location> end;
  const Amount capacity;
  const Skills skills;
  const TimeWindow tw;

  Vehicle(Id id,
          const boost::optional<Location>& start,
          const boost::optional<Location>& end,
          const Amount& capacity = Amount(0),
          const Skills& skills = Skills(),
          const TimeWindow& tw = TimeWindow());

  bool has_start() const;

  bool has_end() const;

  bool has_same_locations(const Vehicle& other) const;
};

} // namespace EfOVRP

#endif
