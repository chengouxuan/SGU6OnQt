QT += widgets

HEADERS       = \
                window.h \
                boardwidget.h \
    boardpainter.h \
    gamelogic.h

SOURCES       = main.cpp \
                window.cpp \
                boardwidget.cpp \
                boardpainter.cpp \
    gamelogic.cpp

RESOURCES     = \
    sgu6.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/painting/basicdrawing
INSTALLS += target
