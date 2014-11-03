#include "OpeningFinder.h"
#include "defines.h"

bool OpeningFinder::find(Board board, bool isBlack, DMove &dMove) {
    Board b;
    std::fill(b[0], b[RowMax], CellTypeEmpty);
    int midR = RowMax / 2;
    int midC = ColumnMax / 2;
    b[RowMax / 2][ColumnMax / 2] = CellTypeBlack;
    if(memcmp(b, board, sizeof(Board)) == 0) {
        dMove = DMove(midR, midC - 2, midR - 1, midC - 1, isBlack);
        return true;
    }
    int row = -1, col = -1;
    FOR_EACH_CELL(r, c) {
        if(board[r][c] == CellTypeBlack) {
            if(! (row == -1 && col == -1)) {
                return false;
            }
            row = r;
            col = c;
        }
        if(board[r][c] == CellTypeWhite) {
            return false;
        }
    }
    if(row == midR) {
        int sign = (midC - col) / std::abs(midC - col);
        dMove = DMove(midR, col + 1 * sign, midR, col + 2 * sign, isBlack);
    } else if(col == midC) {
        int sign = (midR - row) / std::abs(midR - row);
        dMove = DMove(row + 1 * sign, midC, row + 2 * sign, midC, isBlack);
    } else {
        int signR = (midR - row) / std::abs(midR - row);
        int signC = (midC - col) / std::abs(midC - col);
        dMove = DMove(row + 1 * signR, col + 1 * signC, row + 2 * signR, col + 2 * signC, isBlack);
    }
    return true;
}
