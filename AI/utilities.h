#pragma once

#include "defines.h"
#include <cstring>
#include "UtilityTypes.h"

inline bool IsInsideBoard(int row, int col) {
    return IS_INSIDE_BOARD(row, col);
}

class Counter {
public:
    int _cnt[RowMax][ColumnMax];
    Counter() {
        memset(_cnt, 0, sizeof(_cnt));
    }
    void Zero() {
        memset(_cnt, 0, sizeof(_cnt));
    }
    int &Cnt(int row, int col) {
        return _cnt[row][col];
    }
    int &Cnt(const Point &p) {
        return Cnt(p._row, p._col);
    }
};

