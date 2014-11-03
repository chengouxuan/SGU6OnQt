#pragma once

/*
 * �ļ��������������� Frame, ��ȫ��ʵ�� frame
 * Frame ͨ��ȫ�ֺ������� complexBoard, segmentTable, moveGenerator
 *
 *                                                           --2011.08.21
 *
 */

#include "TranspositionTable.h"
#include "MoveSearcher.h"
#include "DTSSer.h"
#include "HistoryScore.h"
#include "MoveGenerator.h"
#include "RZDeferder.h"

class Frame {
public:
    static /*const*/ int _depth_limit/* = 3*/;        // ������������
    static const int _DTSS_SCORE = Infinity;  // ˫��в�����ɹ�����
    static /*const*/ int _time_limit/* = 10000*/;     // ʱ������, ��δ���������ƺ��������� _DEPTH_LIMIT ʱ������һ������

    DMove _dMove; // �����ŷ�
    int _nodes;   // ����ͳ��

    class Item { // �û�����
    public:
        enum Type {
            TYPE_FAIL_HIGH,
            TYPE_FAIL_LOW,
            TYPE_SUCCESS,
            TYPE_NOT_A_ITEM
        } _type;
        int _eval;
        int _depth;
        unsigned __int64 _checkSum;
        DMove _dMove;

        Item(Type type, int eval, int depth, __int64 checkSum, const DMove &dm)
            : _type(type)
            , _eval(eval)
            , _depth(depth)
            , _checkSum(checkSum)
            , _dMove(dm) {}
        Item()
            : _type(TYPE_NOT_A_ITEM)
            , _eval(0)
            , _depth(0)
            , _checkSum(0)
            , _dMove(DMove()) {}
        Item(const Item &ti) {
            memcpy(this, &ti, sizeof(*this));
        }
    };

    typedef TranspositionTable <Item, 1048583> TransTableBase;
    class TransTable: public TransTableBase { // �û���
    public:
        void Enter(int depth, Item::Type type, int eval, const DMove &dm) {
            TransTableBase::Enter(Item(type, eval, depth, ::CheckSum(), dm), ::Hash());
        }
        bool LookUp(int depth, int lower, int upper, int &eval, DMove &dm) {
            Item item;
            if(!TransTableBase::LookUp(::CheckSum(), ::Hash(), item) || item._type == Item::TYPE_NOT_A_ITEM || item._depth < depth) {
                return false;
            }
            eval = item._eval;
            dm = item._dMove;
            switch(item._type) {
                case Item::TYPE_SUCCESS: return true;
                case Item::TYPE_FAIL_LOW: return lower >= item._eval;
                case Item::TYPE_FAIL_HIGH: return upper <= item._eval;
                default: assert(item._type == Item::TYPE_NOT_A_ITEM); return false;
            }
        }
    } _transTableBlack, _transTableWinte;

    TransTable &GetTransTable(bool isBlack) {
        return isBlack ? _transTableBlack : _transTableWinte;
    }

private:
    //************************************
    // Method:    NegaMax
    // FullName:  Frame::NegaMax
    // Access:    private 
    // Returns:   int
    // Qualifier: ������ֵ
    // Parameter: bool isBlack �ڽ���Ϊ true, �׽���Ϊ false
    // Parameter: int depth ��������
    // Parameter: int lower alpha ֵ
    // Parameter: int upper beta ֵ
    //************************************
    int NegaMax(bool isBlack, int depth, int lower, int upper);

    //int MTDF(bool isBlack, int guess);

    //************************************
    // Method:    IDMTDF
    // FullName:  Frame::IDMTDF
    // Access:    private 
    // Returns:   int
    // Qualifier: �������� MTD(f)
    // Parameter: bool isBlack �ڷ�Ϊ true, �׷�Ϊ false
    //************************************
    DMove IDMTDF(bool isBlack);

    //int MiniWind(bool isBlack, int depth, int lower, int upper);

    //************************************
    // Method:    FindWinningMove
    // FullName:  Frame::FindWinningMove
    // Access:    private 
    // Returns:   bool true �ҵ�, false �Ҳ���
    // Qualifier: �γ��������ŷ�
    // Parameter: bool isBlack �ڷ�Ϊ true, �׷�Ϊ false
    // Parameter: DMove & dm �ŷ�
    //************************************
    bool FindWinningMove(bool isBlack, DMove &dm);

    //************************************
    // Method:    IsGameOver
    // FullName:  Frame::IsGameOver
    // Access:    private 
    // Returns:   bool
    // Qualifier: �Ƿ���������Ϸ�Ѿ�����
    // Parameter: bool isBlack �ڷ��غ�Ϊ true, �׷��غ�Ϊ false
    // Parameter: int & eval ����Ϸ������ֵΪ����ֵ
    // Parameter: DMove & dm ����Ϸ�������ŷ�Ϊ�ýڵ��������ŷ�
    //************************************
    bool IsGameOver(bool isBlack, int &eval, DMove &dm) {
        if(FindWinningMove(isBlack, dm)) {
            eval = +Infinity;
            return true;
        } else if(dtsser.Search(isBlack, true)) {
            eval = _DTSS_SCORE;
            dm = dtsser._dMove;
            return true;
        } else {
            return false;
        }
    }

public:
    //************************************
    // Method:    ResetCounters
    // FullName:  Frame::ResetCounters
    // Access:    public 
    // Returns:   void
    // Qualifier: ͳ������
    //************************************
    void ResetCounters() {
        _nodes = 0;
        _transTableBlack._hits = 0;
        _transTableWinte._hits = 0;
    }

private:
    void NewSearchInit(Board board) {
        complexBoard.NewSearchInit(board);
        historyScore.Zero();
        evaluator->ResetCounters();
        dtsser.ResetCounters();
        this->ResetCounters();
    }

    //************************************
    // Method:    SortByHistoryScore
    // FullName:  Frame::SortByHistoryScore
    // Access:    private static 
    // Returns:   void
    // Qualifier: �ŷ�����
    // Parameter: MGDMoveArray & ���������ŷ�
    //************************************
    static void SortByHistoryScore(MGDMoveArray &);

    //int MiniWind(bool isBlack) {
    //    MiniWind(isBlack, _DEPTH_LIMIT, -INFINITY, +INFINITY);
    //    int eval;
    //    bool b = GetTransTable(isBlack).LookUp(0, -INFINITY, +INFINITY, eval, _dMove);
    //    assert(b);
    //    return eval;
    //}

    //int Aspiration(bool isBlack, int guess, int wind) {
    //    int scr = MiniWind(isBlack, _DEPTH_LIMIT, guess - wind, guess + wind);
    //    if(guess - wind < scr && scr < guess + wind) {
    //        printf("aspiration success\n");
    //        GetTransTable(isBlack).LookUp(0, guess - wind, guess + wind, scr, _dMove);
    //    } else if(scr <= guess - wind) {
    //        printf("aspiration fail low\n");
    //        MiniWind(isBlack, _DEPTH_LIMIT, -INFINITY, guess - wind);
    //        GetTransTable(isBlack).LookUp(0, -INFINITY, guess - wind, scr, _dMove);
    //    } else {
    //        printf("aspiration fail high\n");
    //        MiniWind(isBlack, _DEPTH_LIMIT, guess + wind, +INFINITY);
    //        GetTransTable(isBlack).LookUp(0, guess + wind, +INFINITY, scr, _dMove);
    //    }
    //    return scr;
    //}

public:
    //************************************
    // Method:    Search
    // FullName:  Frame::Search
    // Access:    public 
    // Returns:   void
    // Qualifier: ����, �ⲿ���õ��ô˺�����������
    // Parameter: Board board ��������
    // Parameter: bool isBlack �ڷ��غ�Ϊ true, �׷��غ�Ϊ false
    //************************************
    DMove Search(Board board, bool isBlack) {
        NewSearchInit(board);
        return IDMTDF(isBlack);
    }
};

extern Frame frame;
