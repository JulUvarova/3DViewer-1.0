#include "scene.h"

namespace s21 {
void Scene::LoadSceneMeshData(OBJData& obj_data) {
  scene_mesh_data_.vertexes.assign(obj_data.vertices.begin(),
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
}
void Scene::TransformSceneMeshData(Mat4f& transform_matrix) {
  for (auto& vertex : scene_mesh_data_.vertexes) {
    vertex = vertex * transform_matrix;
  }}
}  // namespace s21