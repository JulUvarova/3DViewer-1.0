#pragma once

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <memory>

#include "Logger.h"
#include "controller.h"
#include "scene.h"
#include "user_setting.h"

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
  Viewport3D(std::shared_ptr<UserSetting> setting, QWidget *parent = nullptr);

  /**
   * @brief Sets the scene to be rendered.
   *
   * This function updates the internal scene data and requests a repaint.
   *
   * @param sc Shared pointer to the new scene data.
   */
  void SetScene(std::shared_ptr<s21::DrawSceneData> sc);

  /**
   * @brief Change projection matrix before and after grabbing the screen.
   *
   * Adjusts the projection matrix for a 4:3 aspect ratio, useful for capturing
   * a screenshot.
   * @param ifGif Change condition for count aspect ration.
   */
  void ChangeAspectRatio(bool isGif);

  /**
   * @brief Updates the model transformation matrix.
   *
   * Resets the model matrix and applies translation, scaling, and rotation
   * based on the current scene parameters.
   */
  void UpdateModelMatrix();

  /**
   * @brief Repaints the viewport.
   *
   * This function updates the projection matrix and requests a repaint.
   */
  void Repaint();

 protected:
  /**
   * @brief Sets the background color for the OpenGL context.
   *
   * Retrieves the background color from user settings and applies it.
   */
  void SetBackColor();
  /**
   * @brief Initializes the OpenGL context.
   *
   * Sets up OpenGL functions, background color, depth testing, shaders,
   * and creates necessary vertex and index buffers.
   */
  void initializeGL() override;

  /**
   * @brief Handles widget resize events.
   *
   * Adjusts the OpenGL viewport and updates the projection matrix accordingly.
   *
   * @param w New width.
   * @param h New height.
   */
  void resizeGL(int w, int h) override;

  /**
   * @brief Renders the scene using OpenGL.
   *
   * Clears the screen, updates buffers if needed, sets shader uniforms,
   * and draws the edges and vertices based on the current rendering settings.
   */
  void paintGL() override;

 private:
  /// Shared pointer to user settings for rendering
  std::shared_ptr<UserSetting> renderSetting_;
  /// Shared pointer to the scene data to be rendered
  std::shared_ptr<s21::DrawSceneData> scene_;

  // Modern OpenGL members
  /// Vertex Array Object for storing vertex attribute configuration
  QOpenGLVertexArrayObject vao_;
  /// Vertex Buffer Object for vertex data
  QOpenGLBuffer vbo_{QOpenGLBuffer::VertexBuffer};
  /// Element Buffer Object for index data
  QOpenGLBuffer ebo_{QOpenGLBuffer::IndexBuffer};
  /// Shader program used for rendering
  QOpenGLShaderProgram *shaderProgram_ = nullptr;

  /// Projection transformation matrix
  QMatrix4x4 projectionMatrix_;
  /// View (camera) transformation matrix
  QMatrix4x4 viewMatrix_;
  /// Model transformation matrix
  QMatrix4x4 modelMatrix_;

  /// Flag indicating if the buffer data needs to be updated
  bool needBufferUpdate_ = false;
  /// Number of vertices in the current scene
  int vertexCount_ = 0;
  /// Number of indices in the current scene
  int indexCount_ = 0;
  /// Condition to make projection matrix for a 4:3 aspect ratio
  bool isGifRatio_ = false;

  /**
   * @brief Initializes the shader program.
   *
   * Compiles and links the vertex and fragment shaders, and sets up the shader
   * program.
   */
  void InitShaders();

  /**
   * @brief Updates the OpenGL buffer objects with the current scene data.
   *
   * Allocates and uploads vertex and index data to the GPU.
   */
  void UpdateBuffers();

  /**
   * @brief Updates the projection matrix based on the current widget
   * dimensions.
   *
   * Chooses between orthographic and perspective projections based on user
   * settings.
   */
  void UpdateProjectionMatrix();
};
