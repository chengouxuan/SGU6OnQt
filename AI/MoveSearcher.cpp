
#include "defines.h"
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <algorithm>
#include "MoveSearcher.h"
#include "MoveArray.h"
#include "Zone.h"
#include "DTSSer.h"
#include "HistoryScore.h"
#include "Frame.h"
#include <queue>
#include <numeric>
#include "OpeningFinder.h"
#include "boardformatter.h"
#include <iostream>
#include <iomanip>


DMove MoveSearcher::SearchGoodMoves(Board board, bool isBlack, int moves /* = 2 */) {
    DMove dMove;
    if(moves == 1) {
        dMove._r1 = RowMax / 2;
        dMove._c1 = ColumnMax / 2;
        return dMove;
    } else if(openingFinder.find(board, isBlack, dMove)) {
        return dMove;
    }
    if (evaluator == NULL) {
        evaluator = new Evaluator;
    }
    dMove = frame.Search(board, isBlack);
    return dMove;
}


DMove RandomDMove(bool isBlacksTurn) {
    DMove ret;
    do {
        ret = DMove(rand() % RowMax, rand() % ColumnMax, rand() % RowMax, rand() % ColumnMax, isBlacksTurn);
    } while(!IsValid(ret));
    return ret;
}

int CountThreats(bool isBlack) {
    SegmentTable::Table &four = GetSegmentTable(!isBlack, 4);
    SegmentTable::Table &five = GetSegmentTable(!isBlack, 5);
    if(four.Size() + five.Size() == 0) {
        return 0;
    }
    SegmentTable::Table &tab = (four.Size() == 0 ? five : four);
    FOR_EACH(k, 6) {
        Point p = tab.GetItem(0).GetPoint(k);
        if(::GetCell(p) != CellTypeEmpty) {
            continue;
        }
        ::MakeMove(p, isBlack);
        int t = four.Size() + five.Size();
        ::UnmakeLastMove();
        if(t == 0) {
            return 1;
        }
    }
    return 2;
}

void MoveSearcher::test()
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
        "19 . . . . . . . . . . . . . . . . . . ." "\n";


    frame._time_limit = 20000;
    frame._depth_limit = 7;
    dtsser._dtss_depth = 9;
    dtsser._id_dtss_depth = 5;


    BoardStruct boardStruct = BoardFormatter::stringToBoard(testData);
    MoveSearcher *moveSearcher = new MoveSearcher;

    DMove dMove = moveSearcher->SearchGoodMoves(boardStruct.board, false);

    std::cerr << "((" << dMove.GetPoint1()._row + 1 << ", " << (char)(dMove.GetPoint1()._col + 'A') << "),("
                          << dMove.GetPoint2()._row + 1 << ", " << (char)(dMove.GetPoint2()._col + 'A') << "))\n" << std::endl;

    assert(boardStruct.board[dMove.GetPoint1()._row][dMove.GetPoint1()._col] == CellTypeEmpty);
    assert(boardStruct.board[dMove.GetPoint2()._row][dMove.GetPoint2()._col] == CellTypeEmpty);
}

