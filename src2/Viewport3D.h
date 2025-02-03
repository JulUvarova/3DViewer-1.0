#pragma once
#include <QOpenGLFunctions>
#include <QOpenGLWidget>

class Viewport3D : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  Viewport3D(QWidget *parent = nullptr) : QOpenGLWidget(parent) {}

 protected:
  void initializeGL() override {
    initializeOpenGLFunctions();
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);  // Dark gray background
  }

  void resizeGL(int w, int h) override { glViewport(0, 0, w, h); }

  void paintGL() override {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Add 3D rendering logic here
  }
};