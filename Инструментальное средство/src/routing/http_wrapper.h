#ifndef HTTP_WRAPPER_H
#define HTTP_WRAPPER_H

#include "structures/typedefs.h"
#include "structures/EfOVRP/location.h"

namespace EfOVRP {
namespace routing {

class HttpWrapper {
private:
  std::string send_then_receive(const std::string& query) const;

  std::string ssl_send_then_receive(const std::string& query) const;

  static const std::string HTTPS_PORT;

protected:
  const Server _server;

  HttpWrapper(const Server& server);

  std::string run_query(const std::string& query) const;

  virtual std::string build_query(const std::vector<Location>& locations,
                                  std::string service,
                                  std::string extra_args) const = 0;
};

} // namespace routing
} // namespace EfOVRP

#endif
