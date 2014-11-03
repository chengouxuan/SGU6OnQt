
#include "defines.h"
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <algorithm>
#include "MoveSearcher.h"
#include "MoveArray.h"
//#include "DMoveArray.h"
#include "Zone.h"
#include "DTSSer.h"
#include "HistoryScore.h"
#include "Frame.h"
#include <queue>
#include <numeric>
#include "OpeningFinder.h"


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

MoveSearcher gSearcher;
