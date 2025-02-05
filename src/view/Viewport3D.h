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

    // qDebug() << "paintGL";
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
    glColor3f(1.0, 0.0, 0.0);

    // Определяем вершины куба
    GLfloat vertices[] = {-0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f, 0.5f,
                          -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, 0.5f, 0.5f,
                          -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,  -0.5f, 0.5f, 0.5f};

    // Устанавливаем толщину ребер
    glLineWidth(3.0);

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
}
;