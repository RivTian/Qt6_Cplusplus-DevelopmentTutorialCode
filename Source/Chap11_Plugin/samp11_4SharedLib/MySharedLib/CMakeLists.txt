cmake_minimum_required(VERSION 3.14)

project(MySharedLib LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

#add_library用于生成静态库或动态库，SHARED表示动态库（共享库）
add_library(MySharedLib SHARED
  MySharedLib_global.h
  tpendialog.cpp
  tpendialog.h
  tpendialog.ui
)

target_link_libraries(MySharedLib PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_compile_definitions(MySharedLib PRIVATE MYSHAREDLIB_LIBRARY)
