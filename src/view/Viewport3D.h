#pragma once
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <memory>

#include "Logger.h"
#include "UserSetting.h"
#include "controller.h"
#include "scene.h"

class Viewport3D : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 Q_SIGNALS:
  void signalChangeSize(const int w, const int h);

 public:
  Viewport3D(std::shared_ptr<UserSetting> setting, QWidget *parent = nullptr)
      : QOpenGLWidget(parent) {
    renderSetting = setting;
  }

  void setScene(std::shared_ptr<s21::DrawSceneData> sc) {
    scene = std::move(sc);
    needBufferUpdate = true;
    update();  // Request a repaint
  }

  //! для взятия экрана 4:3 и обратно
  void beforeGrab() {
    updateGif();
    update();
  }
  void afterGrab() {
    updateProjectionMatrix();
    update();
  }

  void UpdateModelMatrix() {
    modelMatrix.setToIdentity();
    auto [tx, ty, tz, rx, ry, rz, sx, sy, sz] =
        s21::Controller::GetInstance()->GetSceneParameters();

    // Apply translation
    modelMatrix.translate(tx, ty, tz);
    // Apply scaling
    modelMatrix.scale(sx, sy, sz);

    // Apply rotation
    modelMatrix.rotate(qRadiansToDegrees(rx), 1.0f, 0.0f,
                       0.0f);  // Rotate around X-axis
    modelMatrix.rotate(qRadiansToDegrees(ry), 0.0f, 1.0f,
                       0.0f);  // Rotate around Y-axis
    modelMatrix.rotate(qRadiansToDegrees(rz), 0.0f, 0.0f,
                       1.0f);  // Rotate around Z-axis
  }

  void repaint() {
    updateProjectionMatrix();
    update();
  }

 protected:
  void setBackColor() {
    glClearColor(renderSetting->getBackgroundColor().red() / 255.0,
                 renderSetting->getBackgroundColor().green() / 255.0,
                 renderSetting->getBackgroundColor().blue() / 255.0, 1.0);
  }

  void initializeGL() override {
    initializeOpenGLFunctions();
    setBackColor();
    glEnable(GL_DEPTH_TEST);
    initShaders();

    // Create VAO and VBO
    if (!vao.isCreated()) vao.create();
    if (!vbo.isCreated()) vbo.create();
    if (!ebo.isCreated()) ebo.create();
  }

  void resizeGL(int w, int h) override {
    glViewport(0, 0, w, h);
    updateProjectionMatrix();
  }

  void paintGL() override {
    setBackColor();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!scene || scene->vertices.empty() || scene->vertex_indices.empty())
      return;

    // Update buffers if needed
    if (needBufferUpdate) {
      updateBuffers();
      needBufferUpdate = false;
    }

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Bind shader program
    shaderProgram->bind();

    // Set uniforms
    shaderProgram->setUniformValue("projectionMatrix", projectionMatrix);
    shaderProgram->setUniformValue("viewMatrix", viewMatrix);
    shaderProgram->setUniformValue("modelMatrix", modelMatrix);

    // Set color uniforms based on settings
    QColor edgeColor = renderSetting->getEdgesColor();
    shaderProgram->setUniformValue("edgeColor", edgeColor.redF(),
                                   edgeColor.greenF(), edgeColor.blueF(), 1.0f);

    QColor vertexColor = renderSetting->getVerticesColor();
    shaderProgram->setUniformValue("vertexColor", vertexColor.redF(),
                                   vertexColor.greenF(), vertexColor.blueF(),
                                   1.0f);

    // Bind VAO
    vao.bind();

    // Draw edges
    if (renderSetting->getEdgesType() != "none" && indexCount > 0) {
      shaderProgram->setUniformValue("renderMode", 0);  // Edges mode

      if (renderSetting->getEdgesType() == "dashed") {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0x00FF);
      } else {
        glDisable(GL_LINE_STIPPLE);
      }

      glLineWidth(renderSetting->getEdgesSize());

      // Make sure the index buffer is bound before drawing
      ebo.bind();
      glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_INT, nullptr);
      ebo.release();
    }

    // Draw vertices
    if (renderSetting->getVerticesType() != "none" && vertexCount > 0) {
      shaderProgram->setUniformValue("renderMode", 1);  // Vertices mode

      if (renderSetting->getVerticesType() == "circle") {
        glEnable(GL_POINT_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
      }

      glPointSize(renderSetting->getVerticesSize());
      glDrawArrays(GL_POINTS, 0, vertexCount);

      if (renderSetting->getVerticesType() == "circle") {
        glDisable(GL_BLEND);
        glDisable(GL_POINT_SMOOTH);
      }
    }

    // Unbind VAO and shader
    vao.release();
    shaderProgram->release();

    // Disable depth testing
    glDisable(GL_DEPTH_TEST);
  }

 private:
  std::shared_ptr<UserSetting> renderSetting;
  std::shared_ptr<s21::DrawSceneData> scene;

  // New members for modern OpenGL
  QOpenGLVertexArrayObject vao;
  QOpenGLBuffer vbo{QOpenGLBuffer::VertexBuffer};
  QOpenGLBuffer ebo{QOpenGLBuffer::IndexBuffer};
  QOpenGLShaderProgram *shaderProgram = nullptr;

  QMatrix4x4 projectionMatrix;
  QMatrix4x4 viewMatrix;
  QMatrix4x4 modelMatrix;

  bool needBufferUpdate = false;
  int vertexCount = 0;
  int indexCount = 0;

  // Modern OpenGL
  // Initialize shaders
  void initShaders() {
    shaderProgram = new QOpenGLShaderProgram(this);

    // Vertex shader
    const char *vertexShaderSource = R"(
      #version 330 core
      layout (location = 0) in vec3 aPos;
      
      uniform mat4 projectionMatrix;
      uniform mat4 viewMatrix;
      uniform mat4 modelMatrix;
      
      void main() {
          gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);
      }
    )";

    // Fragment shader
    const char *fragmentShaderSource = R"(
      #version 330 core
      out vec4 FragColor;
      
      uniform int renderMode; // 0 for edges, 1 for vertices
      uniform vec4 edgeColor;
      uniform vec4 vertexColor;
      
      void main() {
          if (renderMode == 0) {
              FragColor = edgeColor;
          } else {
              FragColor = vertexColor;
          }
      }
    )";

    shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                           vertexShaderSource);
    shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                           fragmentShaderSource);
    shaderProgram->link();
  }

  // Update buffer data
  void updateBuffers() {
    if (!scene || scene->vertices.empty()) return;

    vertexCount = scene->vertices.size() / 3;
    indexCount = scene->vertex_indices.size();

    if (vertexCount == 0) return;

    // Make sure we have a valid shader program
    if (!shaderProgram || !shaderProgram->isLinked()) {
      initShaders();
    }

    // Bind VAO
    vao.bind();

    // Update vertex buffer
    vbo.bind();
    vbo.allocate(scene->vertices.data(),
                 scene->vertices.size() * sizeof(float));

    // Set vertex attributes
    shaderProgram->enableAttributeArray(0);
    shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);
    vbo.release();

    // Update index buffer if we have indices
    if (indexCount > 0) {
      ebo.bind();
      ebo.allocate(scene->vertex_indices.data(),
                   scene->vertex_indices.size() * sizeof(unsigned int));
      ebo.release();
    }

    // Release VAO
    vao.release();
  }

  void updateGif() {
    projectionMatrix.setToIdentity();

    float aspect = static_cast<float>(width()) / (width() * 3 / 4);

    if (renderSetting->isParallelProjectrion()) {
      // Orthographic projection
      float size = 1.0f;
      projectionMatrix.ortho(-size * aspect, size * aspect, -size, size, 1.0f,
                             100.0f);
    } else {
      // Perspective projection
      projectionMatrix.perspective(45.0f, aspect, 1.0f, 100.0f);
    }

    // Set view matrix (camera position)
    viewMatrix.setToIdentity();
    viewMatrix.translate(0.0f, 0.0f, -2.0f);

    // Set model matrix (identity by default)
    UpdateModelMatrix();
  }

  // Update projection matrix
  void updateProjectionMatrix() {
    projectionMatrix.setToIdentity();

    float aspect = static_cast<float>(width()) / height();

    if (renderSetting->isParallelProjectrion()) {
      // Orthographic projection
      float size = 1.0f;
      projectionMatrix.ortho(-size * aspect, size * aspect, -size, size, 1.0f,
                             100.0f);
    } else {
      // Perspective projection
      projectionMatrix.perspective(45.0f, aspect, 1.0f, 100.0f);
    }

    // Set view matrix (camera position)
    viewMatrix.setToIdentity();
    viewMatrix.translate(0.0f, 0.0f, -2.0f);

    // Reset the model matrix to identity
    UpdateModelMatrix();
  }
};