#include "myopenglwidget.h"

// #include <GL/glu.h>

#include "iostream"

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent) : QOpenGLWidget{parent} {
  std::cout << "create MyGL" << std::endl;
}

MyOpenGLWidget::~MyOpenGLWidget() {}

void MyOpenGLWidget::setScene(s21::Scene *scene) { this->scene = scene; }

void MyOpenGLWidget::initializeGL() {
  // иногда без этого не работает
  // initializeOpenGLFunctions();
  //  включаем буфер глубины для отображения Z-координаты
  glEnable(GL_DEPTH_TEST);

  //! цвет фона
  glClearColor(0.5, 0.5, 0.5, 1.0);
}

// вызывается при изменении размера окна (пока фиксировано)
void MyOpenGLWidget::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void MyOpenGLWidget::paintGL() {
  std::cout << "paintGL" << std::endl;
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
