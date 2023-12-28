#ifndef MUNKRES_H
#define MUNKRES_H

/*






*/

#include <unordered_map>

#include "structures/generic/matrix.h"
#include "structures/typedefs.h"

namespace EfOVRP {
namespace utils {

template <class T>
std::unordered_map<Index, Index>
minimum_weight_perfect_matching(const Matrix<T>& m);

template <class T>
std::unordered_map<Index, Index>
greedy_symmetric_approx_mwpm(const Matrix<T>& m);

} // namespace utils
} // namespace EfOVRP

#endif
