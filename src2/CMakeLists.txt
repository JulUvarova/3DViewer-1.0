cmake_minimum_required(VERSION 3.21)
project(BlenderLikeUI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets OpenGLWidgets REQUIRED)

add_executable(${PROJECT_NAME}
  main.cpp
  MainWindow.h
  MainWindow.cpp
  Viewport3D.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  Qt6::Core
  Qt6::Gui
  Qt6::Widgets
  Qt6::OpenGLWidgets
)