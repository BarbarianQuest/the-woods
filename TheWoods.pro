TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SRCDIR = "src"

INCLUDEPATH += "SFML/include" \
    "$$SRCDIR/window" \
    "$$SRCDIR/application" \
    "$$SRCDIR/utils" \
    "$$SRCDIR/game" \
    "$$SRCDIR/game/geometry" \
    "$$SRCDIR/input" \
    "$$SRCDIR/components" \
    "$$SRCDIR/gameobject" \
    "$$SRCDIR/factories" \
    "$$SRCDIR/engines" \
    "$$SRCDIR/shapes" \
    "$$SRCDIR/logic"

HEADERS += $$SRCDIR/window/gamewindow.h \
    src/window/windowmanager.h \
    src/utils/debugutils.h \
    src/application/app.h \
    src/game/game.h \
    src/typedef/apptypes.h \
    src/utils/timeutils.h \
    src/game/geometry/grid.h \
    src/utils/mathsutils.h \
    src/utils/logger.h \
    src/utils/loggerfactory.h \
    src/input/inputmap.h \
    src/input/keyinput.h \
    src/gameobject/gameobject.h \
    src/components/component.h \
    src/factories/gameobjectfactory.h \
    src/gameobject/gameobjectstack.h \
    src/components/transform.h \
    src/factories/componentfactory.h \
    src/factories/factory.h \
    src/engines/engine.h \
    src/engines/transformengine.h \
    src/shapes/crossshape.h \
    src/shapes/customshape.h \
    src/components/playerinput.h \
    src/engines/inputengine.h \
    src/engines/eventengine.h \
    src/logic/event.h \
    src/factories/eventfactory.h \
    src/input/basicinput.h \
    src/input/buttoninput.h

SOURCES += src/main.cpp $$SRCDIR/window/gamewindow.cpp \
    src/window/windowmanager.cpp \
    src/utils/debugutils.cpp \
    src/application/app.cpp \
    src/game/game.cpp \
    src/utils/timeutils.cpp \
    src/game/geometry/grid.cpp \
    src/utils/mathsutils.cpp \
    src/utils/logger.cpp \
    src/utils/loggerfactory.cpp \
    src/input/inputmap.cpp \
    src/input/keyinput.cpp \
    src/gameobject/gameobject.cpp \
    src/components/component.cpp \
    src/factories/gameobjectfactory.cpp \
    src/gameobject/gameobjectstack.cpp \
    src/components/transform.cpp \
    src/factories/componentfactory.cpp \
    src/factories/factory.cpp \
    src/engines/engine.cpp \
    src/engines/transformengine.cpp \
    src/shapes/crossshape.cpp \
    src/shapes/customshape.cpp \
    src/components/playerinput.cpp \
    src/engines/inputengine.cpp \
    src/engines/eventengine.cpp \
    src/logic/event.cpp \
    src/factories/eventfactory.cpp \
    src/input/basicinput.cpp \
    src/input/buttoninput.cpp

LIBS += -L"$$PWD/SFML/lib"

CONFIG(release, debug|release): LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system
CONFIG(debug, debug|release): LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-network-d -lsfml-window-d -lsfml-system-d

INCLUDEPATH += "SFML/include"
DEPENDPATH += "SFML/include"
