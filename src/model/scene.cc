#include "scene.h"

namespace s21 {
DrawSceneData* Scene::LoadSceneMeshData(OBJData obj_data) {
  // Store initial mesh vetrex coordinates
  mesh_vertexes_.assign(obj_data.vertices.begin(), obj_data.vertices.end());

  // Vertices for render
  draw_scene_data_.vertices.reserve(mesh_vertexes_.size() * 3);

  // Flatten vertices into x,y,z components
  for (const auto& [x, y, z, w] : mesh_vertexes_) {
    draw_scene_data_.vertices.insert(draw_scene_data_.vertices.end(),
                                     {x, y, z});
  }

  // Process mesh faces and edges using range-based iteration
  auto processFace = [this](const Face& face) {
    size_t vertices_size = face.vertices.size();
    if (vertices_size >= 2) {
      for (size_t i = 0; i < vertices_size; ++i) {
        draw_scene_data_.vertex_indices.push_back(face.vertices[i].v);
        draw_scene_data_.vertex_indices.push_back(
            face.vertices[(i + 1) % vertices_size].v);
      }
    }
  };

  for (const auto& object : obj_data.objects) {
    for (const auto& mesh : object.meshes) {
      std::for_each(mesh.faces.begin(), mesh.faces.end(), processFace);
    }
  }

  draw_scene_data_.info = obj_data.toString();

  return &draw_scene_data_;
}

void Scene::TransformSceneMeshData(Mat4f& transform_matrix) {
  const size_t vertexCount = mesh_vertexes_.size();

  LogInfoOnce << "First transform..." << std::endl;
  size_t num_threads = std::thread::hardware_concurrency();  // Auto-detect
  num_threads = (num_threads == 0) ? 1 : num_threads;        // Handle failure

  LogInfoOnce << "Threads count: " << num_threads << std::endl;
  const size_t chunk_size = (vertexCount + num_threads - 1) / num_threads;

  std::vector<std::thread> threads;
  threads.reserve(num_threads);

  // Create and launch threads
  for (size_t i = 0; i < num_threads; ++i) {
    const size_t start = i * chunk_size;
    const size_t end = std::min(start + chunk_size, vertexCount);

    // Skip empty chunks if vertexCount < num_threads
    if (start >= end) break;

    threads.emplace_back([this, &transform_matrix, start, end] {
      for (size_t j = start; j < end; ++j) {
        auto [x, y, z, w] = mesh_vertexes_[j] * transform_matrix;
        draw_scene_data_.vertices[j * 3] = x;
        draw_scene_data_.vertices[j * 3 + 1] = y;
        draw_scene_data_.vertices[j * 3 + 2] = z;
      }
    });
  }
  // Wait for all threads to complete
  for (auto& thread : threads) {
    thread.join();
  }
}
}  // namespace s21