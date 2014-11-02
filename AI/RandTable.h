#pragma once

#include <ctime>
#include <cstdlib>
#include <cassert>
#include "defines.h"

class RandTable {
public:
    static const int _RAND_TABLES = 3;
    unsigned int _rand32[_RAND_TABLES][RowMax][ColumnMax];
    unsigned __int64 _rand64[_RAND_TABLES][RowMax][ColumnMax];

    RandTable();
    int TableNo(CellType type) {
        switch(type) {
            case CellTypeBlack: return 0;
            case CellTypeWhite: return 1;
            case CellTypeEmpty: return 2;
            default: assert(false); return 0;
        }
    }
    unsigned int Rand32(int row, int col, CellType type) {
        return _rand32[TableNo(type)][row][col];
    }
    unsigned __int64 Rand64(int row, int col, CellType type) {
        return _rand64[TableNo(type)][row][col];
    }
};

extern RandTable randTable;
