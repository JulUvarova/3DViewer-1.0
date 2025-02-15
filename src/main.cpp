#include <QApplication>

#include "view/MainWindow.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  MainWindow window(new s21::Controller());
  window.show();
  return app.exec();
}
