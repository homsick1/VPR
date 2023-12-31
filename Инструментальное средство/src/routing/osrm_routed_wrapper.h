#ifndef OSRM_ROUTED_WRAPPER_H
#define OSRM_ROUTED_WRAPPER_H

/*






*/

#include "routing/http_wrapper.h"
#include "routing/routing_wrapper.h"

namespace EfOVRP {
namespace routing {

class OsrmRoutedWrapper : public RoutingWrapper, HttpWrapper {

private:
  virtual std::string build_query(const std::vector<Location>& locations,
                                  std::string service,
                                  std::string extra_args) const override;

public:
  OsrmRoutedWrapper(const std::string& profile, const Server& server);

  virtual Matrix<Cost>
  get_matrix(const std::vector<Location>& locs) const override;

  virtual void add_route_info(Route& route) const override;
};

} // namespace routing
} // namespace EfOVRP

#endif
