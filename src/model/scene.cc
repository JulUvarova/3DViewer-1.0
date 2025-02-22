#include "scene.h"

namespace s21 {
DrawSceneData* Scene::LoadSceneMeshData(OBJData obj_data) {
  // initial point coordinates
  scene_mesh_data_.vertexes.assign(obj_data.vertices.begin(),
                                   obj_data.vertices.end());
  // transformed point coordinates
  // scene_mesh_data_.transformed_vertexes.assign(obj_data.vertices.begin(),
  //                                              obj_data.vertices.end());
  // vertices for render
  draw_scene_data_.vertices.reserve(scene_mesh_data_.vertexes.size());

  for (auto& point : scene_mesh_data_.vertexes) {
    draw_scene_data_.vertices.push_back(point.x);
    draw_scene_data_.vertices.push_back(point.y);
    draw_scene_data_.vertices.push_back(point.z);
  }

  // edges collections with looping on the first element
  for (const auto& object : obj_data.objects) {
    for (const auto& mesh : object.meshes) {
      for (const auto& face : mesh.faces) {
        std::vector<int> face_vertices{};
        face_vertices.reserve(face.vertices.size());
        for (const auto& vertex : face.vertices) {
          face_vertices.push_back(vertex.v);
        }
        //! Оно тут нужно? Не меняется ведь
        // Edges for meshData
        scene_mesh_data_.face_vertex_indices.push_back(face_vertices);
      }
    }
  }
  // Edges for render with looping on the first element
  for (auto& face : scene_mesh_data_.face_vertex_indices) {
    for (size_t i = 0; i < face.size(); ++i) {
      draw_scene_data_.vertex_indices.push_back(face[i]);
      if (i == (face.size() - 1))
        draw_scene_data_.vertex_indices.push_back(face[0]);
      else
        draw_scene_data_.vertex_indices.push_back(face[i + 1]);
    }
  }
  // create data for drawing
  draw_scene_data_.info = obj_data.toString();

  return &draw_scene_data_;
}

void Scene::TransformSceneMeshData(Mat4f& transform_matrix) {
  // Determine the total number of vertices.
  const size_t vertexCount = scene_mesh_data_.vertexes.size();

  // Use TBB's parallel_for to divide the work among available threads.
  tbb::parallel_for(tbb::blocked_range<size_t>(0, vertexCount),
                    [&](const tbb::blocked_range<size_t>& range) {
                      for (size_t i = range.begin(); i < range.end(); ++i) {
                        // Apply transformation to the vertex.
                        auto [x, y, z, w] =
                            scene_mesh_data_.vertexes[i] * transform_matrix;

                        // Write the transformed vertex coordinates into
                        // draw_scene_data_.
                        draw_scene_data_.vertices[i * 3] = x;
                        draw_scene_data_.vertices[i * 3 + 1] = y;
                        draw_scene_data_.vertices[i * 3 + 2] = z;
                      }
                    });
}
// DrawSceneData& Scene::DrawScene() {
//   draw_scene_data_.vertices.reserve(
//       scene_mesh_data_.transformed_vertexes.size());

//   for (auto& point : scene_mesh_data_.transformed_vertexes) {
//     draw_scene_data_.vertices.push_back(point.x);
//     draw_scene_data_.vertices.push_back(point.y);
//     draw_scene_data_.vertices.push_back(point.z);
//   }

//   for (auto& face : scene_mesh_data_.face_vertex_indices) {
//     for (size_t i = 0; i < face.size(); ++i) {
//       draw_scene_data_.vertex_indices.push_back(face[i]);
//       if (i == face.size() - 1)
//         scene.vertex_indices.push_back(face[0]);
//       else
//         scene.vertex_indices.push_back(face[i + 1]);
//     }
//   }
//   scene.info = scene_mesh_data_.obj_info;
//   return scene;
// }
}  // namespace s21