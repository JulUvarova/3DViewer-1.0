#include "MainWindow.h"

MainWindow::MainWindow(s21::Controller *ctrl, QWidget *parent)
    : QMainWindow(parent), controller(ctrl) {
  setupUI();

  saveLayout();

  // Location coordinates
  connect(locationSlidersBox, &SlidersBox::signalChangeX, this,
          &MainWindow::slotLocationCoordX);
  connect(locationSlidersBox, &SlidersBox::signalChangeY, this,
          &MainWindow::slotLocationCoordY);
  connect(locationSlidersBox, &SlidersBox::signalChangeZ, this,
          &MainWindow::slotLocationCoordZ);

  // scale coordinates
  connect(scaleSlidersBox, &SlidersBox::signalChangeX, this,
          &MainWindow::slotScaleCoordX);
  connect(scaleSlidersBox, &SlidersBox::signalChangeY, this,
          &MainWindow::slotScaleCoordY);
  connect(scaleSlidersBox, &SlidersBox::signalChangeZ, this,
          &MainWindow::slotScaleCoordZ);

  // rotate coordinates
  connect(rotateSlidersBox, &SlidersBox::signalChangeX, this,
          &MainWindow::slotRotateCoordX);
  connect(rotateSlidersBox, &SlidersBox::signalChangeY, this,
          &MainWindow::slotRotateCoordY);
  connect(rotateSlidersBox, &SlidersBox::signalChangeZ, this,
          &MainWindow::slotRotateCoordZ);

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

  // background prop
  connect(centralWidget, &Viewport3D::signalChangeSize, this,
          &MainWindow::slotViewportSize);

  // projection
  connect(centralWidget, &Viewport3D::signalChangeProjection, this,
          &MainWindow::slotProjectionType);
}

void MainWindow::setSceneParameters() {
  // set users parameters in controller
  controller->SetBackgroundColor(userSetting->getBackgroundColor());

  controller->SetProjectionType(userSetting->IsParallelProjectrion());

  controller->SetVertexColor(userSetting->getVerticesColor());
  controller->SetVertexType(userSetting->getVerticesType());
  controller->SetVertexSize(userSetting->getVerticesSize());

  controller->SetEdgeColor(userSetting->getEdgesColor());
  controller->SetEdgeType(userSetting->getEdgesType());
  controller->SetEdgeSize(userSetting->getEdgesSize());
}

void MainWindow::slotProjectionType(const bool isParallel) {
  controller->SetProjectionType(isParallel);
  userSetting->setProjection(isParallel);

  centralWidget->update();
}

void MainWindow::slotBackgroundColor(const QColor &color) {
  controller->SetBackgroundColor(color);
  userSetting->setBackgroundColor(color);

  centralWidget->update();
}

void MainWindow::slotVerticesColor(const QColor &color) {
  controller->SetVertexColor(color);
  userSetting->setVerticesColor(color);

  centralWidget->update();
}

void MainWindow::slotVerticesType(const QString &text) {
  controller->SetVertexType(text);
  userSetting->setVerticesType(text);

  centralWidget->update();
}

void MainWindow::slotVerticesSize(const int value) {
  controller->SetVertexSize(value);
  userSetting->setVerticesSize(value);

  centralWidget->update();
}

void MainWindow::slotEdgesColor(const QColor &color) {
  controller->SetEdgeColor(color);
  userSetting->setEdgesColor(color);

  centralWidget->update();
}

void MainWindow::slotEdgesType(const QString &text) {
  controller->SetEdgeType(text);
  userSetting->setEdgesType(text);

  centralWidget->update();
}

void MainWindow::slotEdgesSize(const int value) {
  controller->SetEdgeSize(value);
  userSetting->setEdgesSize(value);

  centralWidget->update();
}

void MainWindow::slotLocationCoordX(int coordX) {
  s21::DrawSceneData scene = controller->SetLocationX(coordX);
  drawScene(scene);
}
void MainWindow::slotLocationCoordY(int coordY) {
  s21::DrawSceneData scene = controller->SetLocationY(coordY);
  drawScene(scene);
}

void MainWindow::slotLocationCoordZ(int coordZ) {
  s21::DrawSceneData scene = controller->SetLocationZ(coordZ);
  drawScene(scene);
}

void MainWindow::slotScaleCoordX(int coordX) {
  s21::DrawSceneData scene = controller->SetScaleX(coordX);
  drawScene(scene);
}
void MainWindow::slotScaleCoordY(int coordY) {
  s21::DrawSceneData scene = controller->SetScaleY(coordY);
  drawScene(scene);
}

void MainWindow::slotScaleCoordZ(int coordZ) {
  s21::DrawSceneData scene = controller->SetScaleZ(coordZ);
  drawScene(scene);
}

void MainWindow::slotRotateCoordX(int coordX) {
  s21::DrawSceneData scene = controller->SetRotationX(coordX);
  drawScene(scene);
}
void MainWindow::slotRotateCoordY(int coordY) {
  s21::DrawSceneData scene = controller->SetRotationY(coordY);
  drawScene(scene);
}

void MainWindow::slotRotateCoordZ(int coordZ) {
  s21::DrawSceneData scene = controller->SetRotationZ(coordZ);
  drawScene(scene);
}

void MainWindow::slotViewportSize(const int w, const int h) {
  controller->SetViewportSize(w, h);

  centralWidget->update();
}

void MainWindow::setupUI() {
  // read saved settings
  userSetting = new UserSetting();

  // Window properties
  setWindowTitle("3DViewer 2.0");
  resize(1280, 720);

  // Central 3D viewport
  centralWidget = new Viewport3D(userSetting, this);
  setCentralWidget(centralWidget);

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
  QDockWidget *toolsDock = new QDockWidget("Tools", this);
  toolsDock->setObjectName("toolsDock");
  toolsDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  addDockWidget(Qt::LeftDockWidgetArea, toolsDock);

  // Create sliders (Location, Rotate, Scale)
  locationSlidersBox = new SlidersBox("Location", -100, 100, this);
  rotateSlidersBox = new SlidersBox("Rotate", -180, 180, this);
  scaleSlidersBox = new SlidersBox("Scale", 1, 200, this);

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

  // Fill left dock
  QWidget *box = new QWidget();
  box->setLayout(new QVBoxLayout);
  box->layout()->addWidget(locationSlidersBox);
  box->layout()->addWidget(rotateSlidersBox);
  box->layout()->addWidget(scaleSlidersBox);
  box->layout()->addWidget(verticesBox);
  box->layout()->addWidget(edgesBox);
  box->layout()->addWidget(backBox);

  toolsDock->setWidget(box);

  // Right dock (Properties)
  QDockWidget *propsDock = new QDockWidget("Properties", this);
  propsDock->setObjectName("propsDock");
  propsDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  propsDock->setWidget(new QWidget);
  addDockWidget(Qt::RightDockWidgetArea, propsDock);

  // Scene info on Properties
  propsInfo = new QLabel();
  propsDock->setWidget(propsInfo);

  // Bottom dock (Timeline)
  QDockWidget *timelineDock = new QDockWidget("Timeline", this);
  timelineDock->setObjectName("timelineDock");
  timelineDock->setWidget(new QWidget);
  timelineDock->setAllowedAreas(Qt::BottomDockWidgetArea);
  timelineDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
  addDockWidget(Qt::BottomDockWidgetArea, timelineDock);

  // Enable docking features
  setDockOptions(QMainWindow::AllowNestedDocks | QMainWindow::AllowTabbedDocks);
}

void MainWindow::createMenuAndToolbars() {
  // Menu bar
  QMenuBar *menuBar = new QMenuBar(this);
  QMenu *fileMenu = menuBar->addMenu("File");
  fileMenu->addAction("Open", this, &MainWindow::openFile);
  fileMenu->addAction("Save image..", this, &MainWindow::saveImage);
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

  // // Top toolbar
  // QToolBar *toolbar = new QToolBar("Main Toolbar", this);
  // toolbar->addAction("Edit Mode");
  // toolbar->addSeparator();
  // toolbar->addAction("Render");
  // toolbar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
}

void MainWindow::appExit() {
  // сохраняем настройки отображения QSettings
  userSetting->saveRenderSettings();
  close();
}

void MainWindow::openFile() {
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), "./",
                                                  tr("Images (*.obj)"));
  //! if cancel
  if (fileName.isEmpty()) {
    return;
  }

  // upload all user render param to controller
  setSceneParameters();

  QByteArray byteArray = fileName.toUtf8();
  const char *cstr = byteArray.constData();
  // QMessageBox::information(this, tr("Unable to open file"),
  //                          "File is not opened =(");

  s21::DrawSceneData scene = controller->LoadScene(cstr);
  drawScene(scene);
  //! check canOpen
  // if (!isSaved)

  propsInfo->setText(QString::fromStdString(scene.info));
}

void MainWindow::drawScene(s21::DrawSceneData scene) {
  centralWidget->setScene(scene.vertices, scene.vertex_indices);
  centralWidget->update();
}

void MainWindow::saveImage() {
  // file name & save location
  QString selectedFilter;
  QString fileName = QFileDialog::getSaveFileName(
      this, tr("Choose folder"), "./", tr("*.jpeg;;*.bmp"), &selectedFilter);

  QString postfix;
  if (selectedFilter == "*.bmp")
    postfix = ".bmp";
  else if (selectedFilter == "*.jpeg")
    postfix = ".jpeg";

  centralWidget->beforeGrab();
  bool isSaved = centralWidget->grab().save(fileName + postfix);
  if (!isSaved)
    QMessageBox::information(this, tr("Unable to save file"),
                             "File is not saved =(");
  else
    //! отладка
    QMessageBox::information(this, tr("File saved"), "File is saved! =)");
  centralWidget->afterGrab();
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
  setSceneParameters();
  setVisualParameters();
}

void MainWindow::restoreUserSettings() {
  userSetting->readRenderSettings();
  setSceneParameters();
  setVisualParameters();
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

  //! projection
}