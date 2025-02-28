#include <QApplication>

#include "view/main_window.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  MainWindow window(s21::Controller::GetInstance());
  window.show();
  return app.exec();
}
