#ifndef TRANSFORMMATRIX_H
#define TRANSFORMMATRIX_H

#include "../figure/point.h"

namespace s21 {

class TransformMatrix {
 public:
  TransformMatrix operator*(const TransformMatrix& other) {
    // создаем результирующую матрицу
    (void)other;
    return TransformMatrix();
  }

  void TransformPoint(Point& point) { (void)point; }

 private:
  float matrix_[4][4]{0};
};
}  // namespace s21
#endif  // TRANSFORMMATRIX_H
