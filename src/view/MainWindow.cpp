#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setupUI();
  restoreLayout();  // Load previous layout

  //! чтение настроек QSettings("", "");

  connect(xSlider, &QSlider::sliderMoved, this, &MainWindow::xChange);
  connect(ySlider, &QSlider::sliderMoved, this, &MainWindow::yChange);
  connect(zSlider, &QSlider::sliderMoved, this, &MainWindow::zChange);
}

void MainWindow::xChange() { qDebug() << "X: " << xSlider->value(); }

void MainWindow::yChange() { qDebug() << "Y: " << ySlider->value(); }

void MainWindow::zChange() { qDebug() << "Z: " << zSlider->value(); }

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
  toolsDock->setWidget(new QWidget);  // Add your tool buttons here
  addDockWidget(Qt::LeftDockWidgetArea, toolsDock);

  // Box with test sliders
  //! В прототип с обработкой сигналов изменения
  int min = 0;
  int max = 100;
  float step = 0.01;

  QLabel *xLabel = new QLabel(" X ");
  QLabel *yLabel = new QLabel(" Y ");
  QLabel *zLabel = new QLabel(" Z ");

  xSlider = new QSlider;
  xSlider->setRange(min, max);
  xSlider->setTickInterval(step);
  xSlider->setValue(max / 2);
  xSlider->setOrientation(Qt::Horizontal);

  ySlider = new QSlider(xSlider);
  ySlider->setValue(max / 2);
  ySlider->setOrientation(Qt::Horizontal);

  zSlider = new QSlider(xSlider);
  zSlider->setValue(max / 2);
  zSlider->setOrientation(Qt::Horizontal);

  QGroupBox *xInfo = new QGroupBox();
  QHBoxLayout *xLayout = new QHBoxLayout();
  xLayout->addWidget(xLabel);
  xLayout->addWidget(xSlider);
  xInfo->setLayout(xLayout);

  QGroupBox *yInfo = new QGroupBox();
  QHBoxLayout *yLayout = new QHBoxLayout();
  yLayout->addWidget(yLabel);
  yLayout->addWidget(ySlider);
  yInfo->setLayout(yLayout);

  QGroupBox *zInfo = new QGroupBox();
  QHBoxLayout *zLayout = new QHBoxLayout();
  zLayout->addWidget(zLabel);
  zLayout->addWidget(zSlider);
  zInfo->setLayout(zLayout);

  QGroupBox *group = new QGroupBox();
  QVBoxLayout *layout = new QVBoxLayout();

  layout->addWidget(xInfo);
  layout->addWidget(yInfo);
  layout->addWidget(zInfo);
  group->setLayout(layout);

  group->setTitle("Location");

  toolsDock->setWidget(group);

  // Right dock (Properties)
  QDockWidget *propsDock = new QDockWidget("Properties", this);
  propsDock->setWidget(new QWidget);  // Add property editors here
  addDockWidget(Qt::RightDockWidgetArea, propsDock);

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

  // Top toolbar
  QToolBar *toolbar = new QToolBar("Main Toolbar", this);
  toolbar->addAction("Edit Mode");
  toolbar->addSeparator();
  toolbar->addAction("Render");
  toolbar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
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
  obj_data.parse(cstr);
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