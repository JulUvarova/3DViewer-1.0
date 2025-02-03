#ifndef BASEFILEREADER_H
#define BASEFILEREADER_H

#include "scene.h"
#include "normalization_param.h"

namespace s21 {

class BaseFileReader {
 public:
  virtual Scene ReadScene(const char *path, NormalizationParam &param) = 0;
};
}  // namespace s21
#endif  // BASEFILEREADER_H
