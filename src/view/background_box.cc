#include "background_box.h"

BackgroundBox::BackgroundBox(const char *name, QColor color, QWidget *parent)
    : QWidget(parent) {
  // color
  colorLabel_ = new QLabel("Color:", this);
  colorButton_ = new QPushButton(" ", this);
  SetColorButton(color);

  // boxing
  QHBoxLayout *colorLayout = new QHBoxLayout();
  colorLayout->addWidget(colorLabel_);
  colorLayout->addWidget(colorButton_);

  QVBoxLayout *groupBoxLayout = new QVBoxLayout();
  groupBoxLayout->addLayout(colorLayout);

  QGroupBox *groupBox = new QGroupBox(name, this);
  groupBox->setLayout(groupBoxLayout);

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(groupBox);
  setLayout(mainLayout);

  connect(colorButton_, &QPushButton::clicked, this,
          &BackgroundBox::ColorChange);
};

void BackgroundBox::ColorChange() {
  QColorDialog *colorDialog = new QColorDialog(this);
  if (colorDialog->exec() == QDialog::Accepted) {
    QColor newColor = colorDialog->currentColor();
    SetColorButton(newColor);
    Q_EMIT signalChangeColor(newColor);
  }
}

void BackgroundBox::SetColorButton(const QColor &color) {
  colorButton_->setStyleSheet(
      "border: none; background-color: " + color.name() + ";");
}