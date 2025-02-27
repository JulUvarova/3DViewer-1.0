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

/**
 * @brief The Viewport3D class provides a 3D viewport widget using modern
 * OpenGL.
 *
 * This widget handles OpenGL initialization, shader setup, and rendering of a
 * 3D scene. It supports updating the scene, handling resizing, and toggling
 * between different rendering modes.
 */
class Viewport3D : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 Q_SIGNALS:
  /**
   * @brief Signal emitted when the viewport size changes.
   * @param w New width.
   * @param h New height.
   */
  void signalChangeSize(const int w, const int h);

 public:
  /**
   * @brief Constructs a new Viewport3D widget.
   * @param setting Shared pointer to the user settings for rendering.
   * @param parent Pointer to the parent widget, default is nullptr.
   */
  Viewport3D(std::shared_ptr<UserSetting> setting, QWidget *parent = nullptr)
      : QOpenGLWidget(parent) {
    renderSetting = setting;
  }

  /**
   * @brief Sets the scene to be rendered.
   *
   * This function updates the internal scene data and requests a repaint.
   *
   * @param sc Shared pointer to the new scene data.
   */
  void setScene(std::shared_ptr<s21::DrawSceneData> sc) {
    scene = std::move(sc);
    needBufferUpdate = true;
    update();  // Request a repaint
  }

  /**
   * @brief Change projection matrix before and after grabbing the screen.
   *
   * Adjusts the projection matrix for a 4:3 aspect ratio, useful for capturing
   * a screenshot.
   * @param ifGif Change condition for count aspect ration.
   */
  void changeAspectRatio(bool isGif) {
    isGifRatio = isGif;
    repaint();
  }

  /**
   * @brief Updates the model transformation matrix.
   *
   * Resets the model matrix and applies translation, scaling, and rotation
   * based on the current scene parameters.
   */
  void UpdateModelMatrix() {
    modelMatrix.setToIdentity();
    auto [tx, ty, tz, rx, ry, rz, sx, sy, sz] =
        s21::Controller::GetInstance()->GetSceneParameters();

    // Apply translation
    modelMatrix.translate(tx, ty, tz);
    // Apply scaling
    modelMatrix.scale(sx, sy, sz);

    // Apply rotation (converted from radians to degrees)
    modelMatrix.rotate(qRadiansToDegrees(rx), 1.0f, 0.0f,
                       0.0f);  // Rotate around X-axis
    modelMatrix.rotate(qRadiansToDegrees(ry), 0.0f, 1.0f,
                       0.0f);  // Rotate around Y-axis
    modelMatrix.rotate(qRadiansToDegrees(rz), 0.0f, 0.0f,
                       1.0f);  // Rotate around Z-axis
  }

  /**
   * @brief Repaints the viewport.
   *
   * This function updates the projection matrix and requests a repaint.
   */
  void repaint() {
    updateProjectionMatrix();
    update();
  }

 protected:
  /**
   * @brief Sets the background color for the OpenGL context.
   *
   * Retrieves the background color from user settings and applies it.
   */
  void setBackColor() {
    glClearColor(renderSetting->getBackgroundColor().red() / 255.0,
                 renderSetting->getBackgroundColor().green() / 255.0,
                 renderSetting->getBackgroundColor().blue() / 255.0, 1.0);
  }

  /**
   * @brief Initializes the OpenGL context.
   *
   * Sets up OpenGL functions, background color, depth testing, shaders,
   * and creates necessary vertex and index buffers.
   */
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

  /**
   * @brief Handles widget resize events.
   *
   * Adjusts the OpenGL viewport and updates the projection matrix accordingly.
   *
   * @param w New width.
   * @param h New height.
   */
  void resizeGL(int w, int h) override {
    glViewport(0, 0, w, h);
    updateProjectionMatrix();
  }

  /**
   * @brief Renders the scene using OpenGL.
   *
   * Clears the screen, updates buffers if needed, sets shader uniforms,
   * and draws the edges and vertices based on the current rendering settings.
   */
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

    // Set transformation uniforms
    shaderProgram->setUniformValue("projectionMatrix", projectionMatrix);
    shaderProgram->setUniformValue("viewMatrix", viewMatrix);
    shaderProgram->setUniformValue("modelMatrix", modelMatrix);

    // Set color uniforms based on user settings
    QColor edgeColor = renderSetting->getEdgesColor();
    shaderProgram->setUniformValue("edgeColor", edgeColor.redF(),
                                   edgeColor.greenF(), edgeColor.blueF(), 1.0f);

    QColor vertexColor = renderSetting->getVerticesColor();
    shaderProgram->setUniformValue("vertexColor", vertexColor.redF(),
                                   vertexColor.greenF(), vertexColor.blueF(),
                                   1.0f);

    // Bind VAO
    vao.bind();

    // Draw edges if enabled
    if (renderSetting->getEdgesType() != "none" && indexCount > 0) {
      shaderProgram->setUniformValue("renderMode", 0);  // Edges mode

      if (renderSetting->getEdgesType() == "dashed") {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0x00FF);
      } else {
        glDisable(GL_LINE_STIPPLE);
      }

      glLineWidth(renderSetting->getEdgesSize());

      // Bind index buffer and draw elements
      ebo.bind();
      glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_INT, nullptr);
      ebo.release();
    }

    // Draw vertices if enabled
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

    // Unbind VAO and shader program
    vao.release();
    shaderProgram->release();

    // Disable depth testing after rendering
    glDisable(GL_DEPTH_TEST);
  }

 private:
  /// Shared pointer to user settings for rendering
  std::shared_ptr<UserSetting> renderSetting;
  /// Shared pointer to the scene data to be rendered
  std::shared_ptr<s21::DrawSceneData> scene;

  // Modern OpenGL members
  /// Vertex Array Object for storing vertex attribute configuration
  QOpenGLVertexArrayObject vao;
  /// Vertex Buffer Object for vertex data
  QOpenGLBuffer vbo{QOpenGLBuffer::VertexBuffer};
  /// Element Buffer Object for index data
  QOpenGLBuffer ebo{QOpenGLBuffer::IndexBuffer};
  /// Shader program used for rendering
  QOpenGLShaderProgram *shaderProgram = nullptr;

  /// Projection transformation matrix
  QMatrix4x4 projectionMatrix;
  /// View (camera) transformation matrix
  QMatrix4x4 viewMatrix;
  /// Model transformation matrix
  QMatrix4x4 modelMatrix;

  /// Flag indicating if the buffer data needs to be updated
  bool needBufferUpdate = false;
  /// Number of vertices in the current scene
  int vertexCount = 0;
  /// Number of indices in the current scene
  int indexCount = 0;
  /// Condition to make projection matrix for a 4:3 aspect ratio
  bool isGifRatio = false;

  /**
   * @brief Initializes the shader program.
   *
   * Compiles and links the vertex and fragment shaders, and sets up the shader
   * program.
   */
  void initShaders() {
    shaderProgram = new QOpenGLShaderProgram(this);

    // Vertex shader source code
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

    // Fragment shader source code
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

  /**
   * @brief Updates the OpenGL buffer objects with the current scene data.
   *
   * Allocates and uploads vertex and index data to the GPU.
   */
  void updateBuffers() {
    if (!scene || scene->vertices.empty()) return;

    vertexCount = scene->vertices.size() / 3;
    indexCount = scene->vertex_indices.size();

    if (vertexCount == 0) return;

    // Ensure a valid shader program is available
    if (!shaderProgram || !shaderProgram->isLinked()) {
      initShaders();
    }

    // Bind VAO to store buffer configuration
    vao.bind();

    // Update vertex buffer
    vbo.bind();
    vbo.allocate(scene->vertices.data(),
                 scene->vertices.size() * sizeof(float));

    // Set vertex attribute pointer for position (location = 0)
    shaderProgram->enableAttributeArray(0);
    shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);
    vbo.release();

    // Update index buffer if indices are available
    if (indexCount > 0) {
      ebo.bind();
      ebo.allocate(scene->vertex_indices.data(),
                   scene->vertex_indices.size() * sizeof(unsigned int));
      ebo.release();
    }

    // Release VAO
    vao.release();
  }

  /**
   * @brief Updates the projection matrix based on the current widget
   * dimensions.
   *
   * Chooses between orthographic and perspective projections based on user
   * settings.
   */
  void updateProjectionMatrix() {
    projectionMatrix.setToIdentity();

    float aspect = static_cast<float>(width()) /
                   (isGifRatio ? (width() * 3 / 4) : height());

    if (renderSetting->isParallelProjectrion()) {
      // Orthographic projection parameters
      float size = 1.0f;
      projectionMatrix.ortho(-size * aspect, size * aspect, -size, size, 1.0f,
                             100.0f);
    } else {
      // Perspective projection parameters
      projectionMatrix.perspective(45.0f, aspect, 1.0f, 100.0f);
    }

    // Set the view matrix (camera transformation)
    viewMatrix.setToIdentity();
    viewMatrix.translate(0.0f, 0.0f, -2.0f);

    // Update the model matrix
    UpdateModelMatrix();
  }
};
