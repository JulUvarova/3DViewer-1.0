#pragma once

#include <algorithm>  // for std::clamp
#include <cmath>      // for M_PI and degrees to radians conversion

namespace s21 {

// enum class ProjectionTypes {
//   kParallel,
//   kCentral,
// };

// enum class VertexStyle { kNone, kSquare, kCircle };
// enum class EdgeStyle { kNone, kLine, kDashed };

// struct Color {
//   float r{0.0f};
//   float g{0.0f};
//   float b{0.0f};
// };

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
  // ProjectionTypes GetProjectionType() const { return projection_type_; }

  // Color GetBackgroundColor() const { return background_color_; }

  // float GetAspectRate() const { return aspect_rate_; }

  // VertexStyle GetVertexStyle() const { return vertex_style_; }
  // int GetVertexSize() const { return vertex_size_; }
  // Color GetVertexColor() const { return vertex_color_; }

  // EdgeStyle GetEdgeStyle() const { return edge_style_; }
  // int GetEdgeSize() const { return edge_size_; }
  // Color GetEdgeColor() const { return edge_color_; }

  // void SetProjectionType(ProjectionTypes type) { projection_type_ = type; }

  // void SetBackgroundColor(Color color) { background_color_ = color; }

  // void SetAspectRate(float value) { aspect_rate_ = value; }

  // void SetVertexStyle(VertexStyle style) { vertex_style_ = style; }
  // void SetVertexSize(int size) { vertex_size_ = size; }
  // void SetVertexColor(Color color) { vertex_color_ = color; }

  // void SetEdgeStyle(EdgeStyle style) { edge_style_ = style; }
  // void SetEdgeSize(int size) { edge_size_ = size; }
  // void SetEdgeColor(Color color) { edge_color_ = color; }

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
  // ProjectionTypes projection_type_;
  // VertexStyle vertex_style_;
  // int vertex_size_;
  // Color vertex_color_;
  // EdgeStyle edge_style_;
  // int edge_size_;
  // Color edge_color_;
  // Color background_color_;
  // float aspect_rate_{1.0f};

  // Helper functions
  static float Clamp(float value, float min, float max) {
    return std::clamp(value, min, max);
  }

  static float DegreesToRadians(float degrees) {
    return degrees * static_cast<float>(M_PI) / 180.0f;
  }
};
}  // namespace s21
