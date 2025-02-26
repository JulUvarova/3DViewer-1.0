#include "MainWindow.h"

MainWindow::MainWindow(std::shared_ptr<s21::Controller> ctrl, QWidget *parent)
    : QMainWindow(parent), controller(ctrl) {
  setupUI();

  saveLayout();

  // timer for gif animation
  timer = new QTimer(this);
  timer->setInterval(100);  // 1000ms = 1 sec (100 -> 10 fps)
  connect(timer, &QTimer::timeout, this, &MainWindow::grabScene);

  // Settings
  connect(resetCoordsButton, &QPushButton::clicked, this,
          &MainWindow::resetCoords);
  connect(resetElemsButton, &QPushButton::clicked, this,
          &MainWindow::resetUserSettings);
  connect(restoreElemsButton, &QPushButton::clicked, this,
          &MainWindow::restoreUserSettings);
  connect(saveElemsButton, &QPushButton::clicked, this,
          &MainWindow::saveUserSettings);

  // Location coordinates
  connect(
      locationSlidersBox, &SlidersBox::signalChangeX, this,
      [this](int value) { slotTransform(TransformType::LocationX, value); });
  connect(
      locationSlidersBox, &SlidersBox::signalChangeY, this,
      [this](int value) { slotTransform(TransformType::LocationY, value); });
  connect(
      locationSlidersBox, &SlidersBox::signalChangeZ, this,
      [this](int value) { slotTransform(TransformType::LocationZ, value); });
  connect(centralWindow, &CentralWindow::signalChangeMoveCoords, locationSlidersBox,
          &SlidersBox::setCoords);

  // scale coordinates
  connect(scaleSlidersBox, &SlidersBox::signalChangeX, this,
          [this](int value) { slotTransform(TransformType::Scale, value); });
  connect(centralWindow, &CentralWindow::signalChangeScaleCoords, scaleSlidersBox,
          &SlidersBox::setCoords);

  // rotate coordinates

  connect(
      rotateSlidersBox, &SlidersBox::signalChangeX, this,
      [this](int value) { slotTransform(TransformType::RotationX, value); });
  connect(
      rotateSlidersBox, &SlidersBox::signalChangeY, this,
      [this](int value) { slotTransform(TransformType::RotationY, value); });
  connect(
      rotateSlidersBox, &SlidersBox::signalChangeZ, this,
      [this](int value) { slotTransform(TransformType::RotationZ, value); });
  connect(centralWindow, &CentralWindow::signalChangeRotateCoords, rotateSlidersBox,
          &SlidersBox::setCoords);

  // vertex prop
  connect(verticesBox, &ElemBox::signalChangeType, this,
          &MainWindow::slotVerticesType);
  connect(verticesBox, &ElemBox::signalChangeSize, this,
          &MainWindow::slotVerticesSize);
  connect(verticesBox, &ElemBox::signalChangeColor, this,
          &MainWindow::slotVerticesColor);

  // edges prop
  connect(edgesBox, &ElemBox::signalChangeType, this,
          &MainWindow::slotEdgesType);
  connect(edgesBox, &ElemBox::signalChangeSize, this,
          &MainWindow::slotEdgesSize);
  connect(edgesBox, &ElemBox::signalChangeColor, this,
          &MainWindow::slotEdgesColor);
  connect(backBox, &BackgroundBox::signalChangeColor, this,
          &MainWindow::slotBackgroundColor);

  // perspective
  connect(centralProj, &QRadioButton::clicked, this, [this]() {
    userSetting->setProjection(false);
    renderWindow->repaint();
  });
  connect(perspectiveProj, &QRadioButton::clicked, this, [this]() {
    userSetting->setProjection(true);
    renderWindow->repaint();
  });

  // connect(sceneInfoButton, &QPushButton::clicked, propsObjectsInfo,
  //         &QLabel::show);
}

void MainWindow::resetCoords() {
  controller->ResetScene();

  scaleSlidersBox->resetCoords();
  locationSlidersBox->resetCoords();
  rotateSlidersBox->resetCoords();

  renderWindow->update();
}

void MainWindow::slotBackgroundColor(const QColor &color) {
  userSetting->setBackgroundColor(color);

  renderWindow->update();
}

void MainWindow::slotVerticesColor(const QColor &color) {
  userSetting->setVerticesColor(color);

  renderWindow->update();
}

void MainWindow::slotVerticesType(const QString &text) {
  userSetting->setVerticesType(text);

  renderWindow->update();
}

void MainWindow::slotVerticesSize(const int value) {
  userSetting->setVerticesSize(value);

  renderWindow->update();
}

void MainWindow::slotEdgesColor(const QColor &color) {
  userSetting->setEdgesColor(color);

  renderWindow->update();
}

void MainWindow::slotEdgesType(const QString &text) {
  userSetting->setEdgesType(text);

  renderWindow->update();
}

void MainWindow::slotEdgesSize(const int value) {
  userSetting->setEdgesSize(value);

  renderWindow->update();
}

void MainWindow::slotTransform(TransformType type, int value) {
  switch (type) {
    case TransformType::LocationX:
      controller->SetLocationX(value);
      break;
    case TransformType::LocationY:
      controller->SetLocationY(value);
      break;
    case TransformType::LocationZ:
      controller->SetLocationZ(value);
      break;
    case TransformType::RotationX:
      controller->SetRotationX(value);
      break;
    case TransformType::RotationY:
      controller->SetRotationY(value);
      break;
    case TransformType::RotationZ:
      controller->SetRotationZ(value);
      break;
    case TransformType::Scale:
      controller->SetScaleX(value);
      break;
  }
  renderWindow->UpdateModelMatrix();
  renderWindow->update();
}

void MainWindow::setupUI() {
  // read saved settings
  userSetting = std::make_shared<UserSetting>();

  // Window properties
  setWindowTitle("3DViewer 2.0");
  resize(1280, 720);

  // Central 3D viewport
  renderWindow = new Viewport3D(userSetting, this);
  centralWindow = new CentralWindow(renderWindow);
  setCentralWidget(centralWindow);

  // UI components
  createDockWidgets();
  createStatusBar();

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
  controller->SetSceneUpdateCallback(
      [this](const std::shared_ptr<s21::DrawSceneData> &sceneData) {
        // This will be called whenever the scene is transformed
        renderWindow->setScene(sceneData);
      });
}

void MainWindow::createDockWidgets() {
  // Left dock (Tools)
  toolsDock = new QDockWidget(this);
  toolsDock->setObjectName("toolsDock");
  toolsDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  addDockWidget(Qt::LeftDockWidgetArea, toolsDock);
  // Create sliders
  locationSlidersBox =
      new SlidersBox("Location", std::pair<int, int>{-100, 100}, 3, this);
  rotateSlidersBox =
      new SlidersBox("Rotate", std::pair<int, int>{-180, 180}, 3, this);
  scaleSlidersBox =
      new SlidersBox("Scale", std::pair<int, int>{1, 200}, 1, this);

  // Create projection choose
  QGroupBox *projBox = new QGroupBox("Projection", this);
  centralProj = new QRadioButton("Central", this);
  perspectiveProj = new QRadioButton("Perspective", this);
  QVBoxLayout *projLayout = new QVBoxLayout();
  projLayout->addWidget(centralProj);
  projLayout->addWidget(perspectiveProj);
  projBox->setLayout(projLayout);
  perspectiveProj->setChecked(true);

  // Create verticesBox
  QStringList verticesLst;
  verticesLst << "circle" << "square" << "none";
  Setting verticesSetting{userSetting->getVerticesType(),
                          userSetting->getVerticesColor(),
                          userSetting->getVerticesSize()};
  verticesBox = new ElemBox("Vertices", verticesLst, verticesSetting, this);

  // Create EdgeBox
  QStringList edgesLst;
  edgesLst << "line" << "dashed" << "none";
  Setting edgesSetting{userSetting->getEdgesType(),
                       userSetting->getEdgesColor(),
                       userSetting->getEdgesSize()};
  edgesBox = new ElemBox("Edges", edgesLst, edgesSetting, this);

  // create backgroundBox
  backBox =
      new BackgroundBox("Background", userSetting->getBackgroundColor(), this);

  // reset buttons
  resetCoordsButton = new QPushButton("Reset coords");
  resetElemsButton = new QPushButton("Reset settings");
  restoreElemsButton = new QPushButton("Restore settings");
  saveElemsButton = new QPushButton("Save settings");

  // Fill left dock
  QWidget *toolBox = new QWidget();
  toolBox->setLayout(new QVBoxLayout);
  toolBox->layout()->addWidget(locationSlidersBox);
  toolBox->layout()->addWidget(rotateSlidersBox);
  toolBox->layout()->addWidget(scaleSlidersBox);
  toolBox->layout()->addWidget(resetCoordsButton);
  toolBox->layout()->addWidget(projBox);
  toolBox->layout()->addWidget(verticesBox);
  toolBox->layout()->addWidget(edgesBox);
  toolBox->layout()->addWidget(backBox);
  toolBox->layout()->addWidget(saveElemsButton);
  toolBox->layout()->addWidget(restoreElemsButton);
  toolBox->layout()->addWidget(resetElemsButton);

  toolsDock->setWidget(toolBox);

  // block close
  toolsDock->setFeatures(QDockWidget::DockWidgetMovable |
                         QDockWidget::DockWidgetFloatable);
}

void MainWindow::createStatusBar() {
  QStatusBar *propBox = new QStatusBar(this);

  propsFileInfo = new QLabel(propBox);
  propsFileInfo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  propsFileInfo->setWordWrap(true);
  propsFileInfo->setTextInteractionFlags(Qt::TextSelectableByMouse);

  sceneInfoButton = new QPushButton("Scene Info", propBox);
  sceneInfoButton->setFixedSize(80, 30);

  QLabel *fileNameLabel = new QLabel("File:", propBox);
  fileNameLabel->adjustSize();

  warningInfo = new QLabel("REZERVED", propBox);

  propBox->addWidget(warningInfo);
  propBox->addPermanentWidget(fileNameLabel);
  propBox->addPermanentWidget(propsFileInfo);
  propBox->addPermanentWidget(sceneInfoButton);

  setStatusBar(propBox);
  statusBar()->setSizeGripEnabled(false);
}

void MainWindow::appExit() {
  userSetting->saveRenderSettings();
  close();
}

void MainWindow::openFile() {
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), "./",
                                                  tr("Images (*.obj)"));
  //! if cancel
  if (fileName.isEmpty()) return;

  QByteArray byteArray = fileName.toUtf8();
  const char *cstr = byteArray.constData();
  // QMessageBox::information(this, tr("Unable to open file"),
  //                          "File is not opened =(");
  resetCoords();
  auto scene = controller->LoadScene(cstr);
  renderWindow->setScene(scene);

  //! check canOpen
  // if (!isSaved)

  propsFileInfo->setText(fileName);
  // propsObjectsInfo->setText(QString::fromStdString(scene->info));
}

void MainWindow::getFileName(const char *options) {
  QString selectedFilter;
  fileName = QFileDialog::getSaveFileName(this, tr("Choose folder"), "./",
                                          tr(options), &selectedFilter);
  if (fileName.isEmpty()) return;

  if (selectedFilter == "*.bmp")
    fileName.append(".bmp");
  else if (selectedFilter == "*.jpeg")
    fileName.append(".jpeg");
  else if (selectedFilter == "*.gif")
    fileName.append(".gif");
  else
    QMessageBox::information(this, tr("Wrong file name"),
                             "Filename isn't correct");
}

void MainWindow::saveImage() {
  getFileName("*.jpeg;;*.bmp");
  if (fileName.isEmpty()) return;

  renderWindow->beforeGrab();
  bool isSaved = renderWindow->grab().save(fileName);
  if (!isSaved)
    QMessageBox::information(this, tr("Unable to save file"),
                             "File is not saved =(");
  else
    //! отладка
    QMessageBox::information(this, tr("File saved"), "GIF is saved! =)");

  renderWindow->afterGrab();
}

void MainWindow::saveCustomGif() {
  getFileName("*.gif");
  if (fileName.isEmpty()) return;

  timer->start();
}

void MainWindow::grabScene() {
  renderWindow->beforeGrab();
  screens.push_back(renderWindow->grab());
  renderWindow->afterGrab();

  if (screens.size() == 50) {
    timer->stop();

    createGifFile();

    screens.clear();
    //! отладка
    QMessageBox::information(this, tr("File saved"), "File is saved! =)");
  }
}

void MainWindow::saveCycledGif() {
  getFileName("*.gif");
  if (fileName.isEmpty()) return;

  std::array<int, 3> coords;

  coords = locationSlidersBox->getCoords();
  std::pair<float, float> locationX(0, coords[0] / 25.0);
  std::pair<float, float> locationY(0, coords[1] / 25.0);
  std::pair<float, float> locationZ(0, coords[2] / 25.0);

  coords = rotateSlidersBox->getCoords();
  std::pair<float, float> rotateX(0, coords[0] / 25.0);
  std::pair<float, float> rotateY(0, coords[1] / 25.0);
  std::pair<float, float> rotateZ(0, coords[2] / 25.0);

  coords = scaleSlidersBox->getCoords();
  std::pair<float, float> scaleX(100, (coords[0] - 100) / 25.0);

  screens.resize(50);

  controller->ResetScene();
  renderWindow->beforeGrab();
  screens[0] = renderWindow->grab();
  renderWindow->afterGrab();

  for (int i = 1; i <= 25; ++i) {
    controller->SetScaleX(scaleX.first += scaleX.second);

    controller->SetRotationX(rotateX.first += rotateX.second);
    controller->SetRotationY(rotateY.first += rotateY.second);
    controller->SetRotationZ(rotateZ.first += rotateZ.second);

    controller->SetLocationX(locationX.first += locationX.second);
    controller->SetLocationY(locationY.first += locationY.second);
    controller->SetLocationZ(locationZ.first += locationZ.second);

    renderWindow->update();

    renderWindow->beforeGrab();
    screens[i] = renderWindow->grab();
    screens[50 - i] = renderWindow->grab();
    renderWindow->afterGrab();
  }
  createGifFile();
  screens.clear();
  //! отладка
  QMessageBox::information(this, tr("File saved"), "File is saved! =)");
}

void MainWindow::createGifFile() {
  QByteArray byteArray = fileName.toUtf8();
  const char *cstr = byteArray.constData();

  GifWriter gif;
  GifBegin(&gif, cstr, 640, 480, 10);
  for (const auto &screen : screens) {
    // pixmap->QImage->scale 640x480->colors
    QImage scaledImage = screen.toImage()
                             .scaled(QSize(640, 480))
                             .convertToFormat(QImage::Format_RGBA8888);
    GifWriteFrame(&gif, scaledImage.bits(), 640, 480, 0);
  }
  GifEnd(&gif);
}

void MainWindow::saveLayout() {
  userSetting->setLayoutState(saveState());
  userSetting->saveLayoutSettings();
}

void MainWindow::restoreLayout() {
  userSetting->readLayoutSettings();
  restoreState(userSetting->getLayoutState());
}

void MainWindow::resetUserSettings() {
  userSetting->removeRenderSettings();
  userSetting->readRenderSettings();
  setVisualParameters();
  renderWindow->update();
}

void MainWindow::restoreUserSettings() {
  userSetting->readRenderSettings();
  setVisualParameters();
  renderWindow->update();
}

void MainWindow::saveUserSettings() { userSetting->saveRenderSettings(); }

void MainWindow::closeEvent(QCloseEvent *event) {
  userSetting->saveRenderSettings();
  userSetting->removeLayoutSettings();
  QMainWindow::closeEvent(event);
}

void MainWindow::setVisualParameters() {
  Setting edgesSetting{userSetting->getEdgesType(),
                       userSetting->getEdgesColor(),
                       userSetting->getEdgesSize()};
  edgesBox->setSetting(edgesSetting);

  Setting verticesSetting{userSetting->getVerticesType(),
                          userSetting->getVerticesColor(),
                          userSetting->getVerticesSize()};
  verticesBox->setSetting(verticesSetting);

  backBox->setSetting(userSetting->getBackgroundColor());
}