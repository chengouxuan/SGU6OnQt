
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

DWORD SearchThread(LPVOID p) {
    if (evaluator == NULL) {
        evaluator = new Evaluator;
    }
    frame.Search(searcher._board, searcher._isBlack);
    return 0;
}



void MoveSearcher::SearchGoodMoves(Board board, bool isBlack, int moves /* = 2 */) {
    if(moves == 1) {
        _dMove._r1 = RowMax / 2;
        _dMove._c1 = ColumnMax / 2;
        return;
    } else if(openingFinder.Find(board, isBlack)) {
        _dMove = openingFinder._dMove;
        return;
    }
    // begin the search thread
    _isBlack = isBlack;
    memcpy(_board, board, sizeof(Board));
    SearchThread(0);
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

void ReInitParams() {
    frame._depth_limit = 7;
    frame._time_limit = 10000;
    searcher._time_limit = 20000;
    dtsser._dtss_depth = 9;
    dtsser._id_dtss_depth = 5;
}

int MoveSearcher::_time_limit;

MoveSearcher searcher;
