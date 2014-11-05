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
            switch(board.board[r][c]) {
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
            case 'X': board.board[r][c++] = CellTypeBlack; break;
            case 'O': board.board[r][c++] = CellTypeWhite; break;
            case '.': board.board[r][c++] = CellTypeEmpty; break;
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


    assert(boardToString(stringToBoard(testData)).compare(testData) == 0);
}
