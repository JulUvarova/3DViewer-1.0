#include <iostream>

#include "obj_data.h"

int main() {
  s21::OBJData obj_data;
  obj_data.Parse("box_and_cyl.obj");
  // obj_data.Parse("duck.obj");
  // obj_data.Parse("megaduck.obj");

  std::cout << obj_data.toString();

  // print top 20 vertices
  std::cout << "Top 20 vertices:" << std::endl;
  for (int i = 0; i < 20; i++) {
    std::cout << obj_data.vertices[i].x << " " << obj_data.vertices[i].y << " "
              << obj_data.vertices[i].z << std::endl;
  }

  obj_data.Normalize();

  // print top 20 vertices
  std::cout << "Top 20 normalized vertices:" << std::endl;
  for (int i = 0; i < 20; i++) {
    std::cout << obj_data.vertices[i].x << " " << obj_data.vertices[i].y << " "
              << obj_data.vertices[i].z << std::endl;
  }
  return 0;
}