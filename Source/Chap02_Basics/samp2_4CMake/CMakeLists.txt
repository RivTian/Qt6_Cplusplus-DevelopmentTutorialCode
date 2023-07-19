cmake_minimum_required(VERSION 3.5)         #需要的CMake最低版本

project(samp2_4 VERSION 0.1 LANGUAGES CXX)  #项目版本0.1, 编程语言是C++

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)       # UIC能被自动执行
set(CMAKE_AUTOMOC ON)       # MOC能被自动执行
set(CMAKE_AUTORCC ON)       # RCC能被自动执行

set(CMAKE_CXX_STANDARD 11)  #设置编译器需要满足的C++语言标准， 设置为 C++ 11
set(CMAKE_CXX_STANDARD_REQUIRED ON)  #要求编译器满足 C++标准

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)       #查找Qt6或Qt5，导入Widgets模块
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)  #查找Qt6，导入Widgets模块

set(PROJECT_SOURCES         #设置变量 PROJECT_SOURCES 等于下面的列表
        main.cpp            #也就是项目的源文件列表
        dialog.cpp
        dialog.h
        dialog.ui
        res.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)    #如果是Qt6以上的版本
    qt_add_executable(samp2_4       #创建可执行文件 samp2_4
        MANUAL_FINALIZATION         #可选参数，手动结束创建目标的过程
        ${PROJECT_SOURCES}          #文件列表来源于前面定义的变量PROJECT_SOURCES
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET samp2_4 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(samp2_4 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(samp2_4
            ${PROJECT_SOURCES}
        )
    endif()
endif()

#在连接生成目标samp2_4时, 需要利用前面用find_package()导入的 Qt6::Widgets库
target_link_libraries(samp2_4 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(samp2_4 PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(samp2_4)     #结束生成可执行文件的过程
endif()
