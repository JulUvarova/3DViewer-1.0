#include <iostream>
#include "obj_data.h"

int main() {
  s21::OBJData obj_data;
  // obj_data.parse("box_and_cyl.obj");
  // obj_data.parse("duck.obj");
  obj_data.parse("megaduck.obj");

  std::cout << "Vertices count: " << obj_data.vertices.size() << std::endl;
  std::cout << "Objects count: " << obj_data.objects.size() << std::endl;
  for (const auto& object : obj_data.objects) {
    std::cout << "Object: " << object.name << std::endl;
    for (const auto& mesh : object.meshes) {
      std::cout << "  Group material: " << mesh.material << std::endl;
      std::cout << "  Faces count: " << mesh.faces.size() << std::endl;
    }
  }
  return 0;
}