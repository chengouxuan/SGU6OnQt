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
    MoveSearcher *searcher = new MoveSearcher;

    frame._depth_limit = alphdBetaDepth;
    frame._time_limit = timeLimitationMS;
    dtsser._dtss_depth = dtssDepth;
    dtsser._id_dtss_depth = idDtssDepth;

    DMove dMove = searcher->SearchGoodMoves(board, whichPlayer, movesToGo);

    r1 = dMove.GetPoint1()._row;
    c1 = dMove.GetPoint1()._col;
    r2 = dMove.GetPoint2()._row;
    c2 = dMove.GetPoint2()._col;

    delete searcher;
}

void SearcherBridge::test()
{
    const char *testData =

        "   A B C D E F G H I J K L M N o P Q R S" "\n"
        " 1 . . . . . . . . . . . . . . . . . . ." "\n"
        " 2 . . . . . . . . . . . . . . . . . . ." "\n"
        " 3 . . . . . . . . . . . . . . . . . . ." "\n"
        " 4 . . . . . . . . . . . . . . . . . . ." "\n"
        " 5 . . . . . . . . . . . . . . . . . . ." "\n"
        " 6 . . . . . . . . . . . . . . . . . . ." "\n"
        " 7 . . . . . . . . . . . . . . . . . . ." "\n"
        " 8 . . . . . . . . . O . . . . . . . . ." "\n"
        " 9 . . . . . . . . O . . . . . . . . . ." "\n"
        "10 . . . . . . . X . X . . . . . . . . ." "\n"
        "11 . . . . . . . . X . . . . . . . . . ." "\n"
        "12 . . . . . . . . . . . . . . . . . . ." "\n"
        "13 . . . . . . . . . . . . . . . . . . ." "\n"
        "14 . . . . . . . . . . . . . . . . . . ." "\n"
        "15 . . . . . . . . . . . . . . . . . . ." "\n"
        "16 . . . . . . . . . . . . . . . . . . ." "\n"
        "17 . . . . . . . . . . . . . . . . . . ." "\n"
        "18 . . . . . . . . . . . . . . . . . . ." "\n"
        "19 . . . . . . . . . . . . . . . . . . ." "\n";


    int r1, c1, r2, c2;
    BoardStruct boardStruct = BoardFormatter::stringToBoard(testData);
    SearcherBridge::invokeSearch(boardStruct.board,
                                 WhitePlayer, 2,
                                 3, Infinity, 0, 0,
                                 r1, c1, r2, c2);

    std::cerr << "((" << r1 + 1 << ", " << (char)(c1 + 'A') << "),("
                          << r2 + 1 << ", " << (char)(c2 + 'A') << "))\n" << std::endl;

    assert(boardStruct.board[r1][c1] == CellTypeEmpty);
    assert(boardStruct.board[r2][c2] == CellTypeEmpty);
}
