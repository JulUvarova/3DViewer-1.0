#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "scene_object.h"
#include "../math/transform_matrix.h"

namespace s21 {

class SceneObject {
 public:
  virtual void Transform(TransformMatrix& transform_mat) = 0;
};
}  // namespace s21
#endif  // SCENEOBJECT_H
