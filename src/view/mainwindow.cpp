#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  setFixedSize(700, 500);
  facade = new s21::Facade();

  // s21::NormalizationParam param;
  // s21::FacadeOperationResult res =
  //     facade->LoadScene("", param);
  // if (res.IsSuccess()) {
  //   myOpenGLWidget->setScene(facade->GetScene());
  // }

  ui->openGLWidget = new MyOpenGLWidget();
  ui->openGLWidget->makeCurrent();
  // ui->openGLWidget->update();
  ui->openGLWidget->repaint();
  // ui->openGLWidget->setFocus();

  ui->openGLWidget->show();
}

MainWindow::~MainWindow() { delete ui; }