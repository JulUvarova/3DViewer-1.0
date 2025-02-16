#pragma once

#include <vector>

#include "obj/obj_data.h"
#include "scene_mesh_data.h"
#include "scene_parameters.h"

namespace s21 {

class Scene {
 public:
  explicit Scene(OBJData& obj_data) { LoadSceneMeshData(obj_data); }

  void LoadSceneMeshData(OBJData& obj_data);
  void TransformSceneMeshData(Mat4f& transform_matrix);
  inline SceneMeshData* GetSceneMeshData() { return &scene_mesh_data_; }

 private:
  SceneMeshData scene_mesh_data_;

  //! будет ли использовано тут?
  //! Поступает все равно готовая матрица от контроллера
  SceneParameters scene_parameters_;
};
}  // namespace s21
