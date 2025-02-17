#pragma once

#include <vector>

#include "obj/obj_data.h"
#include "scene_mesh_data.h"
#include "scene_parameters.h"

namespace s21 {
// на отрисовку
   struct DrawSceneData {
        std::vector<float> vertices;
        std::vector<int> vertex_indices;
        std::string info;
    };


class Scene {
 public:
  void LoadSceneMeshData(OBJData obj_data);
  void TransformSceneMeshData(Mat4f& transform_matrix);
  DrawSceneData DrawScene();

 private:
  SceneMeshData scene_mesh_data_;

  //! будет ли использовано тут?
  //! Поступает все равно готовая матрица от контроллера
  SceneParameters scene_parameters_;
};
}  // namespace s21
