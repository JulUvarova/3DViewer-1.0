#pragma once

#include <vector>
#include "scene_mesh_data.h"
#include "scene_parameters.h"
#include "obj/obj_data.h"

namespace s21 {

class Scene {
 public:
  void LoadSceneMeshData(OBJData& obj_data);
  void TransformSceneMeshData(Mat4f& transform_matrix);
 private:
  SceneMeshData scene_mesh_data_;
  SceneParameters scene_parameters_;
};
}  // namespace s21
