#pragma once
#include <stdexcept>
#include <string>

namespace s21 {
class ViewerException : public std::runtime_error {
 public:
  explicit ViewerException(const std::string &msg) : std::runtime_error(msg) {}
};

class MeshLoadException : public ViewerException {
 public:
  explicit MeshLoadException(const std::string &msg) : ViewerException(msg) {}
};

class RenderException : public ViewerException {
 public:
  explicit RenderException(const std::string &msg) : ViewerException(msg) {}
};
};  // namespace s21