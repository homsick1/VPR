/*






*/

#include "algorithms/local_search/operator.h"

namespace EfOVRP {
namespace ls {

Gain Operator::gain() {
  if (!gain_computed) {
    this->compute_gain();
  }
  return stored_gain;
}

} // namespace ls
} // namespace EfOVRP
