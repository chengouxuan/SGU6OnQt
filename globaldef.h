#ifndef GLOBALDEF_H
#define GLOBALDEF_H

enum
{
    RowMax = 19,
    ColumnMax = 19,
};

enum CellType
{
    CellTypeBlack,
    CellTypeWhite,
    CellTypeEmpty
};

enum WhichPlayer
{
    WhitePlayer,
    BlackPlayer,
    UnknownPlayer,
    ResultDraw
};

typedef CellType Board[RowMax][ColumnMax];

#endif // GLOBALDEF_H
