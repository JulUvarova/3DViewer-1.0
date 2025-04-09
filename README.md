# 3DViewer v1.0

## Оглавление

- [О проекте](#о-проекте)
  - [Описание](#описание)
  - [Пример работы](#пример-работы)
  - [Инструменты разработки](#инструменты-разработки)
- [Установка](#установка)
  - [Предустановка](#предустановка)
  - [Таргеты](#таргеты)
- [Участники проекта](#участники-проекта)

---


## О проекте

### Описание
Учебный проект 3DViewer представляет собой программу для визуализации каркасной модели в трехмерном пространстве.
Программа позволяет:
- загружать каркасную модель из файла формата obj (поддержка только списка вершин и поверхностей);
- корректно обрабатывать и позволять пользователю просматривать модели с деталями до 100, 1000, 10 000, 100 000, 1 000 000 вершин без зависания (примеры простых моделей в src/view/primitives, крупных - https://disk.yandex.ru/d/WUhyihtAWnTGpA);
- перемещать модель на заданное расстояние относительно осей X, Y, Z;
- поворачивать модель на заданный угол относительно своих осей X, Y, Z;
- масштабировать модель на заданное значение;
- настраивать тип проекции (параллельная и центральная);
- настраивать тип (сплошная, пунктирная), цвет и толщину ребер, способ отображения (отсутствует, круг, квадрат), цвет и размер вершин;
- выбирать цвет фона;
- сохранять выбранные настройки между перезапусками программы;
- сохранять полученные («отрендеренные») изображения в файл в форматах bmp и jpeg;
- записывать небольшие «скринкасты» — текущие пользовательские аффинные преобразования загруженного объекта в gif-анимацию (640x480, 10fps, 5s) циклическую (от начального положения до указанного пользователем и обратно) и кастомную (действия пользователя с моделью).

### Пример работы

![Example GIF](src/3dviewer.gif)

### Инструменты разработки

<p align="center">
  <p>
    <a href="https://www.cplusplus.com/">
      <img src="https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" width="80" height="30">
    </a>
    <a href="https://cmake.org/">
      <img src="https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white" width="80" height="30">
    </a>
    <a href="https://www.qt.io/">
      <img src="https://img.shields.io/badge/Qt-%23217346.svg?style=for-the-badge&logo=Qt&logoColor=white" width="80" height="30">
    </a>
    <a href="https://www.opengl.org/">
        <img src="https://img.shields.io/badge/OpenGL-%233D3D3D.svg?style=for-the-badge&logo=opengl&logoColor=white" width="80" height="30">
    </a>
  </p>
</p>

<p align="right">(<a href="#readme-top">back to top</a>)</p>


## Установка

Для установки проекта следуйте следующим шагам.

### Предустановка

Проверьте, что в системе установлены следующие библиотеки:

* CMake
  ```
  sudo apt install cmake
  ```

* Qt
  ```
  sudo apt-get install build-essential libgl1-mesa-dev
  ```

* Google test
  ```
  sudo apt install libgtest-dev
  ``` 

* lcov
  ```
  sudo apt-get install lcov
  ```

* Doxygen
  ```
  sudo apt-get install doxygen
  ```

### Таргеты

Проект включает цели:

* all - сборка и установка игры в папку build
  ```
  make all
  ```

* install - сборка и установка игры в папку build
  ```
  make install
  ```

* uninstall - очистка содержимого папки build
  ```
  make uninstall
  ```

* test - запускает тестирование бекенда игры, включая проверку на утечки
  ```
  make test
  ```

* gcov_report - создание отчета о тестовом покрытии бекенда игры 
  ```
  make gcov_report
  ```

* check - проверка файлов проекта с помощью clang-format и cpp-check 
  ```
  make check
  ```

* dvi - генерирует документацию проекта
  ```
  make dvi
  ```

* dist - архивирование кода программы
  ```
  make dist
  ```

<p align="right">(<a href="#readme-top">back to top</a>)</p>


## Участники проекта

| Участник | Задачи |
| ------ | ------ |
| [robynarl](https://github.com/antsht) | тимлид, финальная проверка проекта, парсер, афинные преобразования и их тестирование |
| [trumpecy](https://github.com/JulUvarova) | графический интерфейс, сохранение настроек пользователя |

<p align="right">(<a href="#readme-top">back to top</a>)</p>