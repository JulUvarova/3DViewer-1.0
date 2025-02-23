#include "scene.h"

namespace s21 {
DrawSceneData* Scene::LoadSceneMeshData(OBJData obj_data) {
  // initial point coordinates
  mesh_vertexes_.assign(obj_data.vertices.begin(), obj_data.vertices.end());

  // vertices for render
  draw_scene_data_.vertices.reserve(mesh_vertexes_.size());

  for (auto& [x, y, z, w] : mesh_vertexes_) {
    draw_scene_data_.vertices.push_back(x);
    draw_scene_data_.vertices.push_back(y);
    draw_scene_data_.vertices.push_back(z);
  }

  // indices for render edges
  for (const auto& object : obj_data.objects) {
    for (const auto& mesh : object.meshes) {
      for (const auto& face : mesh.faces) {
        size_t num_vertices = face.vertices.size();
        if (num_vertices < 2) continue;  // skip invalid faces (< 2 vertices)
        for (size_t i = 0; i < num_vertices; ++i) {
          draw_scene_data_.vertex_indices.push_back(face.vertices[i].v);
          draw_scene_data_.vertex_indices.push_back(
              face.vertices[(i + 1) % num_vertices].v);
        }
      }
    }
  }
  // create data for drawing
  draw_scene_data_.info = obj_data.toString();

  return &draw_scene_data_;
}

void Scene::TransformSceneMeshData(Mat4f& transform_matrix) {
  const size_t vertexCount = mesh_vertexes_.size();

  // Use TBB's parallel_for to divide the work among available threads.
  tbb::parallel_for(tbb::blocked_range<size_t>(0, vertexCount),
                    [&](const tbb::blocked_range<size_t>& range) {
                      for (size_t i = range.begin(); i < range.end(); ++i) {
                        // Apply transformation to the vertex.
                        auto [x, y, z, w] =
                            mesh_vertexes_[i] * transform_matrix;
                        // Update the vertex in the vertexes_ vector.
                        draw_scene_data_.vertices[i * 3] = x;
                        draw_scene_data_.vertices[i * 3 + 1] = y;
                        draw_scene_data_.vertices[i * 3 + 2] = z;
                      }
                    });
}

}  // namespace s21