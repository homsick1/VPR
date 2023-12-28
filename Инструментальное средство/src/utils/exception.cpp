#include "utils/exception.h"

namespace EfOVRP {

Exception::Exception(ERROR error, const std::string& message)
  : error(error), message(message) {
}

}
