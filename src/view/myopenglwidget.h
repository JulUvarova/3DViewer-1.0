#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QWidget>

#include "../model/scene.h"

class MyOpenGLWidget : public QOpenGLWidget {
  Q_OBJECT
 public:
  MyOpenGLWidget(QWidget *parent = nullptr);
  ~MyOpenGLWidget();

  void setScene(s21::Scene *scene);

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

 private:
  s21::Scene *scene;
};

#endif  // MYOPENGLWIDGET_H
