#pragma once

#include <QColor>
#include <QColorDialog>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QOpenGLFunctions>
#include <QPushButton>
#include <QSpinBox>
#include <QString>
#include <QWidget>

class ElemBox : public QWidget {
  Q_OBJECT

  QLabel *typeLabel, *sizeLabel, *colorLabel;
  QComboBox *type;
  QVBoxLayout *layout;
  QSpinBox *size;
  QColor color;
  QPushButton *colorButton;

  inline void typeChange(const QString &text) { emit signalChangeType(text); }

  inline void sizeChange(const int value) { emit signalChangeSize(value); }

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
  void signalChangeType(const QString &text);
  void signalChangeSize(const int size);
  void signalChangeColor(const QColor &color);

 public:
  ElemBox(const char *name, QStringList &lst, QWidget *parent = nullptr)
      : QWidget(parent) {
    // type
    typeLabel = new QLabel("Type:");
    type = new QComboBox(this);
    type->addItems(lst);
    type->setEditable(true);
    QHBoxLayout *typeLayout = new QHBoxLayout();
    typeLayout->addWidget(typeLabel);
    typeLayout->addWidget(type);

    // size
    sizeLabel = new QLabel("Size:");
    size = new QSpinBox(this);
    size->setRange(0, 100);
    size->setValue(10);
    QHBoxLayout *sizeLayout = new QHBoxLayout();
    sizeLayout->addWidget(sizeLabel);
    sizeLayout->addWidget(size);

    // color
    colorLabel = new QLabel("Color:");
    colorButton = new QPushButton(" ");
    color = QColor(Qt::white);
    setColorButton();

    // boxing
    QHBoxLayout *colorLayout = new QHBoxLayout();
    colorLayout->addWidget(colorLabel);
    colorLayout->addWidget(colorButton);

    QVBoxLayout *groupBoxLayout = new QVBoxLayout();
    groupBoxLayout->addLayout(typeLayout);
    groupBoxLayout->addLayout(sizeLayout);
    groupBoxLayout->addLayout(colorLayout);

    QGroupBox *groupBox = new QGroupBox(name);
    groupBox->setLayout(groupBoxLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(groupBox);
    setLayout(mainLayout);

    connect(type, &QComboBox::currentTextChanged, this, &ElemBox::typeChange);
    connect(size, &QSpinBox::valueChanged, this, &ElemBox::sizeChange);
    connect(colorButton, &QPushButton::clicked, this, &ElemBox::colorChange);
  };
};