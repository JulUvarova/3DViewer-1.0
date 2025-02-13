#include "MainWindow.h"

MainWindow::MainWindow(s21::Controller *controller, QWidget *parent)
    : QMainWindow(parent) {
  controller = controller;

  setupUI();

  restoreLayout();  // Load previous layout

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

void MainWindow::setAllParameters() {
  // set users parameters in controller
  controller->SetBackgroundColor(renderSetting->getBackgroundColor());

  controller->SetProjectionType(renderSetting->getProjection());

  controller->SetVertexColor(renderSetting->getVerticesColor());
  controller->SetVertexType(renderSetting->getVerticesType());
  controller->SetVertexSize(renderSetting->getVerticesSize());

  controller->SetEdgeColor(renderSetting->getEdgesColor());
  controller->SetEdgeType(renderSetting->getEdgesType());
  controller->SetEdgeSize(renderSetting->getEdgesSize());
}

void MainWindow::slotProjectionType(const bool isParallel) {
  controller->SetProjectionType(isParallel);
  renderSetting->setProjection(isParallel);

  centralWidget->update();
}

void MainWindow::slotBackgroundColor(const QColor &color) {
  controller->SetBackgroundColor(color);
  renderSetting->setBackgroundColor(color);

  centralWidget->update();
}

void MainWindow::slotVerticesColor(const QColor &color) {
  controller->SetVertexColor(color);
  renderSetting->setVerticesColor(color);

  centralWidget->update();
}

void MainWindow::slotVerticesType(const QString &text) {
  controller->SetVertexType(text);
  renderSetting->setVerticesType(text);

  centralWidget->update();
}

void MainWindow::slotVerticesSize(const int value) {
  controller->SetVertexSize(value);
  renderSetting->setVerticesSize(value);

  centralWidget->update();
}

void MainWindow::slotEdgesColor(const QColor &color) {
  controller->SetEdgeColor(color);
  renderSetting->setEdgesColor(color);

  centralWidget->update();
}

void MainWindow::slotEdgesType(const QString &text) {
  controller->SetEdgeType(text);
  renderSetting->setEdgesType(text);

  centralWidget->update();
}

void MainWindow::slotEdgesSize(const int value) {
  controller->SetEdgeSize(value);
  renderSetting->setEdgesSize(value);

  centralWidget->update();
}

void MainWindow::slotLocationCoordX(int coordX) {
  controller->SetLocationX(coordX);

  centralWidget->update();
}
void MainWindow::slotLocationCoordY(int coordY) {
  controller->SetLocationY(coordY);

  centralWidget->update();
}

void MainWindow::slotLocationCoordZ(int coordZ) {
  controller->SetLocationZ(coordZ);

  centralWidget->update();
}

void MainWindow::slotScaleCoordX(int coordX) {
  controller->SetScaleX(coordX);

  centralWidget->update();
}
void MainWindow::slotScaleCoordY(int coordY) {
  controller->SetScaleY(coordY);

  centralWidget->update();
}

void MainWindow::slotScaleCoordZ(int coordZ) {
  controller->SetScaleZ(coordZ);

  centralWidget->update();
}

void MainWindow::slotRotateCoordX(int coordX) {
  controller->SetRotationX(coordX);

  centralWidget->update();
}
void MainWindow::slotRotateCoordY(int coordY) {
  controller->SetRotationY(coordY);

  centralWidget->update();
}

void MainWindow::slotRotateCoordZ(int coordZ) {
  controller->SetRotationZ(coordZ);

  centralWidget->update();
}

void MainWindow::slotViewportSize(const int w, const int h) {
  controller->SetViewportSize(w, h);
}

void MainWindow::setupUI() {
  // read saved settings
  renderSetting = new RenderSetting();

  // Window properties
  setWindowTitle("Blender-like UI");
  resize(1280, 720);

  // Central 3D viewport
  centralWidget = new Viewport3D(renderSetting, this);
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
  addDockWidget(Qt::LeftDockWidgetArea, toolsDock);

  // Create sliders (Location, Rotate, Scale)
  locationSlidersBox = new SlidersBox("Location", -100, 100, this);
  rotateSlidersBox = new SlidersBox("Rotate", -180, 180, this);
  scaleSlidersBox = new SlidersBox("Scale", 1, 200, this);

  // Create verticesBox
  QStringList verticesLst;
  verticesLst << "circle" << "square" << "triangle" << "none";
  Setting verticesSetting{renderSetting->getVerticesType(),
                          renderSetting->getVerticesColor(),
                          renderSetting->getVerticesSize()};
  verticesBox = new ElemBox("Vertices", verticesLst, verticesSetting, this);

  // Create EdgeBox
  QStringList edgesLst;
  edgesLst << "line" << "dashed" << "none";
  Setting edgesSetting{renderSetting->getEdgesType(),
                       renderSetting->getEdgesColor(),
                       renderSetting->getEdgesSize()};
  edgesBox = new ElemBox("Edges", edgesLst, edgesSetting, this);

  // create backgroundBox
  // QColor backColor = renderSetting->getBackgroundColor();
  backBox = new BackgroundBox("Background", renderSetting->getBackgroundColor(),
                              this);

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
  propsDock->setWidget(new QWidget);  // Add property editors here
  addDockWidget(Qt::RightDockWidgetArea, propsDock);

  // Scene info on Properties
  propsInfo = new QLabel();
  propsDock->setWidget(propsInfo);

  // Bottom dock (Timeline)
  QDockWidget *timelineDock = new QDockWidget("Timeline", this);
  timelineDock->setWidget(new QWidget);  // Add timeline/console widgets
  addDockWidget(Qt::BottomDockWidgetArea, timelineDock);

  // Enable docking features
  setDockOptions(QMainWindow::AllowNestedDocks | QMainWindow::AllowTabbedDocks);
}

void MainWindow::createMenuAndToolbars() {
  // Menu bar
  QMenuBar *menuBar = new QMenuBar(this);
  QMenu *fileMenu = menuBar->addMenu("File");
  fileMenu->addAction("New", this, &MainWindow::openFile);
  fileMenu->addAction("Save image..", this, &MainWindow::saveImage);
  fileMenu->addSeparator();
  fileMenu->addAction("Exit", this, &MainWindow::appExit);
  // QIcon(tr("path/name.smth")) - иконки

  QMenu *settingMenu = menuBar->addMenu("Setting");
  settingMenu->addAction("Save Layout", this, &MainWindow::saveLayout);
  settingMenu->addAction("Restore Layout", this, &MainWindow::restoreLayout);
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
  renderSetting->saveRenderSetting();
  close();
}

void MainWindow::openFile() {
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), "./",
                                                  tr("Images (*.obj)"));
  qDebug() << fileName;

  //! if cancel
  if (fileName.isEmpty()) return;

  // upload all user render param to controller
  setAllParameters();

  QByteArray byteArray = fileName.toUtf8();
  const char *cstr = byteArray.constData();

  // parser test
  obj_data.Parse(cstr);
  propsInfo->setText(QString::fromStdString(obj_data.toString()));
}

void MainWindow::saveImage() {
  // file name & save location
  QString selectedFilter;
  QString fileName = QFileDialog::getSaveFileName(
      this, tr("Choose folder"), "./", tr("*.jpeg;;*.bmp"), &selectedFilter);

  qDebug() << fileName;

  if (selectedFilter == "*.bmp") {
    qDebug() << "bmp";
    centralWidget->grab().save(fileName + ".bmp");

  } else if (selectedFilter == "*.jpeg") {
    qDebug() << "jpeg";
    centralWidget->grab().save(fileName + ".jpeg");
  }

  // QFile file(fileName);
  // if (!file.open(QIODevice::WriteOnly)) {
  //   QMessageBox::information(this, tr("Unable to open file"),
  //                            file.errorString());
  //   return;
  // }
}

void MainWindow::saveLayout() {
  QSettings settings;
  settings.setValue("windowState", saveState());
}

void MainWindow::restoreLayout() {
  QSettings settings;
  restoreState(settings.value("windowState").toByteArray());
}

void MainWindow::closeEvent(QCloseEvent *event) {
  renderSetting->saveRenderSetting();
  QMainWindow::closeEvent(event);
}