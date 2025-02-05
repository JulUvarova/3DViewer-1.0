#pragma once

#include <QGroupBox>
#include <QLabel>
#include <QLayout>

class SlidersBox : public QGroupBox {
  int min = 0;
  int max = 100;
  float step = 0.01;

  // QLabel *xLabel = new QLabel(" X ");
  // QLabel *yLabel = new QLabel(" Y ");
  // QLabel *xLabel = new QLabel(" X ");
 public:
  SlidersBox(const char *name): SlidersBox() {
    QSlider *xSlider = new QSlider;
    xSlider->setMinimum(min);
    xSlider->setMaximum(max);
    xSlider->setTickInterval(step);
    xSlider->setTickPosition(QSlider::NoTicks);
    xSlider->setOrientation(Qt::Horizontal);

    QSlider *ySlider = new QSlider(xSlider);
    ySlider->setOrientation(Qt::Horizontal);

    QSlider *zSlider = new QSlider(xSlider);
    zSlider->setOrientation(Qt::Horizontal);

    QGroupBox *group = new QGroupBox();
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(xSlider);
    layout->addWidget(ySlider);
    layout->addWidget(zSlider);
    group->setLayout(layout);

    group.setTitle(name);
  };
}