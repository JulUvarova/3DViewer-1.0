#pragma once
#include <QDockWidget>
#include <QMainWindow>

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);

 private slots:
  void saveLayout();
  void restoreLayout();

 private:
  void setupUI();
  void createDockWidgets();
  void createMenuAndToolbars();
};