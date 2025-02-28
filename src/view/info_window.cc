#include "info_window.h"

InfoWindow::InfoWindow(QWidget* parent) : QWidget(parent) {
  setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
  setAttribute(Qt::WA_TranslucentBackground);
  setStyleSheet(
      "background-color: rgba(50, 50, 50, 200); color: white; border-radius: "
      "5px;");

  QVBoxLayout* layout = new QVBoxLayout(this);

  txt_ = new QLabel();
  txt_->setWordWrap(true);
  layout->addWidget(txt_);

  closeButton_ = new QPushButton("×", this);
  closeButton_->setStyleSheet("border: none; font-size: 16px;");
  closeButton_->setFixedSize(20, 20);
  connect(closeButton_, &QPushButton::clicked, this, &QWidget::hide);

  closeButton_->move(width() - closeButton_->width() - 5, 5);
}

void InfoWindow::SetText(const QString& text) {
  txt_->setText(text);
  txt_->adjustSize();
  adjustSize();
  resize(size() * 1.5);
  txt_->setAlignment(Qt::AlignCenter);
}

void InfoWindow::resizeEvent(QResizeEvent* event) {
  closeButton_->move(width() - closeButton_->width(), 0);
  QWidget::resizeEvent(event);
}
