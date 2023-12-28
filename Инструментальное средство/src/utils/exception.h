#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>

#include "structures/typedefs.h"

namespace EfOVRP {

class Exception : public std::exception {
public:
  const ERROR error;
  const std::string message;

  Exception(ERROR error, const std::string& message);
};

}

#endif
