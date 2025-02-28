#include "control_window.h"

ControlWindow::ControlWindow(QWidget* parent) : QWidget(parent) {
  QGridLayout* mainLayout = new QGridLayout(this);
  mainLayout->setContentsMargins(0, 0, 0, 0);

  QWidget* buttonContainer = new QWidget(this);
  buttonContainer->setAttribute(Qt::WA_TranslucentBackground);

  QVBoxLayout* buttonsLayout = new QVBoxLayout(buttonContainer);
  buttonsLayout->setSpacing(10);

  openButton_ = new QPushButton(this);
  jpegButton_ = new QPushButton(this);
  bmpButton_ = new QPushButton(this);
  customGifButton_ = new QPushButton(this);
  cycledGifButton_ = new QPushButton(this);

  QString buttonStyle =
      "QPushButton {"
      "  background-color: #404040;"
      "  border-radius: 25px;"  // круглое
      "  min-width: 50px;"
      "  max-width: 50px;"
      "  min-height: 50px;"
      "  max-height: 50px;"
      "}"
      "QPushButton:hover {"
      "  background-color: grey;"
      "}"
      "QPushButton:pressed {"
      "  background-color: lightgrey;"
      "}";
  openButton_->setStyleSheet(buttonStyle);
  openButton_->setIcon(QIcon("view/resources/open_64_negate.png"));
  openButton_->setIconSize(QSize(30, 30));
  jpegButton_->setStyleSheet(buttonStyle);
  jpegButton_->setIcon(QIcon("view/resources/jpg_64_negate.png"));
  jpegButton_->setIconSize(QSize(30, 30));
  bmpButton_->setStyleSheet(buttonStyle);
  bmpButton_->setIcon(QIcon("view/resources/bmp_64_negate.png"));
  bmpButton_->setIconSize(QSize(30, 30));
  customGifButton_->setStyleSheet(buttonStyle);
  customGifButton_->setIcon(QIcon("view/resources/gif_64_negate.png"));
  customGifButton_->setIconSize(QSize(30, 30));
  cycledGifButton_->setStyleSheet(buttonStyle);
  cycledGifButton_->setIcon(QIcon("view/resources/cgif_64_negate.png"));
  cycledGifButton_->setIconSize(QSize(30, 30));

  buttonsLayout->addWidget(openButton_);
  buttonsLayout->addWidget(jpegButton_);
  buttonsLayout->addWidget(bmpButton_);
  buttonsLayout->addWidget(customGifButton_);
  buttonsLayout->addWidget(cycledGifButton_);

  // поверх окна рендера
  mainLayout->addWidget(buttonContainer, 0, 0, Qt::AlignRight | Qt::AlignTop);

  // отступы от краев
  buttonsLayout->setContentsMargins(0, 10, 10, 0);

  // shadow for buttons
  buttonContainer->setStyleSheet(
      "QWidget {"
      "  background-color: rgba(100,100,100,100);"
      "  border-radius: 6px;"
      "}");
  buttonContainer->setGraphicsEffect(new QGraphicsDropShadowEffect(this));

  connect(openButton_, &QPushButton::clicked, this, &ControlWindow::OpenFile);
  connect(bmpButton_, &QPushButton::clicked, this,
          [this]() { SaveFile(".bmp", 0); });
  connect(jpegButton_, &QPushButton::clicked, this,
          [this]() { SaveFile(".jpeg", 0); });
  connect(customGifButton_, &QPushButton::clicked, this,
          [this]() { SaveFile(".gif", 1); });
  connect(cycledGifButton_, &QPushButton::clicked, this,
          [this]() { SaveFile(".gif", 2); });
}

void ControlWindow::OpenFile() {
  QString filename = QFileDialog::getOpenFileName(this, tr("Open file"), "./",
                                                  tr("Images (*.obj)"));
  if (!filename.isEmpty()) Q_EMIT signalOpenFile(filename);
}

void ControlWindow::SaveFile(const char* options, int type) {
  QString selectedFilter;
  QString filename = QFileDialog::getSaveFileName(
      this, tr("Choose folder"), "./", tr(options), &selectedFilter);
  if (!filename.isEmpty()) {
    filename.append(selectedFilter.remove("*"));
    if (filename.endsWith("gif") && type == 1) {  // TODO enum
      Q_EMIT signalStartCustomGif(filename);
    } else if (filename.endsWith("gif") && type == 2) {
      Q_EMIT signalStartCycledGif(filename);
    } else if (filename.endsWith("bmp") || filename.endsWith("jpeg")) {
      Q_EMIT signalSaveFile(filename);
    }
  }
}

void ControlWindow::mousePressEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton)
    mouseAction_ = MouseAction::kLeftButton;
  else if (event->button() == Qt::MiddleButton)
    mouseAction_ = MouseAction::kMiddleButton;

  if (mouseAction_ != MouseAction::kNone) {
    mousePos_ = event->pos();
  }
}

void ControlWindow::mouseMoveEvent(QMouseEvent* event) {
  if (mouseAction_ != MouseAction::kNone) {
    QPoint pos = event->pos();

    // left will move object
    if (mouseAction_ == MouseAction::kLeftButton) {
      int shiftX = (pos.x() * 200 / width() - 100) -
                   (mousePos_.x() * 200 / width() - 100);
      int shiftY = -((pos.y() * 200 / height() - 100) -
                     (mousePos_.y() * 200 / height() - 100));

      Q_EMIT signalChangeMoveCoords(
          std::pair<int, int>{2 * shiftX, 2 * shiftY});
    }
    // middle will rotate object
    if (mouseAction_ == MouseAction::kMiddleButton) {
      int shiftX = ((pos.x() * 360 / width() - 180) -
                    (mousePos_.x() * 360 / width() - 180));
      int shiftY = ((pos.y() * 360 / height() - 180) -
                    (mousePos_.y() * 360 / height() - 180));

      Q_EMIT signalChangeRotateCoords(std::pair<int, int>{shiftY, shiftX});
    }
    mousePos_ = pos;
  }
}

void ControlWindow::mouseReleaseEvent([[maybe_unused]] QMouseEvent* event) {
  mouseAction_ = MouseAction::kNone;
}

void ControlWindow::wheelEvent(QWheelEvent* event) {
  Q_EMIT signalChangeScaleCoords(
      std::pair<int, int>{event->angleDelta().y() / 24, 0});  // 5 ед
}