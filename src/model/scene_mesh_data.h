#pragma once

#include <vector>
#include "data_structures.h"
namespace s21 {
    struct SceneMeshData {
        std::vector<Vec4f> vertexes;
        std::vector<std::vector<int>> face_vertex_indices;
    };
}