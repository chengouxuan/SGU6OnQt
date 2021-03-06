#include "Frame.h"
//#include "DMoveArray.h"
#include "ComplexBoard.h"
#include "MoveSearcher.h"
#include "MoveGenerator.h"
#include <algorithm>

void Frame::SortByHistoryScore(MGDMoveArray &dMove) {
    FOR_EACH(i, dMove.Size()) {
        FOR_EACH(k, dMove.Size() - i - 1) {
            if(historyScore.GetScore(dMove.ItemRef(k)) < historyScore.GetScore(dMove.ItemRef(k + 1))) {
                std::swap(dMove.ItemRef(k), dMove.ItemRef(k + 1));
            }
        }
    }
}

int Frame::NegaMax(bool isBlack, int depth, int lower, int upper) {
#ifdef _DEBUG
    printf("%s depth = %d\n", isBlack ? "B" : "W", depth);
    ::PrintComplexBoard();
#endif
    int eval;
    DMove dm = RandomDMove(isBlack);
    if(GetTransTable(isBlack).LookUp(depth, lower, upper, eval, dm)) {
        ++GetTransTable(isBlack)._hits;
        return eval;
    } else if(IsGameOver(isBlack, eval, dm)) {
        GetTransTable(isBlack).Enter(_depth_limit, Item::TYPE_SUCCESS, eval, dm);
        return eval;
    } else if(depth <= 0) {
        return evaluator->Evaluate(isBlack);
    }
    ++_nodes;
    bool isSuccess = false;
    int maxScore = -Infinity;
    MGDMoveArray &arr = moveGenerator.Generate(isBlack, depth);
    SortByHistoryScore(arr);
    FOR_EACH(i, arr.Size()) {
        ::MakeDMove(arr.ItemRef(i));
        int score = -NegaMax(!isBlack, depth - 1, -upper, -lower);
        ::UnmakeLastDMove();
        if(score > maxScore || maxScore == -Infinity) {
            dm = arr.ItemRef(i);
            maxScore = score;
        }
        if(maxScore >= upper) {
            GetTransTable(isBlack).Enter(depth, Item::TYPE_FAIL_HIGH, maxScore, dm);
            historyScore.IncreaseScore(arr.ItemRef(i), 1 << depth);
            return maxScore;
        }
        if(maxScore > lower) {
            lower = maxScore;
            isSuccess = true;
        }
    }
    GetTransTable(isBlack).Enter(depth, isSuccess ? Item::TYPE_SUCCESS : Item::TYPE_FAIL_LOW, maxScore, dm);
    return maxScore;
}

bool Frame::FindWinningMove(bool isBlack, DMove &dm) {
    SegmentTable::Table &four = GetSegmentTable(isBlack, 4);
    SegmentTable::Table &five = GetSegmentTable(isBlack, 5);
    if(four.Size() > 0) {
        SegmentTable::Item seg = four.GetItem(0);
        FOR_EACH(i, 6) {
            FOR_EACH(k, i) {
                Point pi = seg.GetPoint(i), pk = seg.GetPoint(k);
                if(::GetCell(pi) == CellTypeEmpty && ::GetCell(pk) == CellTypeEmpty) {
                    dm = DMove(pi, pk, isBlack);
                    return true;
                }
            }
        }
        assert(false);
    } else if(five.Size() > 0) {
        dm = RandomDMove(isBlack);
        SegmentTable::Item seg = five.GetItem(0);
        FOR_EACH(i, 6) {
            Point p = seg.GetPoint(i);
            if(IsValid(dm.GetPoint1(), p, isBlack)) {
                dm = DMove(dm.GetPoint1(), p, isBlack);
                return true;
            } else if(IsValid(p, dm.GetPoint2(), isBlack)) {
                dm = DMove(p, dm.GetPoint2(), isBlack);
                return true;
            }
        }
        assert(false);
    }
    return false;
}

DMove Frame::IDMTDF(bool isBlack) {
    DMove dMove;
    int depth = 1;
    clock_t cl = clock();
    int step = 1500000;
    int eval;
    bool b;
    static int g[2] = {0, 0};
#ifdef _DEBUG
    while(depth <= _depth_limit) {
#else
    while(depth <= _depth_limit && clock() - cl < _time_limit) {
#endif
//        printf("depth limit = %d\n", depth);
        int lower = -Infinity, upper = +Infinity;
        int guess = g[depth % 2];
        while(lower + step < upper) {
//            printf("lower = %d, upper = %d, guess = %d\n", lower, upper, guess);
            int score = NegaMax(isBlack, depth, guess, guess + 1);
            b = GetTransTable(isBlack).LookUp(0, guess, guess + 1, eval, _dMove);
            assert(b);
            if(score <= guess) {
                upper = score;
                guess = upper - 1 - step;
            } else {
                lower = score;
                guess = lower + step;
            }
        }
        // binary search like
        guess = (lower + upper) / 2;
        g[depth % 2] = guess;
        int oldGuess = guess - 1;
        while(lower < upper && oldGuess != guess) {
//            printf("lower = %d, upper = %d, guess = %d\n", lower, upper, guess);
            int score = NegaMax(isBlack, depth, guess, guess + 1);
            b = GetTransTable(isBlack).LookUp(0, guess, guess + 1, eval, _dMove);
            if(score <= guess) {
                upper = score;
            } else {
                lower = score;
            }
            oldGuess = guess;
            guess = (lower + upper) / 2;
        }
        g[depth++ % 2] = guess;
        dMove = _dMove;
//        printf("%d ms\n", clock() - cl);
    }
    return dMove;
//    return g[1];
}

int Frame::_depth_limit;
int Frame::_time_limit;

Frame frame;
