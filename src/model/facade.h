#ifndef FACADE_H
#define FACADE_H

#include "filereader.h"
#include "scene.h"

namespace s21 {

class Facade {
 public:
  Facade() { fileReader_ = new FileReader(); }

  ~Facade() {
    delete fileReader_;
    delete scene_;
  }

  DrawSceneData LoadScene(const char *path) {
    if (scene_) delete scene_;
    scene_ = new Scene();
    scene_->LoadSceneMeshData(fileReader_->ReadFile(path));
    return scene_->DrawScene();
  }

  DrawSceneData DrawScene() {
    if (!scene_) return DrawSceneData();
    return scene_->DrawScene();
  }

  void MoveScene(float x, float y, float z) {
    if (!scene_) return;

    Mat4f mat = TransformMatrixBuilder::CreateMoveMatrix(x, y, z);
    scene_->TransformSceneMeshData(mat);
  }

  void RotateScene(float x, float y, float z) {
    if (!scene_) return;

    Mat4f mat = TransformMatrixBuilder::CreateRotationMatrix(x, y, z);
    scene_->TransformSceneMeshData(mat);
  }

  void ScaleScene(float x, float y, float z) {
    if (!scene_) return;

    Mat4f mat = TransformMatrixBuilder::CreateScaleMatrix(x, y, z);
    scene_->TransformSceneMeshData(mat);
  }

 private:
  FileReader *fileReader_;
  Scene *scene_{nullptr};
};
}  // namespace s21
#endif  // FACADE_H
