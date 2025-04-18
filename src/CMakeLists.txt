cmake_minimum_required(VERSION 3.16)

project(3DViewer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -O2")

# find_package(TBB REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR}OpenGL REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS OpenGLWidgets)

set(PROJECT_SOURCES
        main.cc
        
        view/main_window.h
        view/main_window.cc
        view/info_window.h
        view/info_window.cc
        view/control_window.h
        view/control_window.cc 
        view/viewport3D.h
        view/viewport3D.cc
        view/background_box.h
        view/background_box.cc 
        view/elem_box.h
        view/elem_box.cc 
        view/sliders_box.h
        view/sliders_box.cc 
        view/user_setting.h
        view/user_setting.cc     
        
        model/facade.h
        model/facade.cc
        model/scene.h
        model/scene.cc
        model/obj/obj_data.h
        model/obj/obj_data.cc

        controller/controller.h
        controller/controller.cc
)



if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(3DViewer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET 3DViewer APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(3DViewer SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(3DViewer
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(3DViewer PRIVATE "${CMAKE_SOURCE_DIR}/include/")
target_include_directories(3DViewer PRIVATE "${CMAKE_SOURCE_DIR}/controller/")
target_include_directories(3DViewer PRIVATE "${CMAKE_SOURCE_DIR}/model/")
target_include_directories(3DViewer PRIVATE "${CMAKE_SOURCE_DIR}/view/")

# Settings for logger
add_definitions( -D LOGGER_MAX_LOG_LEVEL_PRINTED=6 )
add_definitions( -D LOGGER_PREFIX_LEVEL=2 )
add_definitions( -D LOGGER_ENABLE_COLORS=1 )
add_definitions( -D LOGGER_ENABLE_COLORS_ON_USER_HEADER=0 )

target_compile_definitions(3DViewer PRIVATE QT_NO_KEYWORDS)

target_link_libraries(3DViewer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(3DViewer PRIVATE Qt${QT_VERSION_MAJOR}::OpenGL)
target_link_libraries(3DViewer PRIVATE Qt${QT_VERSION_MAJOR}::OpenGLWidgets)
# target_link_libraries(3DViewer PRIVATE TBB::tbb)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.3DViewer)
endif()
set_target_properties(3DViewer PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS 3DViewer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(3DViewer)
endif()
