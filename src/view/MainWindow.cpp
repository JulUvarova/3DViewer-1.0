#include "MainWindow.h"

#include <chrono>
#include <thread>

MainWindow::MainWindow(s21::Controller *ctrl, QWidget *parent)
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
  connect(locationSlidersBox, &SlidersBox::signalChangeX, this,
          &MainWindow::slotLocationCoordX);
  connect(locationSlidersBox, &SlidersBox::signalChangeY, this,
          &MainWindow::slotLocationCoordY);
  connect(locationSlidersBox, &SlidersBox::signalChangeZ, this,
          &MainWindow::slotLocationCoordZ);
  connect(renderWindow, &Viewport3D::signalChangeMoveCoords, locationSlidersBox,
          &SlidersBox::setCoords);

  // scale coordinates
  connect(scaleSlidersBox, &SlidersBox::signalChangeX, this,
          &MainWindow::slotScaleCoordX);
  connect(renderWindow, &Viewport3D::signalChangeScaleCoords, scaleSlidersBox,
          &SlidersBox::setCoords);

  // rotate coordinates
  connect(rotateSlidersBox, &SlidersBox::signalChangeX, this,
          &MainWindow::slotRotateCoordX);
  connect(rotateSlidersBox, &SlidersBox::signalChangeY, this,
          &MainWindow::slotRotateCoordY);
  connect(rotateSlidersBox, &SlidersBox::signalChangeZ, this,
          &MainWindow::slotRotateCoordZ);
  connect(renderWindow, &Viewport3D::signalChangeRotateCoords, rotateSlidersBox,
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
}

void MainWindow::resetCoords() {
  controller->ResetScene();

  scaleSlidersBox->resetCoords();
  locationSlidersBox->resetCoords();
  rotateSlidersBox->resetCoords();

  renderWindow->update();
}

void MainWindow::slotProjectionType(const bool isParallel) {
  userSetting->setProjection(isParallel);

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

void MainWindow::slotLocationCoordX(int coordX) {
  controller->SetLocationX(coordX);

  renderWindow->update();
}
void MainWindow::slotLocationCoordY(int coordY) {
  controller->SetLocationY(coordY);

  renderWindow->update();
}

void MainWindow::slotLocationCoordZ(int coordZ) {
  controller->SetLocationZ(coordZ);

  renderWindow->update();
}

void MainWindow::slotScaleCoordX(int coordX) {
  controller->SetScaleX(coordX);

  renderWindow->update();
}

void MainWindow::slotRotateCoordX(int coordX) {
  controller->SetRotationX(coordX);

  renderWindow->update();
}
void MainWindow::slotRotateCoordY(int coordY) {
  controller->SetRotationY(coordY);

  renderWindow->update();
}

void MainWindow::slotRotateCoordZ(int coordZ) {
  controller->SetRotationZ(coordZ);

  renderWindow->update();
}

void MainWindow::setupUI() {
  // read saved settings
  userSetting = new UserSetting();

  // Window properties
  setWindowTitle("3DViewer 2.0");
  resize(1280, 720);

  // Central 3D viewport
  renderWindow = new Viewport3D(userSetting, this);
  setCentralWidget(renderWindow);

  // UI components
  createDockWidgets();
  createMenuAndToolbars();

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
}

void MainWindow::createDockWidgets() {
  // Left dock (Tools)
  toolsDock = new QDockWidget("Tools", this);
  toolsDock->setObjectName("toolsDock");
  toolsDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  addDockWidget(Qt::LeftDockWidgetArea, toolsDock);

  fillToolsDockWidget();

  propsDock = new QDockWidget("Properties", this);
  propsDock->setObjectName("propsDock");
  propsDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  propsDock->setWidget(new QWidget);
  addDockWidget(Qt::RightDockWidgetArea, propsDock);

  fillPropsDockWidget();

  // Enable docking features
  setDockOptions(QMainWindow::AllowNestedDocks | QMainWindow::AllowTabbedDocks);
}

void MainWindow::fillPropsDockWidget() {
  // TODO отдельной кнопкой с открытием окна информации?
  QWidget *propBox = new QWidget();
  propBox->setLayout(new QVBoxLayout);

  propsFileInfo = new QLabel();
  propsObjectsInfo = new QLabel();

  QGroupBox *fileNameBox = new QGroupBox("File name:");
  fileNameBox->setLayout(new QHBoxLayout);
  fileNameBox->layout()->addWidget(propsFileInfo);

  QGroupBox *infoBox = new QGroupBox("Objects info:");
  infoBox->setLayout(new QHBoxLayout);
  infoBox->layout()->addWidget(propsObjectsInfo);

  propBox->layout()->addWidget(fileNameBox);
  propBox->layout()->addWidget(infoBox);
  propsDock->setWidget(propBox);
}

void MainWindow::fillToolsDockWidget() {
  locationSlidersBox =
      new SlidersBox("Location", std::pair<int, int>{-100, 100}, 3, this);
  rotateSlidersBox =
      new SlidersBox("Rotate", std::pair<int, int>{-180, 180}, 3, this);
  scaleSlidersBox =
      new SlidersBox("Scale", std::pair<int, int>{1, 200}, 1, this);

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
  toolBox->layout()->addWidget(verticesBox);
  toolBox->layout()->addWidget(edgesBox);
  toolBox->layout()->addWidget(backBox);
  toolBox->layout()->addWidget(saveElemsButton);
  toolBox->layout()->addWidget(restoreElemsButton);
  toolBox->layout()->addWidget(resetElemsButton);

  toolsDock->setWidget(toolBox);
}

void MainWindow::createMenuAndToolbars() {
  // Menu bar
  menuBar = new QMenuBar(this);
  QMenu *fileMenu = menuBar->addMenu("File");
  fileMenu->addAction("Open", this, &MainWindow::openFile);
  fileMenu->addAction("Save image..", this, &MainWindow::saveImage);
  fileMenu->addAction("Custom gif", this, &MainWindow::saveCustomGif);
  fileMenu->addAction("Cycled gif", this, &MainWindow::saveCycledGif);
  fileMenu->addSeparator();
  fileMenu->addAction("Exit", this, &MainWindow::appExit);
  //! QIcon(tr("path/name.smth")) - иконки

  QMenu *settingMenu = menuBar->addMenu("Setting");
  settingMenu->addAction("Restore layout", this, &MainWindow::restoreLayout);
  settingMenu->addSeparator();
  settingMenu->addAction("Save render settings", this,
                         &MainWindow::saveUserSettings);
  settingMenu->addAction("Restore render settings", this,
                         &MainWindow::restoreUserSettings);
  settingMenu->addAction("Reset render settings", this,
                         &MainWindow::resetUserSettings);
  setMenuBar(menuBar);
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
  s21::DrawSceneData *scene = controller->LoadScene(cstr);
  renderWindow->setScene(scene);

  //! check canOpen
  // if (!isSaved)

  propsFileInfo->setText(fileName);
  propsObjectsInfo->setText(QString::fromStdString(scene->info));
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