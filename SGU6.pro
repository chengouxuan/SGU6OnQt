QT += widgets

HEADERS       = \
                window.h \
                boardwidget.h \
    boardpainter.h \
    gamelogic.h \
    AI/mockai.h \
    aicontroller.h \
    threadrunreimpelement.h \
    AI/Array.h \
    AI/ComplexBoard.h \
    AI/connect6.h \
    AI/defines.h \
    AI/DPointTable.h \
    AI/DTSSer.h \
    AI/Evaluator.h \
    AI/Frame.h \
    AI/HistoryScore.h \
    AI/mockai.h \
    AI/MoveArray.h \
    AI/MoveGenerator.h \
    AI/MoveSearcher.h \
    AI/OpeningFinder.h \
    AI/RandTable.h \
    AI/RZDeferder.h \
    AI/SegmentTable.h \
    AI/TranspositionTable.h \
    AI/utilities.h \
    AI/Zone.h

SOURCES       = main.cpp \
                window.cpp \
                boardwidget.cpp \
                boardpainter.cpp \
    gamelogic.cpp \
    AI/mockai.cpp \
    aicontroller.cpp \
    AI/ComplexBoard.cpp \
    AI/debugmain.cpp \
    AI/DPointTable.cpp \
    AI/DTSSer.cpp \
    AI/Evaluator.cpp \
    AI/Frame.cpp \
    AI/HistoryScore.cpp \
    AI/main.cpp \
    AI/mockai.cpp \
    AI/MoveGenerator.cpp \
    AI/MoveSearcher.cpp \
    AI/OpeningFinder.cpp \
    AI/RandTable.cpp \
    AI/RZDefender.cpp \
    AI/SegmentTable.cpp \
    AI/utilities.cpp \
    AI/Zone.cpp

RESOURCES     = \
    sgu6.qrc
