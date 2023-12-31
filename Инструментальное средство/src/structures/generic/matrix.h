#ifndef MATRIX_H
#define MATRIX_H

/*






*/

#include <initializer_list>
#include <vector>

#include "structures/typedefs.h"

namespace EfOVRP {

template <class T> class Line : private std::vector<T> {

  using parent = std::vector<T>;

public:
  using parent::size;
  using parent::operator[];

  Line(std::size_t n);

  Line(std::initializer_list<T> l);
};

template <class T> class Matrix : private std::vector<Line<T>> {

  using parent = std::vector<Line<T>>;

public:
  using parent::size;
  using parent::operator[];

  Matrix();

  Matrix(std::size_t n);

  Matrix(std::initializer_list<Line<T>> l);

  Matrix<T> get_sub_matrix(const std::vector<Index>& indices) const;
};

} // namespace EfOVRP

#endif
