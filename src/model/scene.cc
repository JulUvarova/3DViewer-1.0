#include "scene.h"

namespace s21 {
void Scene::LoadSceneMeshData(OBJData obj_data) {
  // initial point coordinates
  scene_mesh_data_.vertexes.assign(obj_data.vertices.begin(),
                                   obj_data.vertices.end());
  // transformed point coordinates
  scene_mesh_data_.transformed_vertexes.assign(obj_data.vertices.begin(),
                                               obj_data.vertices.end());

  for (const auto& object : obj_data.objects) {
    for (const auto& mesh : object.meshes) {
      for (const auto& face : mesh.faces) {
        std::vector<int> face_vertices{};
        face_vertices.reserve(face.vertices.size());
        for (const auto& vertex : face.vertices) {
          face_vertices.push_back(vertex.v);
        }
        scene_mesh_data_.face_vertex_indices.push_back(face_vertices);
      }
    }
  }
  scene_mesh_data_.obj_info = obj_data.toString();
}

DrawSceneData Scene::DrawScene() {
  DrawSceneData scene;
  scene.vertices.reserve(scene_mesh_data_.transformed_vertexes.size());

  for (auto& point : scene_mesh_data_.transformed_vertexes) {
    scene.vertices.push_back(point.x);
    scene.vertices.push_back(point.y);
    scene.vertices.push_back(point.z);
  }

  for (auto& face : scene_mesh_data_.face_vertex_indices) {
    for (size_t i = 0; i < face.size(); ++i) {
      scene.vertex_indices.push_back(face[i]);
      if (i == face.size() - 1)
        scene.vertex_indices.push_back(face[0]);
      else
        scene.vertex_indices.push_back(face[i + 1]);
    }
  }
  scene.info = scene_mesh_data_.obj_info;
  return scene;
}

void Scene::TransformSceneMeshData(Mat4f& transform_matrix) {
  size_t i = 0;
  for (auto& vertex : scene_mesh_data_.vertexes) {
    scene_mesh_data_.transformed_vertexes[i] = vertex * transform_matrix;
    ++i;
  }
}
}  // namespace s21