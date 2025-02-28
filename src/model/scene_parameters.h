#pragma once

#include <algorithm>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

namespace s21 {

/**
 * @class SceneParameters
 * @brief Manages the parameters that define the scene, including scale,
 * rotation, and location.
 *
 * The SceneParameters class provides a set of getter and setter methods to
 * access and modify the various parameters that define the scene. These
 * parameters include the scale, rotation, and location of the scene. The
 * setters include clamping logic to ensure the values remain within a valid
 * range.
 *
 * @note Rotation angles are stored in radians, but the setter methods accept
 * degrees for convenience.
 */
class SceneParameters {
 public:
  // Getters

  /**
   * @brief Returns the scale factor along the X-axis.
   * @return The scale factor along the X-axis, which is in the range
   * [0.0, 2.0].
   */
  inline float GetScaleX() const { return scale_x_; }

  /**
   * @brief Returns the scale factor along the Y-axis.
   * @return The scale factor along the Y-axis, which is in the range
   * [0.0, 2.0].
   */
  inline float GetScaleY() const { return scale_y_; }

  /**
   * @brief Returns the scale factor along the Z-axis.
   * @return The scale factor along the Z-axis, which is in the range
   * [0.0, 2.0].
   */
  inline float GetScaleZ() const { return scale_z_; }

  /**
   * @brief Returns the rotation angle around the X-axis.
   * @return The rotation angle in radians.
   */
  inline float GetRotationX() const { return rotation_x_; }

  /**
   * @brief Returns the rotation angle around the Y-axis.
   * @return The rotation angle in radians.
   */
  inline float GetRotationY() const { return rotation_y_; }

  /**
   * @brief Returns the rotation angle around the Z-axis.
   * @return The rotation angle in radians.
   */
  inline float GetRotationZ() const { return rotation_z_; }

  /**
   * @brief Returns the location along the X-axis.
   * @return The location along the X-axis, which is in the range [-1.0, 1.0].
   */
  inline float GetLocationX() const { return location_x_; }

  /**
   * @brief Returns the location along the Y-axis.
   * @return The location along the Y-axis, which is in the range [-1.0, 1.0].
   */
  inline float GetLocationY() const { return location_y_; }

  /**
   * @brief Returns the location along the Z-axis.
   * @return The location along the Z-axis, which is in the range [-1.0, 1.0].
   */
  inline float GetLocationZ() const { return location_z_; }

  // Setters with constraints

  /**
   * @brief Sets the scale factor along the X-axis.
   * @param value The desired scale factor. It will be clamped to the range
   * [0.0, 2.0].
   */
  inline void SetScaleX(float value) {
    scale_x_ = std::clamp(value, 0.0f, 2.0f);
  }

  /**
   * @brief Sets the scale factor along the Y-axis.
   * @param value The desired scale factor. It will be clamped to the range
   * [0.0, 2.0].
   */
  inline void SetScaleY(float value) {
    scale_y_ = std::clamp(value, 0.0f, 2.0f);
  }

  /**
   * @brief Sets the scale factor along the Z-axis.
   * @param value The desired scale factor. It will be clamped to the range
   * [0.0, 2.0].
   */
  inline void SetScaleZ(float value) {
    scale_z_ = std::clamp(value, 0.0f, 2.0f);
  }

  /**
   * @brief Sets the rotation angle around the X-axis.
   * @param degrees The rotation angle in degrees. It will be converted to
   * radians internally.
   */
  inline void SetRotationX(float degrees) {
    rotation_x_ = DegreesToRadians(degrees);
  }

  /**
   * @brief Sets the rotation angle around the Y-axis.
   * @param degrees The rotation angle in degrees. It will be converted to
   * radians internally.
   */
  inline void SetRotationY(float degrees) {
    rotation_y_ = DegreesToRadians(degrees);
  }

  /**
   * @brief Sets the rotation angle around the Z-axis.
   * @param degrees The rotation angle in degrees. It will be converted to
   * radians internally.
   */
  inline void SetRotationZ(float degrees) {
    rotation_z_ = DegreesToRadians(degrees);
  }

  /**
   * @brief Sets the location along the X-axis.
   * @param value The desired location. It will be clamped to the range
   * [-1.0, 1.0].
   */
  inline void SetLocationX(float value) {
    location_x_ = Clamp(value, -1.0f, 1.0f);
  }

  /**
   * @brief Sets the location along the Y-axis.
   * @param value The desired location. It will be clamped to the range
   * [-1.0, 1.0].
   */
  inline void SetLocationY(float value) {
    location_y_ = Clamp(value, -1.0f, 1.0f);
  }

  /**
   * @brief Sets the location along the Z-axis.
   * @param value The desired location. It will be clamped to the range
   * [-1.0, 1.0].
   */
  inline void SetLocationZ(float value) {
    location_z_ = Clamp(value, -1.0f, 1.0f);
  }

 private:
  /// The scale factor along the X-axis, initialized to 1.0.
  float scale_x_{1.0f};
  /// The scale factor along the Y-axis, initialized to 1.0.
  float scale_y_{1.0f};
  /// The scale factor along the Z-axis, initialized to 1.0.
  float scale_z_{1.0f};
  /// The rotation angle around the X-axis in radians, initialized to 0.0.
  float rotation_x_{0.0f};  // Stored in radians
  /// The rotation angle around the Y-axis in radians, initialized to 0.0.
  float rotation_y_{0.0f};  // Stored in radians
  /// The rotation angle around the Z-axis in radians, initialized to 0.0.
  float rotation_z_{0.0f};  // Stored in radians
  /// The location along the X-axis, initialized to 0.0.
  float location_x_{0.0f};
  /// The location along the Y-axis, initialized to 0.0.
  float location_y_{0.0f};
  /// The location along the Z-axis, initialized to 0.0.
  float location_z_{0.0f};

  // Helper functions

  /**
   * @brief Clamps a value between a minimum and maximum value.
   * @param value The value to clamp.
   * @param min The minimum value.
   * @param max The maximum value.
   * @return The clamped value.
   */
  static float Clamp(float value, float min, float max) {
    return std::clamp(value, min, max);
  }

  /**
   * @brief Converts degrees to radians.
   * @param degrees The angle in degrees.
   * @return The angle in radians.
   */
  static float DegreesToRadians(float degrees) {
    return degrees * static_cast<float>(M_PI) / 180.0f;
  }
};
}  // namespace s21