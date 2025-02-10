#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "figure/figure.h"
#include "scene_parameters.h"

namespace s21 {

class Scene {
 public:
  void AddFigure(Figure& figure) { figures_.push_back(figure); }
  std::vector<Figure>& GetFigures() { return figures_; }
  void TransformFigures(TransformMatrix& transform_mat) { (void)transform_mat; }

 private:
  std::vector<Figure> figures_;
  SceneParameters scene_parameters_;
};
}  // namespace s21
#endif  // SCENE_H
