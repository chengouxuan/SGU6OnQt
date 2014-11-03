#include "searcherbridge.h"
#include "MoveSearcher.h"
#include "DTSSer.h"
#include "Frame.h"
#include <iostream>
#include "boardformatter.h"

SearcherBridge::SearcherBridge()
{
}

void SearcherBridge::invokeSearch(Board board, WhichPlayer whichPlayer, int movesToGo,
                                  int alphdBetaDepth, int timeLimitationMS, int dtssDepth, int idDtssDepth,
                                  int &r1, int &c1, int &r2, int &c2)
{
    MoveSearcher *searcher = &gSearcher;

    frame._depth_limit = 7;
    frame._time_limit = 10000;
    dtsser._dtss_depth = 9;
    dtsser._id_dtss_depth = 5;

    DMove dMove = searcher->SearchGoodMoves(board, whichPlayer, movesToGo);

    r1 = dMove.GetPoint1()._row;
    c1 = dMove.GetPoint1()._col;
    r2 = dMove.GetPoint2()._row;
    c2 = dMove.GetPoint2()._col;
}

void SearcherBridge::test()
{
    //    static char *testData = {
    //        "   A B C D E F G H I J K L M N o P Q R S" "\n"
    //        " 1 . . . . . . . . . . . . . . . . . . ." "\n"
    //        " 2 . . . . . . . . . . . . . . . . . . ." "\n"
    //        " 3 . . . . . . . . . . . . . . . . . . ." "\n"
    //        " 4 . . . . . . . . . . . . . . . . . . ." "\n"
    //        " 5 . . . . . . . . . . . . . . . . . . ." "\n"
    //        " 6 . . . . . . . . . . . . . . . . . . ." "\n"
    //        " 7 . . . . . X . O X . . . . . . . . . ." "\n"
    //        " 8 . . . . . . . . O X . . . . . . . . ." "\n"
    //        " 9 . . . . . . . O . X . . . . . . . . ." "\n"
    //        "10 . . . . . . . . O . . . . . . . . . ." "\n"
    //        "11 . . . . . . . . . . . . . . . . . . ." "\n"
    //        "12 . . . . . . . . . . . . . . . . . . ." "\n"
    //        "13 . . . . . . . . . . . X . . . . . . ." "\n"
    //        "14 . . . . . . . . . . . . . . . . . . ." "\n"
    //        "15 . . . . . . . . . . . . . . . . . . ." "\n"
    //        "16 . . . . . . . . . . . . . . . . . . ." "\n"
    //        "17 . . . . . . . . . . . . . . . . . . ." "\n"
    //        "18 . . . . . . . . . . . . . . . . . . ." "\n"
    //        "19 X . . . . . . . . . . . . . . . . . ." "\n"
    //    };
        static char *testData = {
            "   A B C D E F G H I J K L M N o P Q R S" "\n"
            " 1 . . . . . . . . . . . . . . . . . . ." "\n"
            " 2 . . . . . . . . . . . . . . . . . . ." "\n"
            " 3 . . . . . . . . . . . . . . . . . . ." "\n"
            " 4 . . . . . . . . . . . . . . . . . . ." "\n"
            " 5 . . . . . . . . . . . . . . . . . . ." "\n"
            " 6 . . . . . . . . . . . . . . . . . . ." "\n"
            " 7 . . . . . . . . . . . . . . . . . . ." "\n"
            " 8 . . . . . . . . . . . . . . . . . . ." "\n"
            " 9 . . . . . . . . . . . . . . . . . . ." "\n"
            "10 . . . . . . . . X O . . . . . . . . ." "\n"
            "11 . . . . . . . . X O . . . . . . . . ." "\n"
            "12 . . . . . . . . . X . . . . . . . . ." "\n"
            "13 . . . . . . . . . . . . . . . . . . ." "\n"
            "14 . . . . . . . . . . . . . . . . . . ." "\n"
            "15 . . . . . . . . . . . . . . . . . . ." "\n"
            "16 . . . . . . . . . . . . . . . . . . ." "\n"
            "17 . . . . . . . . . . . . . . . . . . ." "\n"
            "18 . . . . . . . . . . . . . . . . . . ." "\n"
            "19 X . . . . . . . . . . . . . . . . . ." "\n"
        };

    int r1, c1, r2, c2;
    SearcherBridge::invokeSearch(BoardFormatter::stringToBoard(testData).board,
                                 WhitePlayer, 2,
                                 7, 20000, 9, 5,
                                 r1, c1, r2, c2);

    std::cerr << "((" << r1 << ", " << (char)(c1 + 'A') << "),("
                          << r2 << ", " << (char)(c2 + 'A') << "))\n";
}
