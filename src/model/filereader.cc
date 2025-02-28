#include "filereader.h"

  s21::OBJData s21::FileReader(const char *path) {
    OBJData data;
    data.Parse(path);
    data.Normalize();
    return data;
  }