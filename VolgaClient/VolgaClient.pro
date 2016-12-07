#-------------------------------------------------
#
# Project created by QtCreator 2016-09-30T21:40:12
#
#-------------------------------------------------

QT       += core gui opengl
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Volga
TEMPLATE = app

QMAKE_LFLAGS += "-framework OpenCL"

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.11

DEFINES += USE_OPENCL RR_EMBED_KERNELS _DEBUG

SOURCES += main.cpp\
        volgamainwindow.cpp \
    renderopenglwidget.cpp \
    ../3rdparty/RadeonRays_SDK/App/AO/aorenderer.cpp \
    ../3rdparty/RadeonRays_SDK/App/Light/Ibl.cpp \
    ../3rdparty/RadeonRays_SDK/App/PT/ptrenderer.cpp \
    ../3rdparty/RadeonRays_SDK/App/Scene/scene.cpp \
    ../3rdparty/RadeonRays_SDK/App/Scene/scene_tracker.cpp \
    ../3rdparty/RadeonRays_SDK/App/config_manager.cpp \
    ../3rdparty/RadeonRays_SDK/App/perspective_camera.cpp \
    ../3rdparty/RadeonRays_SDK/App/sh.cpp \
    ../3rdparty/RadeonRays_SDK/App/shader_manager.cpp \
    ../3rdparty/RadeonRays_SDK/App/shproject.cpp \
    ../3rdparty/RadeonRays_SDK/App/tiny_obj_loader.cpp

HEADERS  += volgamainwindow.h \
    renderopenglwidget.h \
    ../3rdparty/RadeonRays_SDK/App/AO/aorenderer.h \
    ../3rdparty/RadeonRays_SDK/App/CL/cache/kernels.h \
    ../3rdparty/RadeonRays_SDK/App/CLW/clwoutput.h \
    ../3rdparty/RadeonRays_SDK/App/CLW/clwscene.h \
    ../3rdparty/RadeonRays_SDK/App/Core/output.h \
    ../3rdparty/RadeonRays_SDK/App/Core/renderer.h \
    ../3rdparty/RadeonRays_SDK/App/Light/Ibl.h \
    ../3rdparty/RadeonRays_SDK/App/PT/ptrenderer.h \
    ../3rdparty/RadeonRays_SDK/App/Scene/scene.h \
    ../3rdparty/RadeonRays_SDK/App/Scene/scene_tracker.h \
    ../3rdparty/RadeonRays_SDK/App/config_manager.h \
    ../3rdparty/RadeonRays_SDK/App/perspective_camera.h \
    ../3rdparty/RadeonRays_SDK/App/sh.h \
    ../3rdparty/RadeonRays_SDK/App/shader_manager.h \
    ../3rdparty/RadeonRays_SDK/App/shproject.h \
    ../3rdparty/RadeonRays_SDK/App/sobol.h \
    ../3rdparty/RadeonRays_SDK/App/tiny_obj_loader.h

INCLUDEPATH += ../3rdparty/RadeonRays_SDK/App/ \
                ../3rdparty/RadeonRays_SDK/RadeonRays/include/ \
                ../3rdparty/RadeonRays_SDK/CLW/ \
                ../3rdparty/RadeonRays_SDK/3rdParty/oiio16/include


FORMS    += volgamainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../3rdparty/RadeonRays_SDK/Bin/Debug/x64/ -lCLW64D
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../3rdparty/RadeonRays_SDK/Bin/Debug/x64/ -lCLW64Dd
else:unix: LIBS += -L$$PWD/../3rdparty/RadeonRays_SDK/Bin/Debug/x64/ -lCLW64D

INCLUDEPATH += $$PWD/../3rdparty/RadeonRays_SDK/CLW
DEPENDPATH += $$PWD/../3rdparty/RadeonRays_SDK/CLW

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../3rdparty/RadeonRays_SDK/Bin/Debug/x64/libCLW64D.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../3rdparty/RadeonRays_SDK/Bin/Debug/x64/libCLW64Dd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../3rdparty/RadeonRays_SDK/Bin/Debug/x64/CLW64D.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../3rdparty/RadeonRays_SDK/Bin/Debug/x64/CLW64Dd.lib
else:unix: PRE_TARGETDEPS += $$PWD/../3rdparty/RadeonRays_SDK/Bin/Debug/x64/libCLW64D.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../3rdparty/RadeonRays_SDK/Bin/Debug/x64/ -lCalc64D
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../3rdparty/RadeonRays_SDK/Bin/Debug/x64/ -lCalc64Dd
else:unix: LIBS += -L$$PWD/../3rdparty/RadeonRays_SDK/Bin/Debug/x64/ -lCalc64D

INCLUDEPATH += $$PWD/../3rdparty/RadeonRays_SDK/Calc/inc
DEPENDPATH += $$PWD/../3rdparty/RadeonRays_SDK/Calc/inc

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../3rdparty/RadeonRays_SDK/Bin/Debug/x64/libCalc64D.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../3rdparty/RadeonRays_SDK/Bin/Debug/x64/libCalc64Dd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../3rdparty/RadeonRays_SDK/Bin/Debug/x64/Calc64D.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../3rdparty/RadeonRays_SDK/Bin/Debug/x64/Calc64Dd.lib
else:unix: PRE_TARGETDEPS += $$PWD/../3rdparty/RadeonRays_SDK/Bin/Debug/x64/libCalc64D.a

DISTFILES += \
    ../3rdparty/RadeonRays_SDK/App/simple.fsh \
    ../3rdparty/RadeonRays_SDK/App/simple.vsh \
    shaders/fullscreentriangle.vert \
    shaders/fullscreentriangle.frag

macx: LIBS += -L/usr/local/lib/ -lOpenImageIO

LIBS += -L$$PWD/../3rdparty/RadeonRays_SDK/Bin/Debug/x64/ -lRadeonRays64D

INCLUDEPATH += $$PWD/../3rdparty/RadeonRays_SDK/RadeonRays/include
DEPENDPATH += $$PWD/../3rdparty/RadeonRays_SDK/RadeonRays/include

RESOURCES += \
    volga.qrc
