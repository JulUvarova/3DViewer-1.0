#pragma once

#include <QIntValidator>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QOpenGLFunctions>
#include <QSlider>
#include <QString>
#include <QWidget>

struct Coords {
  int x, y, z;
};

class SlidersBox : public QWidget {
  Q_OBJECT

  int step = 1;
  QLabel *xLabel, *yLabel, *zLabel;
  QSlider *xSlider, *ySlider, *zSlider;
  QLineEdit *xValue, *yValue, *zValue;
  QHBoxLayout *xLayout, *yLayout, *zLayout;
  QVBoxLayout *layout;

 signals:
  void signalChangeX(int coordX);
  void signalChangeY(int coordY);
  void signalChangeZ(int coordZ);

 public:
  SlidersBox(const char *name, int min, int max, QWidget *parent = nullptr)
      : QWidget(parent) {
    xLabel = new QLabel(" X ");
    yLabel = new QLabel(" Y ");
    zLabel = new QLabel(" Z ");

    int midle = (min + max) / 2;
    xSlider = new QSlider(Qt::Horizontal, this);
    xSlider->setRange(min, max);
    xSlider->setTickInterval(step);
    xSlider->setValue(midle);
    //! doesn't work?
    // xSlider->setStyleSheet("QSlider::add-page:horizontal { background: green;
    // }");

    ySlider = new QSlider(Qt::Horizontal, this);
    ySlider->setRange(min, max);
    ySlider->setTickInterval(step);
    ySlider->setValue(midle);

    zSlider = new QSlider(Qt::Horizontal, this);
    zSlider->setRange(min, max);
    zSlider->setTickInterval(step);
    zSlider->setValue(midle);

    xValue = new QLineEdit(this);
    QIntValidator *xValidator = new QIntValidator(min, max, this);
    xValue->setValidator(xValidator);
    xValue->setText(QString::number(xSlider->value()));
    xValue->setFixedWidth(4 * QFontMetrics(xValue->font()).averageCharWidth());
    xValue->setAlignment(Qt::AlignRight);
    xValue->setFrame(false);

    yValue = new QLineEdit(this);
    QIntValidator *yValidator = new QIntValidator(min, max, this);
    yValue->setValidator(yValidator);
    yValue->setText(QString::number(ySlider->value()));
    yValue->setFixedWidth(4 * QFontMetrics(yValue->font()).averageCharWidth());
    yValue->setAlignment(Qt::AlignRight);
    yValue->setFrame(false);

    zValue = new QLineEdit(this);
    QIntValidator *zValidator = new QIntValidator(min, max, this);
    zValue->setValidator(zValidator);
    zValue->setText(QString::number(zSlider->value()));
    zValue->setFixedWidth(4 * QFontMetrics(zValue->font()).averageCharWidth());
    zValue->setAlignment(Qt::AlignRight);
    zValue->setFrame(false);

    xLayout = new QHBoxLayout();
    xLayout->addWidget(xLabel);
    xLayout->addWidget(xSlider);
    xLayout->addWidget(xValue);

    yLayout = new QHBoxLayout();
    yLayout->addWidget(yLabel);
    yLayout->addWidget(ySlider);
    yLayout->addWidget(yValue);

    zLayout = new QHBoxLayout();
    zLayout->addWidget(zLabel);
    zLayout->addWidget(zSlider);
    zLayout->addWidget(zValue);

    layout = new QVBoxLayout();
    layout->addLayout(xLayout);
    layout->addLayout(yLayout);
    layout->addLayout(zLayout);

    QGroupBox *groupBox = new QGroupBox(name);
    groupBox->setLayout(layout);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(groupBox);
    setLayout(mainLayout);

    connect(xSlider, &QSlider::valueChanged, this, &SlidersBox::sliderChangeX);
    connect(ySlider, &QSlider::valueChanged, this, &SlidersBox::sliderChangeY);
    connect(zSlider, &QSlider::valueChanged, this, &SlidersBox::sliderChangeZ);

    connect(xValue, &QLineEdit::textChanged, this, &SlidersBox::labelChangeX);
    connect(yValue, &QLineEdit::textChanged, this, &SlidersBox::labelChangeY);
    connect(zValue, &QLineEdit::textChanged, this, &SlidersBox::labelChangeZ);
  };

 private:
  void labelChangeX() {
    xSlider->setValue(xValue->text().toInt());
    emit signalChangeX(xSlider->value());
  }

  void labelChangeY() {
    ySlider->setValue(yValue->text().toInt());
    emit signalChangeY(ySlider->value());
  }

  void labelChangeZ() {
    zSlider->setValue(zValue->text().toInt());
    emit signalChangeZ(zSlider->value());
  }

  void sliderChangeX() {
    xValue->setText(QString::number(xSlider->value()));
    emit signalChangeX(xSlider->value());
  }
  void sliderChangeY() {
    yValue->setText(QString::number(ySlider->value()));
    emit signalChangeY(ySlider->value());
  }
  void sliderChangeZ() {
    zValue->setText(QString::number(zSlider->value()));
    emit signalChangeZ(zSlider->value());
  }

  Coords sendCoords() {
    return Coords{xSlider->value(), ySlider->value(), zSlider->value()};
  }
};