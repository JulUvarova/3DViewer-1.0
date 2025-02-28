#pragma once

#include <QFileDialog>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QIcon>
#include <QMenu>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QPushButton>
#include <QWidget>

enum class MouseAction {
  kNone = 0,
  kLeftButton,
  kMiddleButton,
};

class ControlWindow : public QWidget {
  Q_OBJECT

 public:
  ControlWindow(QWidget* parent = nullptr) : QWidget(parent) {
    QGridLayout* mainLayout = new QGridLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QWidget* buttonContainer = new QWidget(this);
    buttonContainer->setAttribute(Qt::WA_TranslucentBackground);

    QVBoxLayout* buttonsLayout = new QVBoxLayout(buttonContainer);
    buttonsLayout->setSpacing(10);

    openButton = new QPushButton(this);
    jpegButton = new QPushButton(this);
    bmpButton = new QPushButton(this);
    customGifButton = new QPushButton(this);
    cycledGifButton = new QPushButton(this);

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
    openButton->setStyleSheet(buttonStyle);
    openButton->setIcon(QIcon("view/resources/open_64_negate.png"));
    openButton->setIconSize(QSize(30, 30));
    jpegButton->setStyleSheet(buttonStyle);
    jpegButton->setIcon(QIcon("view/resources/jpg_64_negate.png"));
    jpegButton->setIconSize(QSize(30, 30));
    bmpButton->setStyleSheet(buttonStyle);
    bmpButton->setIcon(QIcon("view/resources/bmp_64_negate.png"));
    bmpButton->setIconSize(QSize(30, 30));
    customGifButton->setStyleSheet(buttonStyle);
    customGifButton->setIcon(QIcon("view/resources/gif_64_negate.png"));
    customGifButton->setIconSize(QSize(30, 30));
    cycledGifButton->setStyleSheet(buttonStyle);
    cycledGifButton->setIcon(QIcon("view/resources/cgif_64_negate.png"));
    cycledGifButton->setIconSize(QSize(30, 30));

    buttonsLayout->addWidget(openButton);
    buttonsLayout->addWidget(jpegButton);
    buttonsLayout->addWidget(bmpButton);
    buttonsLayout->addWidget(customGifButton);
    buttonsLayout->addWidget(cycledGifButton);

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

    connect(openButton, &QPushButton::clicked, this, &ControlWindow::openFile);
    connect(bmpButton, &QPushButton::clicked, this,
            [this]() { saveFile(".bmp", 0); });
    connect(jpegButton, &QPushButton::clicked, this,
            [this]() { saveFile(".jpeg", 0); });
    connect(customGifButton, &QPushButton::clicked, this,
            [this]() { saveFile(".gif", 1); });
    connect(cycledGifButton, &QPushButton::clicked, this,
            [this]() { saveFile(".gif", 2); });
  }

 Q_SIGNALS:
  void signalChangeSize(const int w, const int h);
  void signalChangeMoveCoords(std::pair<int, int> coordXY);
  void signalChangeScaleCoords(std::pair<int, int> coordXY);
  void signalChangeRotateCoords(std::pair<int, int> coordXY);
  void signalOpenFile(QString& fname);
  void signalSaveFile(QString& fname);
  void signalStartCustomGif(QString& fname);
  void signalStartCycledGif(QString& fname);

 private:
  QPushButton *openButton, *jpegButton, *bmpButton, *customGifButton,
      *cycledGifButton;
  QMenu* settingMenu;

  // catch mouse press and mouse release
  MouseAction mouseAction = MouseAction::kNone;
  QPoint mousePos;

  void openFile() {
    QString filename = QFileDialog::getOpenFileName(this, tr("Open file"), "./",
                                                    tr("Images (*.obj)"));
    if (!filename.isEmpty())
      Q_EMIT signalOpenFile(filename);
  }

  void saveFile(const char* options, int gif) {
    QString selectedFilter;
    QString filename = QFileDialog::getSaveFileName(
        this, tr("Choose folder"), "./", tr(options), &selectedFilter);
    if (!filename.isEmpty()) {
      filename.append(selectedFilter.remove("*"));
      if (filename.endsWith("gif") && gif == 1) {  // TODO как-то поумнее
        Q_EMIT signalStartCustomGif(filename);
      } else if (filename.endsWith("gif") && gif == 2) {
        Q_EMIT signalStartCycledGif(filename);
      } else if (filename.endsWith("bmp") || filename.endsWith("jpeg")) {
        Q_EMIT signalSaveFile(filename);
      }
    }
  }

  void mousePressEvent(QMouseEvent* event) override {
    if (event->button() == Qt::LeftButton)
      mouseAction = MouseAction::kLeftButton;
    else if (event->button() == Qt::MiddleButton)
      mouseAction = MouseAction::kMiddleButton;

    if (mouseAction != MouseAction::kNone) {
      mousePos = event->pos();
    }
  }

  void mouseMoveEvent(QMouseEvent* event) override {
    if (mouseAction != MouseAction::kNone) {
      QPoint pos = event->pos();

      // left will move object
      if (mouseAction == MouseAction::kLeftButton) {
        int shiftX = (pos.x() * 200 / width() - 100) -
                     (mousePos.x() * 200 / width() - 100);
        int shiftY = -((pos.y() * 200 / height() - 100) -
                       (mousePos.y() * 200 / height() - 100));

        Q_EMIT signalChangeMoveCoords(
            std::pair<int, int>{2 * shiftX, 2 * shiftY});
      }
      // middle will rotate object
      if (mouseAction == MouseAction::kMiddleButton) {
        int shiftX = ((pos.x() * 360 / width() - 180) -
                      (mousePos.x() * 360 / width() - 180));
        int shiftY = ((pos.y() * 360 / height() - 180) -
                      (mousePos.y() * 360 / height() - 180));

        Q_EMIT signalChangeRotateCoords(std::pair<int, int>{shiftY, shiftX});
      }
      mousePos = pos;
    }
  }

  void mouseReleaseEvent([[maybe_unused]] QMouseEvent* event) override {
    mouseAction = MouseAction::kNone;
  }

  void wheelEvent(QWheelEvent* event) {
    Q_EMIT signalChangeScaleCoords(
        std::pair<int, int>{event->angleDelta().y() / 24, 0});  // 5 ед
  }
};