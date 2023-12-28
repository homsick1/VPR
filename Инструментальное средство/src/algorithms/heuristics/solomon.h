#ifndef SOLOMON_H
#define SOLOMON_H

/*






*/

#include "structures/EfOVRP/input/input.h"

namespace EfOVRP {
namespace heuristics {

// Implementation of a variant of the Solomon I1 heuristic.
template <class T> T basic(const Input& input, INIT init, float lambda);

// Adjusting the above for situation with heterogeneous fleet.
template <class T>
T dynamic_vehicle_choice(const Input& input, INIT init, float lambda);

} // namespace heuristics
} // namespace EfOVRP

#endif
