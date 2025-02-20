#pragma once

#include <vector>

#include "obj/obj_data.h"
#include "scene_mesh_data.h"

namespace s21 {
// на отрисовку
struct DrawSceneData {
  std::vector<float> vertices;
  std::vector<int> vertex_indices;
  std::string info;
};

class Scene {
 public:
  DrawSceneData* LoadSceneMeshData(OBJData obj_data);
  void TransformSceneMeshData(Mat4f& transform_matrix);

 private:
  SceneMeshData scene_mesh_data_;
  DrawSceneData draw_scene_data_;
};
}  // namespace s21
