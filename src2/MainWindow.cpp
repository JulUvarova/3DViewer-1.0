#include "MainWindow.h"

#include <QMenuBar>
#include <QSettings>
#include <QStatusBar>
#include <QToolBar>

#include "Viewport3D.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setupUI();
  restoreLayout();  // Load previous layout
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
  toolsDock->setWidget(new QWidget);  // Add your tool buttons here
  addDockWidget(Qt::LeftDockWidgetArea, toolsDock);

  // Right dock (Properties)
  QDockWidget *propsDock = new QDockWidget("Properties", this);
  propsDock->setWidget(new QWidget);  // Add property editors here
  addDockWidget(Qt::RightDockWidgetArea, propsDock);

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
  fileMenu->addAction("New", this, []() { /* ... */ });
  fileMenu->addAction("Save Layout", this, &MainWindow::saveLayout);
  fileMenu->addAction("Restore Layout", this, &MainWindow::restoreLayout);
  setMenuBar(menuBar);

  // Top toolbar
  QToolBar *toolbar = new QToolBar("Main Toolbar", this);
  toolbar->addAction("Edit Mode");
  toolbar->addSeparator();
  toolbar->addAction("Render");
  addToolBar(Qt::TopToolBarArea, toolbar);
}

void MainWindow::saveLayout() {
  QSettings settings;
  settings.setValue("windowState", saveState());
}

void MainWindow::restoreLayout() {
  QSettings settings;
  restoreState(settings.value("windowState").toByteArray());
}