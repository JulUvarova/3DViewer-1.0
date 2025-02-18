#ifndef FACADE_H
#define FACADE_H

#include "filereader.h"
#include "scene.h"
#include "scene_parameters.h"

namespace s21 {

class Facade {
 public:
  Facade() {
    fileReader_ = new FileReader();
    sceneParam_ = new SceneParameters();
  }

  ~Facade() {
    delete fileReader_;
    delete scene_;
    delete sceneParam_;
  }

  DrawSceneData LoadScene(const char *path) {
    if (scene_) delete scene_;

    scene_ = new Scene();
    scene_->LoadSceneMeshData(fileReader_->ReadFile(path));

    // TODO проверка загрузилась ли сцена
    return scene_->DrawScene();
  }

  DrawSceneData ScaleX(const float value) {
    if (!scene_) return DrawSceneData();

    ScaleScene(value - sceneParam_->GetScaleX(), 0, 0);
    sceneParam_->SetScaleX(value);
    return scene_->DrawScene();
  }

  DrawSceneData ScaleY(const float value) {
    if (!scene_) return DrawSceneData();

    ScaleScene(0, value - sceneParam_->GetScaleY(), 0);
    sceneParam_->SetScaleY(value);
    return scene_->DrawScene();
  }

  DrawSceneData ScaleZ(const float value) {
    if (!scene_) return DrawSceneData();

    ScaleScene(0, 0, value - sceneParam_->GetScaleZ());
    sceneParam_->SetScaleZ(value);
    return scene_->DrawScene();
  }

  DrawSceneData MoveX(const float value) {
    if (!scene_) return DrawSceneData();

    MoveScene(value - sceneParam_->GetLocationX(), 0, 0);
    sceneParam_->SetLocationX(value);
    return scene_->DrawScene();
  }

  DrawSceneData MoveY(const float value) {
    if (!scene_) return DrawSceneData();

    MoveScene(0, value - sceneParam_->GetLocationY(), 0);
    sceneParam_->SetLocationY(value);
    return scene_->DrawScene();
  }

  DrawSceneData MoveZ(const float value) {
    if (!scene_) return DrawSceneData();

    MoveScene(0, 0, value - sceneParam_->GetLocationZ());
    sceneParam_->SetLocationZ(value);
    return scene_->DrawScene();
  }

  DrawSceneData RotateX(const float value) {
    if (!scene_) return DrawSceneData();

    std::cout << value << std::endl;  //!

    RotateScene(value - sceneParam_->GetRotationX(), 0, 0);
    sceneParam_->SetRotationX(value);
    return scene_->DrawScene();
  }

  DrawSceneData RotateY(const float value) {
    if (!scene_) return DrawSceneData();

    RotateScene(0, value - sceneParam_->GetRotationY(), 0);
    sceneParam_->SetRotationY(value);
    return scene_->DrawScene();
  }

  DrawSceneData RotateZ(const float value) {
    if (!scene_) return DrawSceneData();

    RotateScene(0, 0, value - sceneParam_->GetRotationZ());
    sceneParam_->SetRotationZ(value);
    return scene_->DrawScene();
  }

 private:
  FileReader *fileReader_;
  Scene *scene_{nullptr};
  SceneParameters *sceneParam_;

  DrawSceneData DrawScene() { return scene_->DrawScene(); }

  void MoveScene(float x, float y, float z) {
    Mat4f mat = TransformMatrixBuilder::CreateMoveMatrix(x, y, z);
    scene_->TransformSceneMeshData(mat);
  }

  void RotateScene(float x, float y, float z) {
    Mat4f mat = TransformMatrixBuilder::CreateRotationMatrix(x, y, z);
    scene_->TransformSceneMeshData(mat);
  }

  void ScaleScene(float x, float y, float z) {
    Mat4f mat = TransformMatrixBuilder::CreateScaleMatrix(x, y, z);
    scene_->TransformSceneMeshData(mat);
  }
};
}  // namespace s21
#endif  // FACADE_H
