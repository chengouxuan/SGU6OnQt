QT += widgets

HEADERS       = \
                window.h \
                boardwidget.h \
    boardpainter.h

SOURCES       = main.cpp \
                window.cpp \
                boardwidget.cpp \
    boardpainter.cpp

RESOURCES     = basicdrawing.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/painting/basicdrawing
INSTALLS += target
