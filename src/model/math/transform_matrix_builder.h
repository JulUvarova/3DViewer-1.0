#ifndef TRANSFORMMATRIXBUILDER_H
#define TRANSFORMMATRIXBUILDER_H

#include "transform_matrix.h"

namespace s21 {

class TransformMatrixBuilder {
 public: 
  static TransformMatrix CreateRotationMatrix(float x, float y, float z) {}
  static TransformMatrix CreateMoveMatrix(float x, float y, float z) {}
  static TransformMatrix CreateScaleMatrix(float x, float y, float z) {}
  static TransformMatrix CreateRotationMatrix(float x, float y, float z) {}

 private:
  TransformMatrixBuilder() = delete;
};
}  // namespace s21
#endif  // TRANSFORMMATRIXBUILDER_H
