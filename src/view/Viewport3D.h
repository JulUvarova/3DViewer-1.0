#pragma once
#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include "../model/obj/obj_data.h"
#include "ProjectionButton.h"

class Viewport3D : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 signals:
  void signalChangeSize(const int w, const int h);
  void signalChangeProjection(const bool isParallel);

 public:
  Viewport3D(UserSetting *setting, QWidget *parent = nullptr)
      : QOpenGLWidget(parent) {
    renderSetting = setting;

    projectionButton = new ProjectionButton(this);

    // get new projection
    connect(projectionButton, &ProjectionButton::signalChangeProjection, this,
            &Viewport3D::changeProjection);
  }

  void setScene(s21::OBJData *obj_data) {
    scene = obj_data;
    update();
  }

 protected:
  void setBackColor() {
    glClearColor(renderSetting->getBackgroundColor().red() / 255.0,
                 renderSetting->getBackgroundColor().green() / 255.0,
                 renderSetting->getBackgroundColor().blue() / 255.0, 0);
  }

  void initializeGL() override {
    initializeOpenGLFunctions();
    setBackColor();
  }

  void resizeGL(int w, int h) override {
    glViewport(0, 0, w, h);

    // resize button
    projectionButton->setLocation(width());

    // send to MainWindow changed size
    emit signalChangeSize(width(), height());
  }

  void paintGL() override {
    // Add 3D rendering logic here

    setBackColor();
    // Очищаем экран
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //! https://opengl.org.ru/books/open_gl/chapter3.1.html
    if (!scene) return;
    // print verteces
    if (renderSetting->getVerticesType() != "none") {
      if (renderSetting->getVerticesType() == "circle")
        glEnable(GL_POINT_SMOOTH);  // без него рисуем квадратами
      glPointSize(renderSetting->getVerticesSize());
      glBegin(GL_POINTS);
      glColor3d(renderSetting->getVerticesColor().red() / 255.0,
                renderSetting->getVerticesColor().green() / 255.0,
                renderSetting->getVerticesColor().blue() / 255.0);
      for (long unsigned int i = 0; i < scene->vertices.size(); i++) {
        glVertex3f(scene->vertices[i].x, scene->vertices[i].y,
                   scene->vertices[i].z);
        // qDebug() << scene->vertices[i].x << " " << scene->vertices[i].y << "
        // "
        //          << scene->vertices[i].z;
      }
      glEnd();
    }
    if (renderSetting->getVerticesType() == "circle")
      glDisable(GL_POINT_SMOOTH);

    // print edges
    if (renderSetting->getEdgesType() != "none") {
      glLineWidth(renderSetting->getEdgesSize());
      glColor3d(renderSetting->getEdgesColor().red() / 255.0,
                renderSetting->getEdgesColor().green() / 255.0,
                renderSetting->getEdgesColor().blue() / 255.0);
      for (auto &obj : scene->objects) {
        for (auto &mesh : obj.meshes) {
          for (auto &face : mesh.faces) {
              glEnable(GL_LINE_SMOOTH);
            if (renderSetting->getEdgesType() == "dashed") {
              glEnable(GL_LINE_STIPPLE);
              glLineStipple(1, 0x00FF);
           
            }    glBegin(GL_LINE_LOOP);
            for (auto &indx : face.vertices) {
              glVertex3f(scene->vertices[indx.v].x, scene->vertices[indx.v].y,
                         scene->vertices[indx.v].z);
            }
            glEnd();
            glDisable(GL_LINE_SMOOTH);
            if (renderSetting->getEdgesType() == "dashed")
              glDisable(GL_LINE_STIPPLE);
          }
        }
      }
    }
  }

 private:
  ProjectionButton *projectionButton;
  UserSetting *renderSetting;
  s21::OBJData *scene{nullptr};

  void changeProjection(bool isParallel) {
    emit signalChangeProjection(isParallel);
  }
};