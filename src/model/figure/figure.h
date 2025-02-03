#ifndef FIGURE_H
#define FIGURE_H

#include <vector>

#include "../math/transform_matrix.h"
#include "edge.h"
#include "scene_object.h"
#include "vertex.h"

namespace s21 {

// одна плоскость (один треугольник)
class Figure : public SceneObject {
 public:
  Figure(std::vector<Vertex> v, std::vector<Edge> e)
      : vertexes_(v), edges_(e) {};

  inline std::vector<Vertex>& GetVertexes() noexcept { return vertexes_; }
  inline std::vector<Edge>& GetEdges() noexcept { return edges_; }
  void Transform(TransformMatrix& transform_mat) override {
    (void)transform_mat;
  }

 private:
  std::vector<Vertex> vertexes_;
  std::vector<Edge> edges_;
};
}  // namespace s21
#endif  // FIGURE_H
