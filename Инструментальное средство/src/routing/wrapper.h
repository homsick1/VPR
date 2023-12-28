#ifndef WRAPPER_H
#define WRAPPER_H

/*






*/

#include <vector>

#include "structures/generic/matrix.h"
#include "structures/EfOVRP/location.h"
#include "structures/EfOVRP/solution/route.h"

namespace EfOVRP {
namespace routing {

template <class T> class Wrapper {

public:
  std::string _profile;

  virtual Matrix<T> get_matrix(const std::vector<Location>& locs) const = 0;

  virtual void add_route_info(Route& route) const = 0;

  virtual ~Wrapper() {
  }

protected:
  Wrapper(const std::string& profile) : _profile(profile) {
  }
};

} // namespace routing
} // namespace EfOVRP

#endif
