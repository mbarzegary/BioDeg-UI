cmake_minimum_required(VERSION 3.5)

project(BioDeg-UI LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR}Charts)

add_executable(BioDeg-UI
main.cpp
mainwindow.cpp
mainwindow.h
mainwindow.ui
global.h
aboutdialog.cpp
aboutdialog.h
aboutdialog.ui
preprocessdialog.h
preprocessdialog.cpp
preprocessdialog.ui
)

target_link_libraries(BioDeg-UI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(BioDeg-UI PRIVATE Qt${QT_VERSION_MAJOR}::Charts)
