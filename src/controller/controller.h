#pragma once

#include <iostream>

#include "../model/facade.h"
#include "../model/scene.h"
#include "../model/scene_parameters.h"

namespace s21 {

class Controller {
 private:
  SceneParameters* sceneParam_;
  Facade* facade_;

 public:
  Controller() {
    facade_ = new Facade();
    sceneParam_ = new SceneParameters();
  }

  ~Controller() {
    delete sceneParam_;
    delete facade_;
  }

  // Upload scene
  DrawSceneData LoadScene(const char* filename) {
    return facade_->LoadScene(filename);
  }

  // Setters
  void SetViewportSize(int w, int h) {
    //! ?
    sceneParam_->SetAspectRate(w / static_cast<float>(h));

    std::cout << "Contr AspectRate: " << sceneParam_->GetAspectRate()
              << std::endl;
  }

  DrawSceneData SetScaleX(const int value) {
    float new_value = value / 1000.0;
    facade_->ScaleScene(new_value - sceneParam_->GetScaleX(), 0, 0);
    sceneParam_->SetScaleX(new_value);
    return facade_->DrawScene();

    std::cout << "Contr Scale X: " << sceneParam_->GetScaleX() << std::endl;
  }

  DrawSceneData SetScaleY(const int value) {
    float new_value = value / 100.0;
    facade_->ScaleScene( 0,new_value - sceneParam_->GetScaleY(), 0);
    sceneParam_->SetScaleY(new_value);
    return facade_->DrawScene();
    
    std::cout << "Contr Scale Y: " << sceneParam_->GetScaleY() << std::endl;
  }

  DrawSceneData SetScaleZ(const intdvalue) {
    float new_value = value / 100.0;
    facade_->ScaleScene( 0, 0,new_value - sceneParam_->GetScaleZ());
    sceneParam_->SetScaleZ(new_value);
    return facade_->DrawScene();
    
    std::cout << "Contr Scale Z: " << sceneParam_->GetScaleZ() << std::endl;
  }

  DrawSceneData SetLocationX(const int value) {
    float new_value = value / 200.0;
    facade_->MoveScene(new_value - sceneParam_->GetLocationX(), 0, 0);
    sceneParam_->SetLocationX(new_value);
    return facade_->DrawScene();

    std::cout << "Contr Location X: " << sceneParam_->GetLocationX()
              << std::endl;
  }

  DrawSceneData SetLocationY(const int value) {
    float new_value = value / 200.0;
    facade_->MoveScene(0, new_value - sceneParam_->GetLocationY(), 0);
    sceneParam_->SetLocationY(new_value);
    return facade_->DrawScene();

    std::cout << "Contr Location Y: " << sceneParam_->GetLocationY()
              << std::endl;
  }

  DrawSceneData SetLocationZ(const int value) {
    float new_value = value / 200.0;
    facade_->MoveScene(0, 0, new_value - sceneParam_->GetLocationZ());
    sceneParam_->SetLocationZ(new_value);
    return facade_->DrawScene();

    std::cout << "Contr Location Z: " << sceneParam_->GetLocationZ()
              << std::endl;
  }

  DrawSceneData SetRotationX(const int value) {
    facade_->RotateScene(value - sceneParam_->GetRotationX(), 0, 0);
    sceneParam_->SetRotationX(value);
    return facade_->DrawScene();

    std::cout << "Contr Rotation X: " << sceneParam_->GetRotationX()
              << std::endl;
  }

  DrawSceneData SetRotationY(const int value) {
    facade_->RotateScene( 0,value - sceneParam_->GetRotationY(), 0);
    sceneParam_->SetRotationY(value);
    return facade_->DrawScene();

    std::cout << "Contr Rotation Y: " << sceneParam_->GetRotationY()
              << std::endl;
  }

  DrawSceneData SetRotationZ(const int value) {
      facade_->RotateScene( 0, 0,value - sceneParam_->GetRotationZ());
    sceneParam_->SetRotationZ(value);
    return facade_->DrawScene();
    std::cout << "Contr Rotation Z: " << sceneParam_->GetRotationZ()
              << std::endl;
  }

  void SetProjectionType(const bool isParallel) {
    if (isParallel)
      sceneParam_->SetProjectionType(ProjectionTypes::kParallel);
    else
      sceneParam_->SetProjectionType(ProjectionTypes::kCentral);
    std::cout << "Contr Projection: change" << std::endl;
  }

  void SetVertexType(const QString type) {
    if (type == "circle") {
      sceneParam_->SetVertexStyle(VertexStyle::kCircle);
    } else if (type == "square") {
      sceneParam_->SetVertexStyle(VertexStyle::kSquare);
    } else {
      sceneParam_->SetVertexStyle(VertexStyle::kNone);
    }
    std::cout << "Contr VertexType: change" << std::endl;
  }

  void SetVertexSize(const int size) {
    sceneParam_->SetVertexSize(size);
    std::cout << "Contr VertexSize: " << sceneParam_->GetVertexSize()
              << std::endl;
  }

  void SetEdgeType(const QString type) {
    if (type == "line") {
      sceneParam_->SetEdgeStyle(EdgeStyle::kLine);
    } else if (type == "dashes") {
      sceneParam_->SetEdgeStyle(EdgeStyle::kDashed);
    } else {
      sceneParam_->SetEdgeStyle(EdgeStyle::kNone);
    }
    std::cout << "Contr EdgeType: change" << std::endl;
  }

  void SetEdgeSize(const int size) {
    sceneParam_->SetEdgeSize(size);
    std::cout << "Contr EdgeSize: " << sceneParam_->GetEdgeSize() << std::endl;
  }

  void SetVertexColor(const QColor& color) {
    sceneParam_->SetVertexColor({static_cast<float>(color.red() / 255.0),
                                 static_cast<float>(color.green() / 255.0),
                                 static_cast<float>(color.blue() / 255.0)});
    std::cout << "Contr VertexColor: r" << sceneParam_->GetVertexColor().r
              << " g" << sceneParam_->GetVertexColor().g << " b"
              << sceneParam_->GetVertexColor().b << std::endl;
  }

  void SetEdgeColor(const QColor& color) {
    sceneParam_->SetEdgeColor({static_cast<float>(color.red() / 255.0),
                               static_cast<float>(color.green() / 255.0),
                               static_cast<float>(color.blue() / 255.0)});
    std::cout << "Contr EdgeColor: r" << sceneParam_->GetEdgeColor().r << " g"
              << sceneParam_->GetEdgeColor().g << " b"
              << sceneParam_->GetEdgeColor().b << std::endl;
  }

  void SetBackgroundColor(const QColor& color) {
    sceneParam_->SetBackgroundColor({static_cast<float>(color.red() / 255.0),
                                     static_cast<float>(color.green() / 255.0),
                                     static_cast<float>(color.blue() / 255.0)});
    std::cout << "Contr BackgroundColor: r"
              << sceneParam_->GetBackgroundColor().r << " g"
              << sceneParam_->GetBackgroundColor().g << " b"
              << sceneParam_->GetBackgroundColor().b << std::endl;
  }
};
}  // namespace s21
