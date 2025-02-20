#pragma once
#include <QOpenGLFunctions>
#include <QOpenGLWidget>

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

    // redraw if projection changed
    connect(projectionButton, &ProjectionButton::signalChangeProjection, this,
            &Viewport3D::repaint);
  }

  // TODO как-то эту хрень оптимизировать, начиная со сцены в фасаде
  void setScene(std::vector<float> vrtx, std::vector<int> indx) {
    vertices = vrtx;
    indices = indx;
  }

  // убирают кнопку проекции для скрина
  void beforeGrab() { projectionButton->setVisible(false); }
  void afterGrab() { projectionButton->setVisible(true); }

 protected:
  void setBackColor() {
    glClearColor(renderSetting->getBackgroundColor().red() / 255.0,
                 renderSetting->getBackgroundColor().green() / 255.0,
                 renderSetting->getBackgroundColor().blue() / 255.0, 1.0);
  }

  void initializeGL() override {
    initializeOpenGLFunctions();
    setBackColor();
  }

  void resizeGL(int w, int h) override {
    glViewport(0, 0, w, h);

    // resize button
    projectionButton->setLocation(width());
  }

  void paintGL() override {
    setBackColor();
    // Очищаем экран
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!vertices.size()) return;

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices.data());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // projection
    chooseProjection();

    // edges
    if (renderSetting->getEdgesType() != "none") drawEdges();

    // vertices
    if (renderSetting->getVerticesType() != "none") drawVertices();

    // close options
    glDisableClientState(GL_VERTEX_ARRAY);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
  }

 private:
  ProjectionButton *projectionButton;
  UserSetting *renderSetting;

  std::vector<float> vertices;
  std::vector<int> indices;

  void chooseProjection() {
    //  gluPerspective(45.0, (GLfloat)width() / (GLfloat)height(), 0.1, 100.0);

    // glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 100.0);

    GLdouble nearPlane = 1.0;   // ближняя плоскость отсечения
    GLdouble farPlane = 100.0;  // дальняя плоскость отсечения

    //! Учет пропорции окна
    GLdouble aspect = (GLdouble)width() / (GLdouble)height();

    if (projectionButton->isParallelProjection()) {
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
    glTranslatef(0.0, 0.0, -5.0);
  }

  void drawEdges() {
    if (renderSetting->getEdgesType() == "dashed") {
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(1, 0x00FF);
    } else
      glDisable(GL_LINE_STIPPLE);

    glLineWidth(renderSetting->getEdgesSize());

    glColor3f(renderSetting->getEdgesColor().red() / 255.0,
              renderSetting->getEdgesColor().green() / 255.0,
              renderSetting->getEdgesColor().blue() / 255.0);

    glEnableClientState(GL_VERTEX_ARRAY);

    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, indices.data());
    glDisableClientState(GL_VERTEX_ARRAY);
  }

  void drawVertices() {
    if (renderSetting->getVerticesType() == "circle")
      glEnable(GL_POINT_SMOOTH);  // без него рисуем квадратами
    glPointSize(renderSetting->getVerticesSize());
    glColor3f(renderSetting->getVerticesColor().red() / 255.0,
              renderSetting->getVerticesColor().green() / 255.0,
              renderSetting->getVerticesColor().blue() / 255.0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_POINTS, 0, vertices.size());
    glDisableClientState(GL_VERTEX_ARRAY);
    if (renderSetting->getVerticesType() == "circle")
      glDisable(GL_POINT_SMOOTH);
  }

  void repaint() { update(); }
};