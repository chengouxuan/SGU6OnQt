#include "utilities.h"
#include "defines.h"
#include <cstdio>
#include <cassert>

bool IsGameOver(Board board) {
    static int dr[] = {0, -1, -1, 1};
    static int dc[] = {-1, 0, -1, -1};
    FOR_EACH_CELL(row, col) {
        if(board[row][col] != CellTypeBlack && board[row][col] != CellTypeWhite) {
            continue;
        }
        FOR_EACH(i, 4) {
            if(! IS_INSIDE_BOARD(row + 5 * dr[i], col + 5 * dc[i])) {
                continue;
            }
            int blacks = 0, whites = 0;
            FOR_EACH(d, 6) {
                int r = row + d * dr[i];
                int c = col + d * dc[i];
                if(board[r][c] == CellTypeBlack) {
                    ++blacks;
                } else if(board[r][c] == CellTypeWhite) {
                    ++whites;
                } else {
                    break;
                }
                if(blacks > 0 && whites > 0) {
                    break;
                }
            }
            if(blacks >= 6 || whites >= 6) {
                return true;
            }
        }
    }
    return false;
}

void PrintBoard(Board board) {
    printf("  ");
    FOR_EACH_COL(i) {
        printf("%2c", i + 'A');
    }
    FOR_EACH_CELL(r, c) {
        if(c == 0) {
            printf("\n%02d", r);
        }
        switch(board[r][c]) {
            case CellTypeBlack: printf(" X"); break;
            case CellTypeWhite: printf(" O"); break;
            case CellTypeEmpty: printf(" ."); break;
#ifdef _ASSERT_SWITCHES
            default: assert(false);
#endif;
        }
    }
    printf("\n\n");
}
