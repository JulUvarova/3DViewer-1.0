#ifndef FILEREADER_H
#define FILEREADER_H

#include "obj/obj_data.h"

namespace s21 {

class FileReader {
 public:
  OBJData ReadFile(const char *path) {
    OBJData data;
    data.Parse(path);
    data.Normalize();
    return data;
  }
};
}  // namespace s21
#endif  // BASEFILEREADER_H
