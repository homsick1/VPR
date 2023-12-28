#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H

#include "structures/EfOVRP/input/input.h"

namespace EfOVRP {
namespace io {

struct CLArgs;

Input parse(const CLArgs& cl_args);

} // namespace io
} // namespace EfOVRP

#endif
