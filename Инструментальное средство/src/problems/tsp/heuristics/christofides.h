#ifndef CHRISTOFIDES_H
#define CHRISTOFIDES_H

#include <list>

#include "structures/generic/matrix.h"
#include "structures/typedefs.h"

namespace EfOVRP {
namespace tsp {

// Implementing a variant of the Christofides heuristic.
std::list<Index> christofides(const Matrix<Cost>& sym_matrix);

} // namespace tsp
} // namespace EfOVRP

#endif
