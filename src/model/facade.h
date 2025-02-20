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

  DrawSceneData* LoadScene(const char *path) {
    if (scene_) delete scene_;

    scene_ = new Scene();
    return scene_->LoadSceneMeshData(fileReader_->ReadFile(path));
    // TODO проверка загрузилась ли сцена
  }

  void resetScenePosition() {
    if (!scene_) return;

    sceneParam_ = std::move(new SceneParameters());
    TransformScene();
  }

  void Scale(const float value) {
    if (!scene_) return;

    sceneParam_->SetScaleX(value);
    TransformScene();
  }

  void ScaleX(const float value) {
    if (!scene_) return;

    sceneParam_->SetScaleX(value);
    TransformScene();
  }

  void ScaleY(const float value) {
    if (!scene_) return;

    sceneParam_->SetScaleX(value);
    TransformScene();
  }

  void ScaleZ(const float value) {
    if (!scene_) return;

    sceneParam_->SetScaleX(value);
    TransformScene();
  }

  void MoveX(const float value) {
    if (!scene_) return;

    sceneParam_->SetLocationX(value);
    TransformScene();
  }

  void MoveY(const float value) {
    if (!scene_) return;

    sceneParam_->SetLocationY(value);
    TransformScene();
  }

  void MoveZ(const float value) {
    if (!scene_) return;

    sceneParam_->SetLocationZ(value);
    TransformScene();
  }

  void RotateX(const float value) {
    if (!scene_) return;

    sceneParam_->SetRotationX(value);
    TransformScene();
  }

  void RotateY(const float value) {
    if (!scene_) return;

    sceneParam_->SetRotationY(value);
    TransformScene();
  }

  void RotateZ(const float value) {
    if (!scene_) return;

    sceneParam_->SetRotationZ(value);
    TransformScene();
  }

 private:
  FileReader *fileReader_;
  Scene *scene_{nullptr};
  SceneParameters *sceneParam_;

  // DrawSceneData DrawScene() { return scene_->DrawScene(); }

  void TransformScene() {
    Mat4f move_mat = TransformMatrixBuilder::CreateMoveMatrix(
        sceneParam_->GetLocationX(), sceneParam_->GetLocationY(),
        sceneParam_->GetLocationZ());
    Mat4f rotate_mat = TransformMatrixBuilder::CreateRotationMatrix(
        sceneParam_->GetRotationX(), sceneParam_->GetRotationY(),
        sceneParam_->GetRotationZ());
    Mat4f scale_mat = TransformMatrixBuilder::CreateScaleMatrix(
        sceneParam_->GetScaleX(), sceneParam_->GetScaleX(),
        sceneParam_->GetScaleX());
    Mat4f transform_mat = scale_mat * move_mat * rotate_mat;
    scene_->TransformSceneMeshData(transform_mat);
  }

  // void MoveScene(float x, float y, float z) {
  //   Mat4f mat = TransformMatrixBuilder::CreateMoveMatrix(x, y, z);
  //   scene_->TransformSceneMeshData(mat);
  // }

  // void RotateScene(float x, float y, float z) {
  //   Mat4f mat = TransformMatrixBuilder::CreateRotationMatrix(x, y, z);
  //   scene_->TransformSceneMeshData(mat);
  // }

  // void ScaleScene(float x, float y, float z) {
  //   Mat4f mat = TransformMatrixBuilder::CreateScaleMatrix(x, y, z);
  //   scene_->TransformSceneMeshData(mat);
  // }
};
}  // namespace s21
#endif  // FACADE_H
