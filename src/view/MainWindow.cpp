#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setupUI();
  restoreLayout();  // Load previous layout

  //! чтение настроек QSettings("", "");

  connect(moveSlidersBox, &SlidersBox::signalChangeCoords, this,
          &MainWindow::slotMoveCoords);
  connect(scaleSlidersBox, &SlidersBox::signalChangeCoords, this,
          &MainWindow::slotScaleCoords);
  connect(rotateSlidersBox, &SlidersBox::signalChangeCoords, this,
          &MainWindow::slotRotateCoords);
  connect(verticesBox, &ElemBox::signalChangeType, this,
          &MainWindow::slotVerticesType);
  connect(verticesBox, &ElemBox::signalChangeSize, this,
          &MainWindow::slotVerticesSize);
  connect(verticesBox, &ElemBox::signalChangeColor, this,
          &MainWindow::slotVerticesColor);
  connect(edgesBox, &ElemBox::signalChangeType, this,
          &MainWindow::slotEdgesType);
  connect(edgesBox, &ElemBox::signalChangeSize, this,
          &MainWindow::slotEdgesSize);
  connect(edgesBox, &ElemBox::signalChangeColor, this,
          &MainWindow::slotEdgesColor);
}

void MainWindow::slotVerticesColor(const QColor &color) {
  qDebug() << "VerticesColor: " << color.red() << color.green() << color.blue();
}

void MainWindow::slotVerticesType(const QString &text) {
  qDebug() << "VerticesType: " << text;
}

void MainWindow::slotVerticesSize(const int value) {
  qDebug() << "VerticesSize: " << value;
}

void MainWindow::slotEdgesColor(const QColor &color) {
  qDebug() << "EdgesColor: " << color.red() << color.green() << color.blue();
}

void MainWindow::slotEdgesType(const QString &text) {
  qDebug() << "EdgesType: " << text;
}

void MainWindow::slotEdgesSize(const int value) {
  qDebug() << "EdgesSize: " << value;
}

void MainWindow::slotMoveCoords(Coords coords) {
  qDebug() << "Move: " << coords.x << " " << coords.y << " " << coords.z;
}

void MainWindow::slotScaleCoords(Coords coords) {
  qDebug() << "Scale: " << coords.x << " " << coords.y << " " << coords.z;
}

void MainWindow::slotRotateCoords(Coords coords) {
  qDebug() << "Rotate: " << coords.x << " " << coords.y << " " << coords.z;
}

void MainWindow::setupUI() {
  // Window properties
  setWindowTitle("Blender-like UI");
  resize(1280, 720);

  // Central 3D viewport
  setCentralWidget(new Viewport3D(this));

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

  // Create sliders (Move, Rotate, Scale)
  moveSlidersBox = new SlidersBox("Move", this);
  rotateSlidersBox = new SlidersBox("Rotate", this);
  scaleSlidersBox = new SlidersBox("Scale", this);

  // Create verticesBox
  QStringList verticesLst;
  verticesLst << "circle" << "square" << "none";
  verticesBox = new ElemBox("Vertices", verticesLst, this);

  // Create EdgeBox
  QStringList edgesLst;
  edgesLst << "solid" << "dashed" << "none";
  edgesBox = new ElemBox("Edges", edgesLst, this);

  // Fill left dock
  QWidget *box = new QWidget();
  box->setLayout(new QVBoxLayout);
  box->layout()->addWidget(moveSlidersBox);
  box->layout()->addWidget(rotateSlidersBox);
  box->layout()->addWidget(scaleSlidersBox);
  box->layout()->addWidget(verticesBox);
  box->layout()->addWidget(edgesBox);

  //! Раскрывающееся меню (CollapseButton)
  // CollapseButton *collapseButton = new CollapseButton();
  // collapseButton->setContent(scaleSlidersBox);
  // box->layout()->addWidget(scaleSlidersBox);

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
  fileMenu->addAction("Exit", this, &MainWindow::close);
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

void MainWindow::close() {
  //! сохраняем настройки отображения QSettings
  // close();
}

void MainWindow::openFile() {
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), "./",
                                                  tr("Images (*.obj)"));
  qDebug() << fileName;

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
    //! сохраняем как bmp

  } else if (selectedFilter == "*.jpeg") {
    qDebug() << "jpeg";
    //! сохраняем как jpeg
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