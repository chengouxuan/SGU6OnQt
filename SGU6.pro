QT += widgets

HEADERS       = \
                window.h \
                boardwidget.h \
    boardpainter.h \
    gamelogic.h \
    AI/mockai.h \
    aicontroller.h \
    AI/Array.h \
    AI/ComplexBoard.h \
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
    AI/Zone.h \
    AI/UtilityTypes.h \
    logger.h \
    globaldef.h \
    AI/boardformatter.h \
    AI/searcherbridge.h \
    AI/test.h

SOURCES       = main.cpp \
                window.cpp \
                boardwidget.cpp \
                boardpainter.cpp \
    gamelogic.cpp \
    aicontroller.cpp \
    AI/ComplexBoard.cpp \
    AI/DPointTable.cpp \
    AI/DTSSer.cpp \
    AI/Evaluator.cpp \
    AI/Frame.cpp \
    AI/HistoryScore.cpp \
    AI/MoveGenerator.cpp \
    AI/MoveSearcher.cpp \
    AI/OpeningFinder.cpp \
    AI/RandTable.cpp \
    AI/RZDefender.cpp \
    AI/SegmentTable.cpp \
    AI/utilities.cpp \
    AI/Zone.cpp \
    logger.cpp \
    AI/boardformatter.cpp \
    AI/searcherbridge.cpp

RESOURCES     = \
    sgu6.qrc
