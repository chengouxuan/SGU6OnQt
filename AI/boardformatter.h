#ifndef BOARDFORMATTER_H
#define BOARDFORMATTER_H

#include "globaldef.h"
#include <string>

struct BoardStruct
{
    CellType board[RowMax][ColumnMax];
};

class BoardFormatter
{
public:

    static std::string boardToString(const BoardStruct &board);
    static BoardStruct stringToBoard(const std::string &string);

    static void test();

private:
    BoardFormatter();
};

#endif // BOARDFORMATTER_H
