#pragma once
#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include "../model/scene_mesh_data.h"
#include "ProjectionButton.h"
#include "UserSetting.h"

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

  void setScene(s21::SceneMeshData *sc) { scene = sc; }

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
    
    // print edges
    if (renderSetting->getEdgesType() != "none") {
      glLineWidth(renderSetting->getEdgesSize());
      glColor3d(renderSetting->getEdgesColor().red() / 255.0,
                renderSetting->getEdgesColor().green() / 255.0,
                renderSetting->getEdgesColor().blue() / 255.0);
      glEnable(GL_LINE_SMOOTH);
      if (renderSetting->getEdgesType() == "dashed") {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0x00FF);
      }

      for (auto &face : scene->face_vertex_indices) {
        glBegin(GL_LINE_LOOP);
        for (auto &indx : face) {
          glVertex3f(scene->vertexes[indx].x, scene->vertexes[indx].y,
                     scene->vertexes[indx].z);
        }
        glEnd();
      }
      glDisable(GL_LINE_SMOOTH);
      if (renderSetting->getEdgesType() == "dashed") glDisable(GL_LINE_STIPPLE);
    }

    // print verteces
    if (renderSetting->getVerticesType() != "none") {
      if (renderSetting->getVerticesType() == "circle")
        glEnable(GL_POINT_SMOOTH);  // без него рисуем квадратами
      glPointSize(renderSetting->getVerticesSize());
      glBegin(GL_POINTS);
      glColor3d(renderSetting->getVerticesColor().red() / 255.0,
                renderSetting->getVerticesColor().green() / 255.0,
                renderSetting->getVerticesColor().blue() / 255.0);
      for (long unsigned int i = 0; i < scene->vertexes.size(); i++) {
        glVertex3f(scene->vertexes[i].x, scene->vertexes[i].y,
                   scene->vertexes[i].z);
      }
      glEnd();
    }
    if (renderSetting->getVerticesType() == "circle")
      glDisable(GL_POINT_SMOOTH);
  }

  /*
    void paintGL() override {
      // Add 3D rendering logic here

      setBackColor();
      // Очищаем экран
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

          //! ОТКРЫТЬ
      glEnableClientState(GL_VERTEX_ARRAY);

      // Устанавливаем режим рисования полигонов
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

      // Устанавливаем матрицу модели-вида-проекции
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      // gluPerspective(45.0, (GLfloat)width() / (GLfloat)height(), 0.1, 100.0);

      //glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 100.0);
      GLdouble aspect = (GLdouble)width() / (GLdouble)height();
      GLdouble nearPlane = 1.0;  // ближняя плоскость отсечения
      GLdouble farPlane = 100.0;  // дальняя плоскость отсечения

      if (renderSetting->getProjection() == true) {  //TODO: некрасиво
        glOrtho(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0, nearPlane, farPlane);
      } else {
        // Вычисляем границы усечённой пирамиды
      GLdouble fov = 45.0;  // поле зрения в градусах

      // Вычисляем границы усечённой пирамиды
      GLdouble top = nearPlane * tan(fov * M_PI / 360.0);  // fov/2 в радианах
      GLdouble bottom = -top;
      GLdouble right = top * aspect;
      GLdouble left = -right;

      glFrustum(left, right, bottom, top, nearPlane, farPlane);
      }
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glTranslatef(0.0, 0.0, -5.0);

      // Устанавливаем цвет
      glColor3f(renderSetting->getEdgesColor().red()/255.0,
    renderSetting->getEdgesColor().green()/255.0,
    renderSetting->getEdgesColor().blue()/255.0);

      // Определяем вершины куба
      GLfloat vertices[] = {-0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f,
    0.5f,  0.5f,  0.5f,  -0.5f, 0.5f, 0.5f};
  */

 private:
  ProjectionButton *projectionButton;
  UserSetting *renderSetting;
  s21::SceneMeshData *scene{nullptr};

  void changeProjection(bool isParallel) {
    emit signalChangeProjection(isParallel);
  }
};