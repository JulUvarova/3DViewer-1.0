#ifndef MOCKFILEREADER_H
#define MOCKFILEREADER_H

#include "../base_filereader.h"

namespace s21 {

class FileReaderMock: public BaseFileReader {
 public:
  virtual Scene ReadScene(const char *path, NormalizationParam &param) override {
    (void)param;
    (void)path;

    Scene scene;
    std::vector<Vertex> ver{Vertex(Point{-0.5, 0.5f, 0.5f}), Vertex(Point{-0.5, 0.5f, 0.5f}), Vertex(Point{-0.5, 0.5f, 0.5f})};
    std::vector<Edge> ed{Edge(ver[0], ver[1]), Edge(ver[1], ver[2]), Edge(ver[2], ver[0])};

    Figure figure{ver, ed};
    scene.AddFigure(figure);

    return scene;
  }
};
}  // namespace s21
#endif  // MOCKFILEREADER_H
