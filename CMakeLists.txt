cmake_minimum_required(VERSION 3.24)
project(QtTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/Sources/Headers)


find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(QtTest Sources/main.cpp Sources/mainwindow.cpp Sources/Headers/mainwindow.h Sources/Forms/mainwindow.ui)
target_link_libraries(QtTest
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )

