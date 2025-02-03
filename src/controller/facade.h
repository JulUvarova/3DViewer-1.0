#ifndef FACADE_H
#define FACADE_H

#include "../model/base_filereader.h"
#include "../model/implementation/filereader_mock.h"
#include "../model/normalization_param.h"
#include "../model/scene.h"
#include "operation_result.h"

namespace s21 {

class Facade {
 public:
  Facade() { fileReader_ = new FileReaderMock(); }

  Scene* GetScene() { return &scene_; }

  FacadeOperationResult LoadScene(const char *path, NormalizationParam &param) {
    scene_ = fileReader_->ReadScene(path, param);
   
    return FacadeOperationResult();
  }
  // FacadeOperationResult MoveScene(float x, float y, float z) { return
  // FacadeOperationResult(); } FacadeOperationResult RotateScene(float x, float
  // y, float z) { return FacadeOperationResult(); } FacadeOperationResult
  // ScaleScene(float x, float y, float z) { return FacadeOperationResult(); }

 private:
  BaseFileReader *fileReader_;
  Scene scene_;

  // ушли во View
  // BaseSceneDrawer sceneDrawer;
  // FacadeOperationResult DrawScene();
};
}  // namespace s21
#endif  // FACADE_H
