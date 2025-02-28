#include "main_window.h"

MainWindow::MainWindow(std::shared_ptr<s21::Controller> ctrl, QWidget *parent)
    : QMainWindow(parent), controller_(ctrl) {
  SetupUI();

  // timer for gif animation
  timer_ = new QTimer(this);
  timer_->setInterval(100);  // 1000ms = 1 sec (100 -> 10 fps)
  connect(timer_, &QTimer::timeout, this, &MainWindow::GrabScene);

  // Settings
  connect(ResetCoordsButton_, &QPushButton::clicked, this,
          &MainWindow::ResetCoords);
  connect(resetElemsButton_, &QPushButton::clicked, this,
          &MainWindow::ResetUserSettings);
  connect(restoreElemsButton_, &QPushButton::clicked, this,
          &MainWindow::RestoreUserSettings);
  connect(saveElemsButton_, &QPushButton::clicked, this,
          &MainWindow::SaveUserSettings);

  // Location coordinates
  connect(
      locationSlidersBox_, &SlidersBox::signalChangeX, this,
      [this](int value) { slotTransform(TransformType::LocationX, value); });
  connect(
      locationSlidersBox_, &SlidersBox::signalChangeY, this,
      [this](int value) { slotTransform(TransformType::LocationY, value); });
  connect(
      locationSlidersBox_, &SlidersBox::signalChangeZ, this,
      [this](int value) { slotTransform(TransformType::LocationZ, value); });
  connect(controlWindow_, &ControlWindow::signalChangeMoveCoords,
          locationSlidersBox_, &SlidersBox::SetCoords);

  // scale coordinates
  connect(scaleSlidersBox_, &SlidersBox::signalChangeX, this,
          [this](int value) { slotTransform(TransformType::Scale, value); });
  connect(controlWindow_, &ControlWindow::signalChangeScaleCoords,
          scaleSlidersBox_, &SlidersBox::SetCoords);

  // rotate coordinates

  connect(
      rotateSlidersBox_, &SlidersBox::signalChangeX, this,
      [this](int value) { slotTransform(TransformType::RotationX, value); });
  connect(
      rotateSlidersBox_, &SlidersBox::signalChangeY, this,
      [this](int value) { slotTransform(TransformType::RotationY, value); });
  connect(
      rotateSlidersBox_, &SlidersBox::signalChangeZ, this,
      [this](int value) { slotTransform(TransformType::RotationZ, value); });
  connect(controlWindow_, &ControlWindow::signalChangeRotateCoords,
          rotateSlidersBox_, &SlidersBox::SetCoords);

  // vertex prop
  connect(verticesBox_, &ElemBox::signalChangeType, this,
          &MainWindow::slotVerticesType);
  connect(verticesBox_, &ElemBox::signalChangeSize, this,
          &MainWindow::slotVerticesSize);
  connect(verticesBox_, &ElemBox::signalChangeColor, this,
          &MainWindow::slotVerticesColor);

  // edges prop
  connect(edgesBox_, &ElemBox::signalChangeType, this,
          &MainWindow::slotEdgesType);
  connect(edgesBox_, &ElemBox::signalChangeSize, this,
          &MainWindow::slotEdgesSize);
  connect(edgesBox_, &ElemBox::signalChangeColor, this,
          &MainWindow::slotEdgesColor);
  connect(backgroundBox_, &BackgroundBox::signalChangeColor, this,
          &MainWindow::slotBackgroundColor);

  // perspective
  connect(perspectiveProj_, &QRadioButton::clicked, this, [this]() {
    userSetting_->SetProjection(false);
    renderWindow_->Repaint();
  });
  connect(parallelProj_, &QRadioButton::clicked, this, [this]() {
    userSetting_->SetProjection(true);
    renderWindow_->Repaint();
  });

  // Work with file
  connect(controlWindow_, &ControlWindow::signalOpenFile, this,
          &MainWindow::LoadScene);
  connect(controlWindow_, &ControlWindow::signalSaveFile, this,
          &MainWindow::SaveImage);
  connect(controlWindow_, &ControlWindow::signalStartCycledGif, this,
          &MainWindow::SaveCycledGif);
  connect(controlWindow_, &ControlWindow::signalStartCustomGif, this,
          &MainWindow::SaveCustomGif);
  connect(sceneInfoButton_, &QPushButton::clicked, this, [this]() {
    MoveInfoWindow();
    sceneInfoWindow_->show();
  });

  connect(toolsDock_, &QDockWidget::dockLocationChanged, this,
          &MainWindow::MoveInfoWindow);
}

void MainWindow::MoveInfoWindow() {
  int wShift = 0;
  if (Qt::RightDockWidgetArea == dockWidgetArea(toolsDock_) &&
      !toolsDock_->isFloating())
    wShift = toolsDock_->width();
  sceneInfoWindow_->move(
      geometry().bottomRight() -
      QPoint(sceneInfoWindow_->width() + wShift,
             sceneInfoWindow_->height() + statusBar()->height()));
}

void MainWindow::resizeEvent(QResizeEvent *event) {
  QMainWindow::resizeEvent(event);
  MoveInfoWindow();
}

void MainWindow::moveEvent(QMoveEvent *event) {
  QMainWindow::moveEvent(event);
  MoveInfoWindow();
}

void MainWindow::ResetCoords() {
  controller_->ResetScene();

  scaleSlidersBox_->ResetCoords();
  locationSlidersBox_->ResetCoords();
  rotateSlidersBox_->ResetCoords();

  renderWindow_->update();
}

void MainWindow::slotBackgroundColor(const QColor &color) {
  userSetting_->SetBackgroundColor(color);

  renderWindow_->update();
}

void MainWindow::slotVerticesColor(const QColor &color) {
  userSetting_->SetVerticesColor(color);

  renderWindow_->update();
}

void MainWindow::slotVerticesType(const QString &text) {
  userSetting_->SetVerticesType(text);

  renderWindow_->update();
}

void MainWindow::slotVerticesSize(const int value) {
  userSetting_->SetVerticesSize(value);

  renderWindow_->update();
}

void MainWindow::slotEdgesColor(const QColor &color) {
  userSetting_->SetEdgesColor(color);

  renderWindow_->update();
}

void MainWindow::slotEdgesType(const QString &text) {
  userSetting_->SetEdgesType(text);

  renderWindow_->update();
}

void MainWindow::slotEdgesSize(const int value) {
  userSetting_->SetEdgesSize(value);

  renderWindow_->update();
}

void MainWindow::slotTransform(TransformType type, int value) {
  switch (type) {
    case TransformType::LocationX:
      controller_->SetLocationX(value);
      break;
    case TransformType::LocationY:
      controller_->SetLocationY(value);
      break;
    case TransformType::LocationZ:
      controller_->SetLocationZ(value);
      break;
    case TransformType::RotationX:
      controller_->SetRotationX(value);
      break;
    case TransformType::RotationY:
      controller_->SetRotationY(value);
      break;
    case TransformType::RotationZ:
      controller_->SetRotationZ(value);
      break;
    case TransformType::Scale:
      controller_->SetScaleX(value);
      break;
  }
  renderWindow_->UpdateModelMatrix();
  renderWindow_->update();
}

void MainWindow::SetupUI() {
  // read saved settings
  userSetting_ = std::make_shared<UserSetting>();

  // Window properties
  setWindowTitle("3DViewer 2.0");
  resize(1280, 720);

  // Central 3D viewport
  QGridLayout *mainLayout = new QGridLayout(this);
  mainLayout->setContentsMargins(0, 0, 0, 0);
  renderWindow_ = new Viewport3D(userSetting_, this);
  controlWindow_ = new ControlWindow(this);
  mainLayout->addWidget(renderWindow_);
  mainLayout->addWidget(controlWindow_, 0, 0);
  QWidget *mainWidget = new QWidget(this);
  mainWidget->setLayout(mainLayout);
  setCentralWidget(mainWidget);

  // UI components
  CreateDockWidgets();
  CreateStatusBar();

  // Apply dark theme
  setStyleSheet(R"(
    QMainWindow, QWidget {
      background-color: #323232;
      color: #ffffff;
    }
    QMenuBar, QToolBar, QDockWidget::title {
      background-color: #2d2d2d;
      border: none;
    }
    QMenuBar::item:selected, QMenu::item:selected {
      background-color: #404040;
    }
    QDockWidget {
      titlebar-close-icon: url(none);
      border: 1px solid #404040;
    }
  )");

  // Connect controller's scene update callback to viewport
  controller_->SetSceneUpdateCallback(
      [this](const std::shared_ptr<s21::DrawSceneData> &sceneData) {
        // This will be called whenever the scene is transformed
        renderWindow_->SetScene(sceneData);
      });
}

void MainWindow::CreateDockWidgets() {
  // Left dock (Tools)
  toolsDock_ = new QDockWidget(this);
  toolsDock_->setObjectName("toolsDock");
  toolsDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  addDockWidget(Qt::LeftDockWidgetArea, toolsDock_);
  // Create sliders
  locationSlidersBox_ =
      new SlidersBox("Location", std::pair<int, int>{-100, 100}, 3, this);
  rotateSlidersBox_ =
      new SlidersBox("Rotate", std::pair<int, int>{-180, 180}, 3, this);
  scaleSlidersBox_ =
      new SlidersBox("Scale", std::pair<int, int>{1, 200}, 1, this);

  // Create projection choose
  QGroupBox *projBox = new QGroupBox("Projection", this);
  perspectiveProj_ = new QRadioButton("Perspective", this);
  parallelProj_ = new QRadioButton("Parallel", this);
  QVBoxLayout *projLayout = new QVBoxLayout();
  projLayout->addWidget(perspectiveProj_);
  projLayout->addWidget(parallelProj_);
  projBox->setLayout(projLayout);
  parallelProj_->setChecked(true);

  // Create verticesBox
  QStringList verticesLst;
  verticesLst << "circle" << "square" << "none";
  Setting verticesSetting{userSetting_->GetVerticesType(),
                          userSetting_->GetVerticesColor(),
                          userSetting_->GetVerticesSize()};
  verticesBox_ = new ElemBox("Vertices", verticesLst, verticesSetting, this);

  // Create EdgeBox
  QStringList edgesLst;
  edgesLst << "line" << "dashed" << "none";
  Setting edgesSetting{userSetting_->GetEdgesType(),
                       userSetting_->GetEdgesColor(),
                       userSetting_->GetEdgesSize()};
  edgesBox_ = new ElemBox("Edges", edgesLst, edgesSetting, this);

  // create backgroundBox
  backgroundBox_ =
      new BackgroundBox("Background", userSetting_->GetBackgroundColor(), this);

  // reset buttons
  ResetCoordsButton_ = new QPushButton("Reset coords");
  resetElemsButton_ = new QPushButton("Reset settings");
  restoreElemsButton_ = new QPushButton("Restore settings");
  saveElemsButton_ = new QPushButton("Save settings");

  // Fill left dock
  QWidget *toolBox = new QWidget();
  toolBox->setLayout(new QVBoxLayout);
  toolBox->layout()->addWidget(locationSlidersBox_);
  toolBox->layout()->addWidget(rotateSlidersBox_);
  toolBox->layout()->addWidget(scaleSlidersBox_);
  toolBox->layout()->addWidget(ResetCoordsButton_);
  toolBox->layout()->addWidget(projBox);
  toolBox->layout()->addWidget(verticesBox_);
  toolBox->layout()->addWidget(edgesBox_);
  toolBox->layout()->addWidget(backgroundBox_);
  toolBox->layout()->addWidget(saveElemsButton_);
  toolBox->layout()->addWidget(restoreElemsButton_);
  toolBox->layout()->addWidget(resetElemsButton_);

  toolsDock_->setWidget(toolBox);

  // block close
  toolsDock_->setFeatures(QDockWidget::DockWidgetMovable |
                          QDockWidget::DockWidgetFloatable);
  toolsDock_->setFixedWidth(toolsDock_->width() * 2);
}

void MainWindow::CreateStatusBar() {
  QStatusBar *propBox = new QStatusBar(this);

  filenameInfo_ = new QLabel(propBox);
  filenameInfo_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  filenameInfo_->setWordWrap(true);
  filenameInfo_->setTextInteractionFlags(Qt::TextSelectableByMouse);

  sceneInfoButton_ = new QPushButton("Scene Info", propBox);
  sceneInfoButton_->setFixedSize(80, 30);

  QLabel *fileNameLabel = new QLabel("File:", propBox);
  fileNameLabel->adjustSize();

  warningInfo_ = new QLabel("REZERVED", propBox);  //!

  sceneInfoWindow_ = new InfoWindow(this);

  propBox->addWidget(warningInfo_);
  propBox->addPermanentWidget(fileNameLabel);
  propBox->addPermanentWidget(filenameInfo_);
  propBox->addPermanentWidget(sceneInfoButton_);

  setStatusBar(propBox);
  statusBar()->setSizeGripEnabled(false);
}

void MainWindow::LoadScene(QString &fname) {
  if (fname.isEmpty()) return;

  ResetCoords();
  try {
    auto scene = controller_->LoadScene(fname.toUtf8().data());
    renderWindow_->SetScene(scene);
    renderWindow_->Repaint();
    filenameInfo_->setText(fname);
    sceneInfoWindow_->SetText(QString::fromStdString(scene->info));
  } catch (const s21::MeshLoadException &e) {
    QMessageBox::warning(this, tr("Unable to open file"), e.what());
  }
}

void MainWindow::SaveImage(QString &fname) {
  if (fname.isEmpty()) return;

  try {
    bool isSaved = renderWindow_->grab().save(fname);
    QMessageBox::information(this, tr("File saved"), "File is saved! =)");
  } catch (const std::exception &e) {
    QMessageBox::warning(this, tr("Unable to save file"), e.what());
  }
}

void MainWindow::SaveCustomGif(QString &fname) {
  if (fname.isEmpty()) return;
  filename_ = fname;

  timer_->start();
}

void MainWindow::GrabScene() {
  renderWindow_->ChangeAspectRatio(true);
  screens_.push_back(renderWindow_->grab());
  renderWindow_->ChangeAspectRatio(false);

  if (screens_.size() == 50) {
    timer_->stop();
    try {
      CreateGifFile();
      QMessageBox::information(this, tr("File saved"), "Gif is saved");
    } catch (const std::exception &e) {
      QMessageBox::warning(this, tr("Unable to save file"), e.what());
    }
    screens_.clear();
  }
}

void MainWindow::SaveCycledGif(QString &fname) {
  if (fname.isEmpty()) return;
  filename_ = fname;

  std::array<int, 3> coords;

  coords = locationSlidersBox_->GetCoords();
  std::pair<float, float> locationX(0, coords[0] / 25.0);
  std::pair<float, float> locationY(0, coords[1] / 25.0);
  std::pair<float, float> locationZ(0, coords[2] / 25.0);

  coords = rotateSlidersBox_->GetCoords();
  std::pair<float, float> rotateX(0, coords[0] / 25.0);
  std::pair<float, float> rotateY(0, coords[1] / 25.0);
  std::pair<float, float> rotateZ(0, coords[2] / 25.0);

  coords = scaleSlidersBox_->GetCoords();
  std::pair<float, float> scaleX(100, (coords[0] - 100) / 25.0);

  screens_.resize(50);

  controller_->ResetScene();
  renderWindow_->ChangeAspectRatio(true);
  screens_[0] = renderWindow_->grab();
  renderWindow_->ChangeAspectRatio(false);

  for (int i = 1; i <= 25; ++i) {
    controller_->SetScaleX(scaleX.first += scaleX.second);

    controller_->SetRotationX(rotateX.first += rotateX.second);
    controller_->SetRotationY(rotateY.first += rotateY.second);
    controller_->SetRotationZ(rotateZ.first += rotateZ.second);

    controller_->SetLocationX(locationX.first += locationX.second);
    controller_->SetLocationY(locationY.first += locationY.second);
    controller_->SetLocationZ(locationZ.first += locationZ.second);

    renderWindow_->update();

    renderWindow_->ChangeAspectRatio(true);
    screens_[i] = renderWindow_->grab();
    screens_[50 - i] = renderWindow_->grab();
    renderWindow_->ChangeAspectRatio(false);
  }
  try {
    CreateGifFile();
    QMessageBox::information(this, tr("File saved"), "Gif is saved");
  } catch (const std::exception &e) {
    QMessageBox::warning(this, tr("Unable to save file"), e.what());
  }
  screens_.clear();
}

void MainWindow::CreateGifFile() {
  GifWriter gif;
  // LogAlert << "Creating gif file" << filename_ << std::endl;
  GifBegin(&gif, filename_.toUtf8().data(), 640, 480, 10);
  for (const auto &screen : screens_) {
    // pixmap->QImage->scale 640x480->colors
    QImage scaledImage = screen.toImage()
                             .scaled(QSize(640, 480))
                             .convertToFormat(QImage::Format_RGBA8888);
    GifWriteFrame(&gif, scaledImage.bits(), 640, 480, 0);
  }
  GifEnd(&gif);
}

void MainWindow::ResetUserSettings() {
  userSetting_->RemoveRenderSettings();
  userSetting_->ReadRenderSettings();
  SetVisualParameters();
  renderWindow_->update();
}

void MainWindow::RestoreUserSettings() {
  userSetting_->ReadRenderSettings();
  SetVisualParameters();
  renderWindow_->update();
}

void MainWindow::SaveUserSettings() { userSetting_->SaveRenderSettings(); }

void MainWindow::closeEvent(QCloseEvent *event) {
  userSetting_->SaveRenderSettings();
  QMainWindow::closeEvent(event);
}

void MainWindow::SetVisualParameters() {
  Setting edgesSetting{userSetting_->GetEdgesType(),
                       userSetting_->GetEdgesColor(),
                       userSetting_->GetEdgesSize()};
  edgesBox_->SetSetting(edgesSetting);

  Setting verticesSetting{userSetting_->GetVerticesType(),
                          userSetting_->GetVerticesColor(),
                          userSetting_->GetVerticesSize()};
  verticesBox_->SetSetting(verticesSetting);

  backgroundBox_->SetSetting(userSetting_->GetBackgroundColor());
}