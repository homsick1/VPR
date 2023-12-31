/*






*/

#include "structures/EfOVRP/vehicle.h"
#include "utils/exception.h"

namespace EfOVRP {

Vehicle::Vehicle(Id id,
                 const boost::optional<Location>& start,
                 const boost::optional<Location>& end,
                 const Amount& capacity,
                 const Skills& skills,
                 const TimeWindow& tw)
  : id(id), start(start), end(end), capacity(capacity), skills(skills), tw(tw) {
  if (!static_cast<bool>(start) and !static_cast<bool>(end)) {
    throw Exception(ERROR::INPUT,
                    "No start or end specified for vehicle " +
                      std::to_string(id) + '.');
  }
}

bool Vehicle::has_start() const {
  return static_cast<bool>(start);
}

bool Vehicle::has_end() const {
  return static_cast<bool>(end);
}

bool Vehicle::has_same_locations(const Vehicle& other) const {
  bool same = (this->has_start() == other.has_start()) and
              (this->has_end() == other.has_end());

  if (same and this->has_start()) {
    same = this->start.get() == other.start.get();
  }

  if (same and this->has_end()) {
    same = this->end.get() == other.end.get();
  }

  return same;
}

} // namespace EfOVRP
