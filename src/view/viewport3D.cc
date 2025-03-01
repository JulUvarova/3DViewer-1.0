#include "viewport3D.h"

Viewport3D::Viewport3D(std::shared_ptr<UserSetting> setting, QWidget *parent)
    : QOpenGLWidget(parent), renderSetting_(setting) {}

void Viewport3D::SetScene(std::shared_ptr<s21::DrawSceneData> sc) {
  scene_ = std::move(sc);
  needBufferUpdate_ = true;
  update();  // Request a repaint
}

void Viewport3D::ChangeAspectRatio(bool isGif) {
  isGifRatio_ = isGif;
  Repaint();
}

void Viewport3D::UpdateModelMatrix() {
  static float last_tx = 0, last_ty = 0, last_tz = 0;
  static float last_rx = 0, last_ry = 0, last_rz = 0;
  static float last_sx = 1, last_sy = 1, last_sz = 1;

  auto [tx, ty, tz, rx, ry, rz, sx, sy, sz] =
      s21::Controller::GetInstance()->GetSceneParameters();

  // Only update the matrix if any parameter has changed
  if (tx != last_tx || ty != last_ty || tz != last_tz || rx != last_rx ||
      ry != last_ry || rz != last_rz || sx != last_sx || sy != last_sy ||
      sz != last_sz) {
    // Save current parameters for next comparison
    last_tx = tx;
    last_ty = ty;
    last_tz = tz;
    last_rx = rx;
    last_ry = ry;
    last_rz = rz;
    last_sx = sx;
    last_sy = sy;
    last_sz = sz;

    modelMatrix_.setToIdentity();

    // Apply translation
    modelMatrix_.translate(tx, ty, tz);
    // Apply scaling
    modelMatrix_.scale(sx, sy, sz);

    // Apply rotation (converted from radians to degrees)
    modelMatrix_.rotate(qRadiansToDegrees(rx), 1.0f, 0.0f, 0.0f);
    modelMatrix_.rotate(qRadiansToDegrees(ry), 0.0f, 1.0f, 0.0f);
    modelMatrix_.rotate(qRadiansToDegrees(rz), 0.0f, 0.0f, 1.0f);
  }
}

void Viewport3D::Repaint() {
  UpdateProjectionMatrix();
  update();
}

void Viewport3D::SetBackColor() {
  glClearColor(renderSetting_->GetBackgroundColor().red() / 255.0,
               renderSetting_->GetBackgroundColor().green() / 255.0,
               renderSetting_->GetBackgroundColor().blue() / 255.0, 1.0);
}

void Viewport3D::initializeGL() {
  initializeOpenGLFunctions();
  SetBackColor();
  glEnable(GL_DEPTH_TEST);
  InitShaders();

  // Create VAO and VBO
  if (!vao_.isCreated()) vao_.create();
  if (!vbo_.isCreated()) vbo_.create();
  if (!ebo_.isCreated()) ebo_.create();
}

void Viewport3D::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);
  UpdateProjectionMatrix();
}

void Viewport3D::paintGL() {
  SetBackColor();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (!scene_ || scene_->vertices.empty() || scene_->vertex_indices.empty())
    return;

  // Update buffers if needed
  if (needBufferUpdate_) {
    UpdateBuffers();
    needBufferUpdate_ = false;
  }

  // Enable depth testing once
  glEnable(GL_DEPTH_TEST);

  // Bind shader program once
  shaderProgram_->bind();

  // Set transformation uniforms - these are the same for both rendering passes
  shaderProgram_->setUniformValue("projectionMatrix", projectionMatrix_);
  shaderProgram_->setUniformValue("viewMatrix", viewMatrix_);
  shaderProgram_->setUniformValue("modelMatrix", modelMatrix_);

  // Bind VAO once
  vao_.bind();

  // Draw edges if enabled
  if (renderSetting_->GetEdgesType() != "none" && indexCount_ > 0) {
    shaderProgram_->setUniformValue("renderMode", 0);  // Edges mode
    QColor edgeColor = renderSetting_->GetEdgesColor();
    shaderProgram_->setUniformValue("edgeColor", edgeColor.redF(),
                                    edgeColor.greenF(), edgeColor.blueF(),
                                    1.0f);

    if (renderSetting_->GetEdgesType() == "dashed") {
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(1, 0x00FF);
    }

    glLineWidth(renderSetting_->GetEdgesSize());

    // Bind index buffer and draw elements
    ebo_.bind();
    glDrawElements(GL_LINES, indexCount_, GL_UNSIGNED_INT, nullptr);
    ebo_.release();

    // Disable stippling if it was enabled
    if (renderSetting_->GetEdgesType() == "dashed") {
      glDisable(GL_LINE_STIPPLE);
    }
  }

  // Draw vertices if enabled
  if (renderSetting_->GetVerticesType() != "none" && vertexCount_ > 0) {
    shaderProgram_->setUniformValue("renderMode", 1);  // Vertices mode
    QColor vertexColor = renderSetting_->GetVerticesColor();
    shaderProgram_->setUniformValue("vertexColor", vertexColor.redF(),
                                    vertexColor.greenF(), vertexColor.blueF(),
                                    1.0f);

    bool usePointSmooth = renderSetting_->GetVerticesType() == "circle";
    if (usePointSmooth) {
      glEnable(GL_POINT_SMOOTH);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    glPointSize(renderSetting_->GetVerticesSize());
    glDrawArrays(GL_POINTS, 0, vertexCount_);

    if (usePointSmooth) {
      glDisable(GL_BLEND);
      glDisable(GL_POINT_SMOOTH);
    }
  }

  // Unbind VAO and shader program
  vao_.release();
  shaderProgram_->release();

  // Disable depth testing after rendering
  glDisable(GL_DEPTH_TEST);
}

void Viewport3D::InitShaders() {
  // Only initialize shaders if they don't exist
  if (shaderProgram_ && shaderProgram_->isLinked()) {
    return;
  }

  if (shaderProgram_) {
    delete shaderProgram_;
  }

  shaderProgram_ = new QOpenGLShaderProgram(this);

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

  if (!shaderProgram_->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                               vertexShaderSource) ||
      !shaderProgram_->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                               fragmentShaderSource) ||
      !shaderProgram_->link()) {
    qDebug() << "Shader program failed to compile or link:";
    qDebug() << shaderProgram_->log();
  }
}

void Viewport3D::UpdateBuffers() {
  if (!scene_ || scene_->vertices.empty()) return;

  vertexCount_ = scene_->vertices.size() / 3;
  indexCount_ = scene_->vertex_indices.size();

  if (vertexCount_ == 0) return;

  // Ensure a valid shader program is available
  if (!shaderProgram_ || !shaderProgram_->isLinked()) {
    InitShaders();
  }

  // Bind VAO to store buffer configuration
  vao_.bind();

  // Update vertex buffer - only reallocate if size changed
  static size_t lastVertexSize = 0;
  const size_t currentVertexSize = scene_->vertices.size() * sizeof(float);

  vbo_.bind();
  if (currentVertexSize != lastVertexSize) {
    vbo_.allocate(scene_->vertices.data(), currentVertexSize);
    lastVertexSize = currentVertexSize;

    // Set vertex attribute pointer for position (location = 0)
    shaderProgram_->enableAttributeArray(0);
    shaderProgram_->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);
  } else {
    // Just update the data without reallocating
    vbo_.write(0, scene_->vertices.data(), currentVertexSize);
  }
  vbo_.release();

  // Update index buffer if indices are available - only reallocate if size
  // changed
  if (indexCount_ > 0) {
    static size_t lastIndexSize = 0;
    const size_t currentIndexSize =
        scene_->vertex_indices.size() * sizeof(unsigned int);

    ebo_.bind();
    if (currentIndexSize != lastIndexSize) {
      ebo_.allocate(scene_->vertex_indices.data(), currentIndexSize);
      lastIndexSize = currentIndexSize;
    } else {
      // Just update the data without reallocating
      ebo_.write(0, scene_->vertex_indices.data(), currentIndexSize);
    }
    ebo_.release();
  }

  // Release VAO
  vao_.release();
}

void Viewport3D::UpdateProjectionMatrix() {
  projectionMatrix_.setToIdentity();

  float aspect = static_cast<float>(width()) /
                 (isGifRatio_ ? (width() * 3 / 4) : height());

  if (renderSetting_->IsParallelProjectrion()) {
    // Orthographic projection parameters
    float size = 1.0f;
    projectionMatrix_.ortho(-size * aspect, size * aspect, -size, size, 1.0f,
                            100.0f);
  } else {
    // Perspective projection parameters
    projectionMatrix_.perspective(45.0f, aspect, 1.0f, 100.0f);
  }

  // Set the view matrix (camera transformation)
  viewMatrix_.setToIdentity();
  viewMatrix_.translate(0.0f, 0.0f, -2.0f);

  // Update the model matrix
  UpdateModelMatrix();
}