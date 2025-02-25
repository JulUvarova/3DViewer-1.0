#pragma once

#include <QPushButton>

class ProjectionButton : public QPushButton {
  Q_OBJECT

 public:
  ProjectionButton(QWidget *parent = nullptr) : QPushButton(parent) {
    setStyleSheet(
        "QPushButton { border-radius: 25px; background-color: grey; } "
        "QPushButton:hover { background-color: lightgrey; }");
    setIconSize(QSize(size - 10, size - 10));
    setLocation(parent->width());
    setProjection();

    connect(this, &QPushButton::clicked, this,
            &ProjectionButton::setProjection);
  }

  void setLocation(int w) { setGeometry(w - size - 10, 10, size, size); }

  bool isParallelProjection() { return isParallel; }
  
 Q_SIGNALS:
  void signalChangeProjection();

 private:
  bool isParallel = true;
  const int size = 50;

  void setProjection() {
    isParallel = !isParallel;
    if (isParallel) {
      setIcon(QIcon("./view/recourses/parallelproj1.png"));
    } else {
      setIcon(QIcon("./view/recourses/centralproj1.png"));
    }
    Q_EMIT signalChangeProjection();
  }
};