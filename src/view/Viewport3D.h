#pragma once
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <memory>

#include "../model/scene.h"
#include "ProjectionButton.h"
#include "UserSetting.h"

enum class MouseAction {
  kNone = 0,
  kLeftButton,
  kMiddleButton,
};

class Viewport3D : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 Q_SIGNALS:
  void signalChangeSize(const int w, const int h);
  void signalChangeProjection(const bool isParallel);
  void signalChangeMoveCoords(std::pair<int, int> coordXY);
  void signalChangeScaleCoords(std::pair<int, int> coordXY);
  void signalChangeRotateCoords(std::pair<int, int> coordXY);

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
  void setScene(std::shared_ptr<s21::DrawSceneData> sc) {
    scene = std::move(sc);
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

    if (!scene || scene->vertices.empty()) return;

    glEnableClientState(GL_VERTEX_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, scene->vertices.data());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // projection
    chooseProjection();

    // edges
    if (renderSetting->getEdgesType() != "none") {
      drawEdges();
    }

    // vertices
    if (renderSetting->getVerticesType() != "none") {
      drawVertices();
    }

    // close options
    glDisableClientState(GL_VERTEX_ARRAY);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
  }

 private:
  ProjectionButton *projectionButton;
  UserSetting *renderSetting;
  std::shared_ptr<s21::DrawSceneData> scene;

  // catch mouse press and mouse release
  MouseAction mouseAction = MouseAction::kNone;
  QPoint mousePos;

  void mousePressEvent(QMouseEvent *event) override {
    if (event->button() == Qt::LeftButton)
      mouseAction = MouseAction::kLeftButton;
    else if (event->button() == Qt::MiddleButton)
      mouseAction = MouseAction::kMiddleButton;

    if (mouseAction != MouseAction::kNone) {
      mousePos = event->pos();
    }
  }

  void mouseMoveEvent(QMouseEvent *event) override {
    if (mouseAction != MouseAction::kNone) {
      QPoint pos = event->pos();

      // left will move object
      if (mouseAction == MouseAction::kLeftButton) {
        int shiftX = (pos.x() * 200 / width() - 100) -
                     (mousePos.x() * 200 / width() - 100);
        int shiftY = -((pos.y() * 200 / height() - 100) -
                       (mousePos.y() * 200 / height() - 100));

        Q_EMIT signalChangeMoveCoords(
            std::pair<int, int>{2 * shiftX, 2 * shiftY});
      }
      // middle will rotate object
      if (mouseAction == MouseAction::kMiddleButton) {
        int shiftX = -((pos.x() * 360 / width() - 180) -
                       (mousePos.x() * 360 / width() - 180));
        int shiftY = -((pos.y() * 360 / height() - 180) -
                       (mousePos.y() * 360 / height() - 180));

        Q_EMIT signalChangeRotateCoords(std::pair<int, int>{shiftY, shiftX});
      }
      mousePos = pos;
    }
  }

  void mouseReleaseEvent([[maybe_unused]] QMouseEvent *event) override {
    mouseAction = MouseAction::kNone;
  }

  void wheelEvent(QWheelEvent *event) {
    Q_EMIT signalChangeScaleCoords(
        std::pair<int, int>{event->angleDelta().y() / 24, 0});  // 5 ед
  }

  void chooseProjection() {
    GLdouble nearPlane = 1.0;  // ближняя плоскость отсечения
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

    glDrawElements(GL_LINES, scene->vertex_indices.size(), GL_UNSIGNED_INT,
                   scene->vertex_indices.data());
    glDisableClientState(GL_VERTEX_ARRAY);
  }

  void drawVertices() {
    if (renderSetting->getVerticesType() == "circle") {
      glEnable(GL_POINT_SMOOTH);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    }  // без этих настроек точки будут квадратными

    glPointSize(renderSetting->getVerticesSize());
    glColor3f(renderSetting->getVerticesColor().red() / 255.0,
              renderSetting->getVerticesColor().green() / 255.0,
              renderSetting->getVerticesColor().blue() / 255.0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_POINTS, 0, scene->vertices.size() / 3);
    glDisableClientState(GL_VERTEX_ARRAY);

    if (renderSetting->getVerticesType() == "circle") {
      glDisable(GL_BLEND);
      glDisable(GL_POINT_SMOOTH);
    }
  }

  void repaint() { update(); }
};