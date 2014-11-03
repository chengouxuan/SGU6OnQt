#include "boardformatter.h"
#include <cstring>
#include <sstream>
#include <cassert>
#include <iomanip>

std::string BoardFormatter::boardToString(const BoardStruct &board)
{
    std::stringstream sstring;
    sstring << "   A B C D E F G H I J K L M N o P Q R S";
    for (int r = 0; r < RowMax; ++r) {
        for (int c = 0; c < ColumnMax; ++c) {
            if(c == 0) {
                sstring << "\n" << std::setfill(' ') << std::setw(2) << (r + 1);
            }
            switch(board.cell[r][c]) {
                case CellTypeBlack: sstring << " X"; break;
                case CellTypeWhite: sstring << " O"; break;
                case CellTypeEmpty: sstring << " ."; break;
            }
        }
    }
    sstring << "\n";
    return sstring.str();
}


BoardStruct BoardFormatter::stringToBoard(const std::string &string)
{
    BoardStruct board;
    int r = 0, c = 0;
    for (unsigned int i = 0; i < string.size(); ++i) {
        switch(string[i]) {
            case 'X': board.cell[r][c++] = CellTypeBlack; break;
            case 'O': board.cell[r][c++] = CellTypeWhite; break;
            case '.': board.cell[r][c++] = CellTypeEmpty; break;
        }
        if (c >= ColumnMax) {
            c = 0;
            if (++r >= RowMax) {
                break;
            }
        }
    }
    return board;
}

void BoardFormatter::test()
{
    static char *testData = {
        "   A B C D E F G H I J K L M N o P Q R S" "\n"
        " 1 O . . . . . . . . . . . . . . . . . ." "\n"
        " 2 . . . . . . . . . . . . . . . . . . ." "\n"
        " 3 . . . . . . . . . . . . . . . . . . ." "\n"
        " 4 . . . . . . . . . . . . . . . . . . ." "\n"
        " 5 . . . . . . . . . . . . . . . . . . ." "\n"
        " 6 . . . . . . . . . O . . . . . . . . ." "\n"
        " 7 . . . . . X . O X X . . . . . . . . ." "\n"
        " 8 . . . . . . O . O X . . . . . . . . O" "\n"
        " 9 . . . . . . . O . X . . . . . . . . ." "\n"
        "10 . . . . . . . . O . . . . . . . . . ." "\n"
        "11 X . . . . . . . . O . . . . . . . . ." "\n"
        "12 . . . . . . . . . . . . . . . . . . ." "\n"
        "13 . . . . . . . . . . . X . . . . . . ." "\n"
        "14 . . . . . . . . . . . . . . . . . . ." "\n"
        "15 . . . . . . . . . . . . . . . . . . ." "\n"
        "16 . . . . . . . . . . . . . . . . . . ." "\n"
        "17 . . . . . . . . . . . . . . . . . . ." "\n"
        "18 . . . . . . . . . . . . . . . . . . ." "\n"
        "19 X . . . . . . . . . . . . . . . . . ." "\n"
    };

    assert(boardToString(stringToBoard(testData)).compare(testData) == 0);
}
