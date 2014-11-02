#pragma once

enum
{
    RowMax = 19,
    ColumnMax = 19,
    Infinity = 1000000000
};

enum CellType
{
    CellTypeBlack,
    CellTypeWhite,
    CellTypeEmpty
};

#define FOR_EACH(i, sz) for(int i=0, SZ=(sz); i<SZ; ++i)
#define FOR_EACH_ROW(i) FOR_EACH(i, RowMax)
#define FOR_EACH_COL(i) FOR_EACH(i, ColumnMax)
#define FOR_EACH_CELL(r, c) FOR_EACH_ROW(r)FOR_EACH_COL(c)
#define FOR_EACH_SEG(r, c, d) FOR_EACH_CELL(r, c)FOR_EACH(d, 4)
#define IS_INSIDE_BOARD(r, c) (0 <= (r) && (r) < RowMax && 0 <= (c) && (c) < ColumnMax)

const int dr[4] = {1, 0, 1, 1};
const int dc[4] = {0, 1, 1, -1};

