#pragma once

#include <QColor>
#include <QColorDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QOpenGLFunctions>
#include <QPushButton>
#include <QString>
#include <QWidget>

class BackgroundBox : public QWidget {
  Q_OBJECT

  QLabel *colorLabel;
  QVBoxLayout *layout;
  QColor color;
  QPushButton *colorButton;

  void colorChange() {
    QColorDialog *colorDialog = new QColorDialog(this);
    colorDialog->setCurrentColor(color);
    if (colorDialog->exec() == QDialog::Accepted) {
      color = colorDialog->currentColor();
      setColorButton();
      emit signalChangeColor(color);
    }
  }

  void setColorButton() {
    colorButton->setStyleSheet("border: none; background-color: rgb(" +
                               QString::number(color.red()) + ", " +
                               QString::number(color.green()) + ", " +
                               QString::number(color.blue()) + ")");
  }

 signals:
  void signalChangeColor(const QColor &color);

 public:
  BackgroundBox(const char *name, QColor clr, QWidget *parent = nullptr)
      : QWidget(parent) {
    // color
    colorLabel = new QLabel("Color:");
    colorButton = new QPushButton(" ");
    color = clr;
    setColorButton();

    // boxing
    QHBoxLayout *colorLayout = new QHBoxLayout();
    colorLayout->addWidget(colorLabel);
    colorLayout->addWidget(colorButton);

    QVBoxLayout *groupBoxLayout = new QVBoxLayout();
    groupBoxLayout->addLayout(colorLayout);

    QGroupBox *groupBox = new QGroupBox(name);
    groupBox->setLayout(groupBoxLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(groupBox);
    setLayout(mainLayout);

    connect(colorButton, &QPushButton::clicked, this,
            &BackgroundBox::colorChange);
  };
};