#pragma once

#include "data_structures.h"

/**
 * @namespace s21
 * @brief Contains utilities for building transformation matrices and related
 * data structures.
 */
namespace s21 {

/**
 * @class TransformMatrixBuilder
 * @brief A utility class for creating 4x4 transformation matrices.
 *
 * This class provides static methods to generate 4x4 matrices for rotation,
 * translation, and scaling transformations in 3D space. It is designed as a
 * static utility class and cannot be instantiated.
 *
 * @note All methods are static, and the constructor is private to prevent
 * instantiation.
 */
class TransformMatrixBuilder {
 public:
  /**
   * @brief Creates a 4x4 rotation matrix for combined rotations around the X,
   * Y, and Z axes.
   * @param x_rot Rotation angle around the X-axis in radians.
   * @param y_rot Rotation angle around the Y-axis in radians.
   * @param z_rot Rotation angle around the Z-axis in radians.
   * @return A Mat4f representing the combined rotation matrix (Rz * Ry * Rx).
   *
   * This method constructs individual rotation matrices for each axis (X, Y, Z)
   * and multiplies them in the order Z-Y-X to produce a single combined
   * rotation matrix. The angles are expected in radians.
   */
  static Mat4f CreateRotationMatrix(float x_rot, float y_rot, float z_rot) {
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

  /**
   * @brief Creates a 4x4 translation matrix.
   * @param x_tr Translation along the X-axis.
   * @param y_tr Translation along the Y-axis.
   * @param z_tr Translation along the Z-axis.
   * @return A Mat4f representing the translation matrix.
   *
   * This method constructs a 4x4 matrix that translates points in 3D space by
   * the specified amounts along the X, Y, and Z axes.
   */
  static Mat4f CreateMoveMatrix(float x_tr, float y_tr, float z_tr) {
    return {{{1.0f, 0.0f, 0.0f, x_tr},
             {0.0f, 1.0f, 0.0f, y_tr},
             {0.0f, 0.0f, 1.0f, z_tr},
             {0.0f, 0.0f, 0.0f, 1.0f}}};
  }

  /**
   * @brief Creates a 4x4 scaling matrix.
   * @param x_scale Scaling factor along the X-axis.
   * @param y_scale Scaling factor along the Y-axis.
   * @param z_scale Scaling factor along the Z-axis.
   * @return A Mat4f representing the scaling matrix.
   *
   * This method constructs a 4x4 matrix that scales points in 3D space by the
   * specified factors along the X, Y, and Z axes.
   */
  static Mat4f CreateScaleMatrix(float x_scale, float y_scale, float z_scale) {
    return {{{x_scale, 0.0f, 0.0f, 0.0f},
             {0.0f, y_scale, 0.0f, 0.0f},
             {0.0f, 0.0f, z_scale, 0.0f},
             {0.0f, 0.0f, 0.0f, 1.0f}}};
  }

 private:
  /**
   * @brief Private constructor to prevent instantiation.
   *
   * This class is intended as a static utility, so instantiation is disabled.
   */
  TransformMatrixBuilder() = delete;
};
}  // namespace s21