cmake_minimum_required(VERSION 3.14)

project(BioDeg-UI LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR}Charts)

add_executable(BioDeg-UI
src/main.cpp
src/mainwindow.cpp
src/mainwindow.h
src/mainwindow.ui
src/global.h
src/aboutdialog.cpp
src/aboutdialog.h
src/aboutdialog.ui
src/preprocessdialog.h
src/preprocessdialog.cpp
src/preprocessdialog.ui
)

target_link_libraries(BioDeg-UI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(BioDeg-UI PRIVATE Qt${QT_VERSION_MAJOR}::Charts)

# Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  test_BioDeg
  tests/test_biodeg.cpp
  tests/tests.cpp
  tests/utils.cpp
)

set_target_properties(test_BioDeg PROPERTIES CXX_STANDARD 17)

target_link_libraries(
  test_BioDeg
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_BioDeg)
