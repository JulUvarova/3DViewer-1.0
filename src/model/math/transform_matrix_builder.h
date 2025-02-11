#ifndef TRANSFORMMATRIXBUILDER_H
#define TRANSFORMMATRIXBUILDER_H

#include "../data_structures.h"
#include "transform_matrix.h"

namespace s21 {

class TransformMatrixBuilder {
public:
  static Mat4f TransformMatrixBuilder::CreateRotationMatrix(float x_rot,
                                                            float y_rot,
                                                            float z_rot) {
    const float cosX = std::cos(x_rot);
    const float sinX = std::sin(x_rot);
    const Mat4f Rx = {
        {{1, 0, 0, 0}, {0, cosX, -sinX, 0}, {0, sinX, cosX, 0}, {0, 0, 0, 1}}};

    const float cosY = std::cos(y_rot);
    const float sinY = std::sin(y_rot);
    const Mat4f Ry = {
        {{cosY, 0, sinY, 0}, {0, 1, 0, 0}, {-sinY, 0, cosY, 0}, {0, 0, 0, 1}}};

    const float cosZ = std::cos(z_rot);
    const float sinZ = std::sin(z_rot);
    const Mat4f Rz = {
        {{cosZ, -sinZ, 0, 0}, {sinZ, cosZ, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};

    return Rz * Ry * Rx;
  }

  static Mat4f TransformMatrixBuilder::CreateMoveMatrix(float x_tr, float y_tr,
                                                        float z_tr) {
    return {{{1.0f, 0.0f, 0.0f, x_tr},
             {0.0f, 1.0f, 0.0f, y_tr},
             {0.0f, 0.0f, 1.0f, z_tr},
             {0.0f, 0.0f, 0.0f, 1.0f}}};
  }
  static Mat4f TransformMatrixBuilder::CreateScaleMatrix(float x_scale,
                                                         float y_scale,
                                                         float z_scale) {
    return {{{x_scale, 0.0f, 0.0f, 0.0f},
             {0.0f, y_scale, 0.0f, 0.0f},
             {0.0f, 0.0f, z_scale, 0.0f},
             {0.0f, 0.0f, 0.0f, 1.0f}}};
  }

private:
  TransformMatrixBuilder() = delete;
};
} // namespace s21
#endif // TRANSFORMMATRIXBUILDER_H
