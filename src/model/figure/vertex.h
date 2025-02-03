#ifndef VERTEX_H
#define VERTEX_H

#include "point.h"
#include "scene_object.h"

namespace s21 {

class Vertex : public SceneObject {
 public:
  Vertex(Point point) : position_(point) {};

  inline const Point& GetPosition() noexcept { return position_; }

 void Transform(TransformMatrix& transform_mat) override {
    (void)transform_mat;
  }

 private:
  Point position_;
};
}  // namespace s21
#endif  // VERTEX_H
