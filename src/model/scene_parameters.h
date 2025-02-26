#pragma once

#include <algorithm>  // for std::clamp

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

namespace s21 {

class SceneParameters {
 public:
  // Getters
  float GetScaleX() const { return scale_x_; }
  float GetScaleY() const { return scale_y_; }
  float GetScaleZ() const { return scale_z_; }

  float GetRotationX() const { return rotation_x_; }
  float GetRotationY() const { return rotation_y_; }
  float GetRotationZ() const { return rotation_z_; }

  float GetLocationX() const { return location_x_; }
  float GetLocationY() const { return location_y_; }
  float GetLocationZ() const { return location_z_; }

  // Setters with constraints
  void SetScaleX(float value) { scale_x_ = std::clamp(value, 0.0f, 2.0f); }
  void SetScaleY(float value) { scale_y_ = std::clamp(value, 0.0f, 2.0f); }
  void SetScaleZ(float value) { scale_z_ = std::clamp(value, 0.0f, 2.0f); }

  void SetRotationX(float degrees) { rotation_x_ = DegreesToRadians(degrees); }
  void SetRotationY(float degrees) { rotation_y_ = DegreesToRadians(degrees); }
  void SetRotationZ(float degrees) { rotation_z_ = DegreesToRadians(degrees); }

  void SetLocationX(float value) { location_x_ = Clamp(value, -1.0f, 1.0f); }
  void SetLocationY(float value) { location_y_ = Clamp(value, -1.0f, 1.0f); }
  void SetLocationZ(float value) { location_z_ = Clamp(value, -1.0f, 1.0f); }

 private:
  float scale_x_{1.0f};
  float scale_y_{1.0f};
  float scale_z_{1.0f};
  float rotation_x_{0.0f};  // Stored in radians
  float rotation_y_{0.0f};  // Stored in radians
  float rotation_z_{0.0f};  // Stored in radians
  float location_x_{0.0f};
  float location_y_{0.0f};
  float location_z_{0.0f};

  // Helper functions
  static float Clamp(float value, float min, float max) {
    return std::clamp(value, min, max);
  }

  static float DegreesToRadians(float degrees) {
    return degrees * static_cast<float>(M_PI) / 180.0f;
  }
};
}  // namespace s21
