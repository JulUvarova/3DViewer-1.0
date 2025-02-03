#ifndef EDGE_H
#define EDGE_H

#include "vertex.h"

namespace s21 {

class Edge {
 public:
  Edge(Vertex& b, Vertex& e) : begin_(b), end_(e) {};
  Edge() = delete;

  inline Vertex& GetBegin() noexcept { return begin_; }
  inline Vertex& GetEnd() noexcept { return end_; }

 private:
  Vertex& begin_;
  Vertex& end_;
};
}  // namespace s21
#endif  // EDGE_H
