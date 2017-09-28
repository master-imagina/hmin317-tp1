QT       += core gui widgets

TARGET = cube
TEMPLATE = app

SOURCES += main.cpp

SOURCES += \
    mainwidget.cpp \
    geometryengine.cpp

HEADERS += \
    mainwidget.h \
    geometryengine.h

RESOURCES += \
    shaders.qrc \
    textures.qrc
CONFIG += c++11
# install
target.path = $$[QT_INSTALL_EXAMPLES]/opengl/cube
INSTALLS += target
