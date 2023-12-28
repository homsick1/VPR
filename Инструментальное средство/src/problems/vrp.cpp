                                                                                                          /*






*/

#include "problems/vrp.h"
#include "structures/EfOVRP/input/input.h"

namespace EfOVRP {

VRP::VRP(const Input& input) : _input(input) {
  assert(_input.vehicles.size() > 0);
}

VRP::~VRP() {
}

} // namespace EfOVRP
