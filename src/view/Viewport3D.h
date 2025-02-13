#pragma once
#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include "ProjectionButton.h"

class Viewport3D : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 signals:
  void signalChangeSize(const int w, const int h);
  void signalChangeProjection(const bool isParallel);

 public:
  Viewport3D(RenderSetting *setting, QWidget *parent = nullptr)
      : QOpenGLWidget(parent) {
    renderSetting = setting;

    projectionButton = new ProjectionButton(this);

    // get new projection
    connect(projectionButton, &ProjectionButton::signalChangeProjection, this,
            &Viewport3D::changeProjection);
  }

 protected:
  void setBackColor() {
    glClearColor(
        renderSetting->getBackgroundColor().red() / 255.0,
        renderSetting->getBackgroundColor().green() / 255.0,
        renderSetting->getBackgroundColor().blue() / 255.0,
        0);  
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

    //! ОТКРЫТЬ
    glEnableClientState(GL_VERTEX_ARRAY);

    // Устанавливаем режим рисования полигонов
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Устанавливаем матрицу модели-вида-проекции
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // gluPerspective(45.0, (GLfloat)width() / (GLfloat)height(), 0.1, 100.0);
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);

    // Устанавливаем цвет
    glColor3f(renderSetting->getEdgesColor().red()/255.0, renderSetting->getEdgesColor().green()/255.0, renderSetting->getEdgesColor().blue()/255.0);

    // Определяем вершины куба
    GLfloat vertices[] = {-0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f, 0.5f,
                          -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, 0.5f, 0.5f,
                          -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,  -0.5f, 0.5f, 0.5f};

    // Устанавливаем толщину ребер
    glLineWidth(renderSetting->getEdgesSize());

    // Определяем индексы куба
    GLuint indices[] = {0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 5, 4,
                        1, 2, 6, 5, 2, 3, 7, 6, 3, 0, 4, 7};

    // Устанавливаем вершины
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    // Рисуем куб
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, indices);

    //! требует #include <GL/glu.h> /нет в кампусе?
    // gluPerspective(45.0, (GLfloat)width() / (GLfloat)height(), 0.1, 100.0);

    // glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 100.0);

    //! ЗАКРЫТЬ
    glDisableClientState(GL_VERTEX_ARRAY);
  }

 private:
  ProjectionButton *projectionButton;
  RenderSetting *renderSetting;

  void changeProjection(bool isParallel) {
    emit signalChangeProjection(isParallel);
  }
};