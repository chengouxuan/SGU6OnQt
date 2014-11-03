#include "evaluator.h"
#include <cassert>
#include "utilities.h"
#include "defines.h"
#include "movesearcher.h"
#include <sstream>

Evaluator::Evaluator()
{
    char *shapes = {
        "111111      100000000       0 ʤ\n"
        "0111110     999983          1 ����\n"
        "010111010   999983          2 ����\n"
        "100111010   999983          3 ����\n"
        "100111001   999983          4 ����\n"
        "00111100    999983          5 ����\n"
        "111110      94427          6 ����\n"
        "111101      94427          7 ����\n"
        "111011      94427          8 ����\n"
        "1111001     94427          9 ����\n"
        "1110011     94427          10 ����\n"
        "1110101     94427          11 ����\n"
        "1101101     94427          12 ����\n"
        "1011101     94427          13 ����\n"
        "1101011     94427          14 ����\n"
        "111100      94427          15 ����\n"
        "111001      94427          16 ����\n"
        "110011      94427          17 ����\n"
        "110101      94427          18 ����\n"
        "101101      94427          19 ����\n"
        "00111000    54667          20 ����\n"
        "00110100    54667          21 ����\n"
        "010110000   9539          22 ������\n"
        "100110000   9539          23 ������\n"
        "010011000   9539          24 ������\n"
        "010101000   9539          25 ������\n"
        "111000      4663          26 ����\n"
        "011100      4663          27 ����\n"
        "110100      4663          28 ����\n"
        "110010      4663          29 ����\n"
        "011010      4663          30 ����\n"
        "110001      4663          31 ����\n"
        "011001      4663          32 ����\n"
        "001101      4663          33 ����\n"
        "101010      4663          34 ����\n"
        "101001      4663          35 ����\n"
        "110000      997           36 �߶�\n"
        "011000      997           37 �߶�\n"
        "001100      997           38 �߶�\n"
        "101000      997           39 �߶�\n"
        "100100      997            40 �߶�\n"
        "010100      997            41 �߶�\n"
        "100010      997            42 �߶�\n"
        "010010      997            43 �߶�\n"
        "100001      997            44 �߶�\n"
        "00110000    4327\n"
        "00101000    4327\n"
        "00100100    4327\n"
        "00011000    4327\n"
    };

    char pat[1024], line[1024];
    int scr;

    memset(_score, 0, sizeof(_score));

    printf("\nscore table:\n");
    int i = 0;

    char *lineBegin = shapes;
    char *lineEnd = NULL;

    while((lineEnd = strchr(lineBegin, '\n')) && *lineEnd == '\n') {

        memset(line, 0, sizeof(line));
        strncpy(line, lineBegin, lineEnd - lineBegin);

        lineBegin = lineEnd + 1;

        if(sscanf(line, "%s%d", pat, &scr) == 2) {
            size_t len = strlen(pat);
            printf("%2d: score[ %9s ] = %9d\n", i++, pat, scr);
            assert(_SHAPE_LEN_MIN <= len && len <= _SHAPE_LEN_MAX);
            const char *p = pat - 1;
            int mask = 0;
            while(*++p) {
                mask <<= 1;
                if(*p == '1') {
                    mask |= 1;
                }
            }
            assert(_score[len][mask] == 0);
            _score[len][mask] = scr;
            mask = 0;
            while(--p >= pat) {
                mask <<= 1;
                if(*p == '1') {
                    mask |= 1;
                }
            }
            assert(_score[len][mask] == 0 || _score[len][mask] == scr);
            _score[len][mask] += scr;
        }
    }
}

int Evaluator::Evaluate(bool isBlacksTurn) {
    int sign = (isBlacksTurn ? 1 : -1);
    int score;
    if(_transTable.LookUp(score)) {
        ++_transTable._hits;
        return score * sign;
    }
    ++_evaluations;
    score = SimpleEval();
    int sz = 0;
    CellType line[_LINE_LEN_MAX];
    FOR_EACH_ROW(row) {
        FOR_EACH_COL(col) {
            line[col] = ::GetCell(row, col);
        }
        score += LineScore(line, ColumnMax);
    }
    FOR_EACH_COL(col) {
        FOR_EACH_ROW(row) {
            line[row] = ::GetCell(row, col);
        }
        score += LineScore(line, RowMax);
    }
    // �Խ���
    FOR_EACH_ROW(row) {
        sz = 0;
        int i = 0;
        while(++i, IsInsideBoard(row + i, 0 + i)) {
            line[sz++] = ::GetCell(row + i, 0 + i);
        }
        score += LineScore(line, sz);
    }
    FOR_EACH_COL(col) {
        sz = 0;
        if(col == 0) {
            continue;
        }
        int i = 0;
        while(++i, IsInsideBoard(0 + i, col + i)) {
            line[sz++] = ::GetCell(0 + i, col + i);
        }
        score += LineScore(line, sz);
    }
    // ���Խ���
    FOR_EACH_ROW(row) {
        sz = 0;
        int i = 0;
        while(++i, IsInsideBoard(row + i, 18 - i)) {
            line[sz++] = ::GetCell(row + i, 18 - i);
        }
        score += LineScore(line, sz);
    }
    FOR_EACH_COL(col) {
        sz = 0;
        if(col == 18) {
            continue;
        }
        int i = 0;
        while(++i, IsInsideBoard(0 + i, col - i)) {
            line[sz++] = ::GetCell(0 + i, col - i);
        }
        score += LineScore(line, sz);
    }
    //static int cellScore[6] = {0, 343, 45630, 55505, 30333, 40444};
    //FOR_EACH(i, 6) {
    //    SegmentTable::Table &tab = ::GetSegmentTable(true, n);
    //    FOR_EACH(k, tab.Size()) {
    //        FOR_EACH(m, 6) {
    //            Point p = tab.GetItem(k).GetPoint(m);
    //            if(::GetCell(p) == CELL_TYPE_BLACK) {
    //                score += cellScore[i];
    //            } else if(::GetCell(p) == CELL_TYPE_WHITE) {
    //                score -= cellScore[i];
    //            }
    //        }
    //    }
    //}
#ifdef _DEBUG
    //::PrintComplexBoard();
    //printf("score = %d\n", score * (isBlacksTurn ? 1 : -1));
#endif
    //assert(score == oldEvaluator.Evaluate(true));
    //score /= 10;
    _transTable.Enter(score);
    return score * sign;
}

Evaluator *evaluator = NULL;

int Evaluator::LineScore(CellType line[_LINE_LEN_MAX], int len) {
    int scr = 0;
#ifdef _DEBUG
    //FOR_EACH(i, len) {
    //    printf(" %s", line[i] == CELL_TYPE_EMPTY ? "." : (line[i] == CELL_TYPE_BLACK ? "X" : "O"));
    //}
    //printf("\n");
#endif
    for(int s=_SHAPE_LEN_MIN; s<=_SHAPE_LEN_MAX; ++s) {
        unsigned int maskB = 0, maskW = 0;
        int lastB = -1, lastW = -1;
        FOR_EACH(i, len) {
            maskB <<= 1;
            maskW <<= 1;
            if(line[i] == CellTypeBlack) {
                maskB |= 1;
                lastB = i;
            } else if(line[i] == CellTypeWhite) {
                maskW |= 1;
                lastW = i;
            }
            if(s <= i + 1) {
                if(i - lastW >= s) {
                    scr += _score[s][maskB & ~(~0 << s)];
                }
                if(i - lastB >= s) {
                    scr -= _score[s][maskW & ~(~0 << s)];
                }
            }
        }
    }
    return scr;
}

int Evaluator::SimpleEval() {
    int ret = 0;
    FOR_EACH(i, 6) {
        ret += ::GetSegmentTable(true, i).Size() * _segmentScore[i]/* * i*/;
        ret -= ::GetSegmentTable(false, i).Size() * _segmentScore[i]/* * i*/;
    }
    return ret;
}

const int Evaluator::_segmentScore[6] = {2, 139,  10321,  65651, 100621, 741071};
