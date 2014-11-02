#pragma once

#include <cassert>
#include "Array.h"


typedef Array <Move, 19 * 19, MoveEqual> MoveArrayBase;
class MoveArray: public MoveArrayBase {
public:
    void PushBack(int row, int col, bool isBlack) {
        MoveArrayBase::PushBack(Move(row, col, isBlack));
    }
    void PushBack(const Point &p, bool isBlack) {
        PushBack(p._row, p._col, isBlack);
    }
    void PushBack(const Move &move) {
        PushBack(move._row, move._col, move._isBlack);
    }
    bool Find(int row, int col, bool isBlack) {
        return MoveArrayBase::Find(Move(row, col, isBlack));
    }
    bool Find(const Move &m) {
        return Find(m._row, m._col, m._isBlack);
    }
    bool Find(const Point &p, bool isBlack) {
        return Find(p._row, p._col, isBlack);
    }
};
