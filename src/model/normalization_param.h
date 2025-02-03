#ifndef NORMALIZATIONPARAM_H
#define NORMALIZATIONPARAM_H

#include <utility>

namespace s21 {

struct NormalizationParam {
  std::pair<float, float> min_max_x;
  std::pair<float, float> min_max_y;
  std::pair<float, float> min_max_z;
};
}  // namespace s21
#endif  // NORMALIZATIONPARAM_H
