#pragma once

#include <QGraphicsDropShadowEffect>  //!
#include <QHBoxLayout>
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

class CentralWindow : public QWidget {
  Q_OBJECT

 public:
  CentralWindow(QOpenGLWidget* renderWindow, QWidget* parent = nullptr)
      : QWidget(parent) {
    // Основной layout
    QGridLayout* mainLayout = new QGridLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mainLayout->addWidget(renderWindow);

    // контейнер для кнопок
    QWidget* buttonContainer = new QWidget(this);
    buttonContainer->setAttribute(Qt::WA_TranslucentBackground);

    QVBoxLayout* buttonsLayout = new QVBoxLayout(buttonContainer);
    buttonsLayout->setSpacing(10);

    openButton = new QPushButton("Open", this);
    saveButton = new QPushButton("Setings", this);
    jpegButton = new QPushButton("jpeg", this);
    bmpButton = new QPushButton("bmp", this);
    customGifButton = new QPushButton("gif1", this);
    cycledGifButton = new QPushButton("gif2", this);

    QString buttonStyle =
        "QPushButton {"
        "  background-color: #404040;"
        "  border-radius: 25px;"  // круглое
        "  min-width: 50px;"
        "  max-width: 50px;"
        "  min-height: 50px;"
        "  max-height: 50px;"
        "  font-size: 18px;"  //! шрифт - убрать
        "}"
        "QPushButton:hover {"
        "  background-color: grey;"
        "}"
        "QPushButton:pressed {"
        "  background-color: lightgrey;"
        "}";
    openButton->setStyleSheet(buttonStyle);
    // openButton->setIconSize(QSize(30, 30));
    saveButton->setStyleSheet(buttonStyle);
    jpegButton->setStyleSheet(buttonStyle);
    bmpButton->setStyleSheet(buttonStyle);
    customGifButton->setStyleSheet(buttonStyle);
    cycledGifButton->setStyleSheet(buttonStyle);

    buttonsLayout->addWidget(openButton);
    buttonsLayout->addWidget(saveButton);
    buttonsLayout->addWidget(jpegButton);
    buttonsLayout->addWidget(bmpButton);
    buttonsLayout->addWidget(customGifButton);
    buttonsLayout->addWidget(cycledGifButton);

    // поверх окна рендера
    mainLayout->addWidget(buttonContainer, 0, 0, Qt::AlignRight | Qt::AlignTop);

    // отступы от краев
    buttonsLayout->setContentsMargins(0, 10, 10, 0);

    //! shadow
    // buttonContainer->setStyleSheet(
    //     "QWidget {"
    //     "  background-color: rgba(240,240,240,100);"
    //     "  border-radius: 6px;"
    //     "}");
    // buttonContainer->setGraphicsEffect(new QGraphicsDropShadowEffect(this));

    // для настроек
    settingMenu = new QMenu(saveButton);
    settingMenu->setStyleSheet(
        "QMenu {"
        "  background-color: #404040;"
        "  border-radius: 8px;"
        "  padding: 5px;"
        "}"
        "QMenu::item {"
        "  padding: 8px 25px;"
        "  border-radius: 4px;"
        "}"
        "QMenu::item:selected {"
        "  background-color: gray;"
        "}"
        "QMenu::separator {"
        "  height: 1px;"
        "  background: background-color: #323232;"
        "}");
    QAction* saveAction = settingMenu->addAction("Save");
    QAction* restoreAction = settingMenu->addAction("Restore");
    QAction* resetAction = settingMenu->addAction("Reset");

    connect(saveButton, &QPushButton::clicked, [this] {
      QPoint pos = saveButton->mapToGlobal(QPoint(0, saveButton->height()));
      settingMenu->exec(pos);
    });

    connect(saveAction, &QAction::triggered, this, &CentralWindow::handleSave);
    connect(restoreAction, &QAction::triggered, this,
            &CentralWindow::handleRestore);
    connect(resetAction, &QAction::triggered, this,
            &CentralWindow::handleReset);
  }

 Q_SIGNALS:
  void signalChangeSize(const int w, const int h);
  void signalChangeMoveCoords(std::pair<int, int> coordXY);
  void signalChangeScaleCoords(std::pair<int, int> coordXY);
  void signalChangeRotateCoords(std::pair<int, int> coordXY);

 private:
  QPushButton *openButton, *saveButton, *jpegButton, *bmpButton,
      *customGifButton, *cycledGifButton;
  QMenu* settingMenu;

  // catch mouse press and mouse release
  MouseAction mouseAction = MouseAction::kNone;
  QPoint mousePos;

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
        int shiftX = -((pos.x() * 360 / width() - 180) -
                       (mousePos.x() * 360 / width() - 180));
        int shiftY = -((pos.y() * 360 / height() - 180) -
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

  void handleSave() { qDebug() << "Save triggered"; }
  void handleRestore() { qDebug() << "Restore triggered"; }
  void handleReset() { qDebug() << "Reset triggered"; }
};