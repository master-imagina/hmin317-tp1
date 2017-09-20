cmake_minimum_required(VERSION 3.5)

project(gameenginepw)

# C++ language version
set(CMAKE_CXX_STANDARD 14)

# Files
set(HEADERS
    mainwidget.h
    geometryengine.h
    cameracontroller.h
    )

set(SOURCES
    mainwidget.cpp
    geometryengine.cpp
    cameracontroller.cpp
    main.cpp
    )

set(QT_RESOURCES
    shaders.qrc
    textures.qrc)

# Packages
#  Qt5
find_package(Qt5 COMPONENTS Widgets REQUIRED)
set(CMAKE_AUTOMOC true)
set(CMAKE_AUTORCC true)

#  Binary
add_executable(tp1 ${SOURCES} ${HEADERS} ${QT_RESOURCES})

target_link_libraries(tp1 Qt5::Widgets)
