#include "searcherbridge.h"
#include "MoveSearcher.h"

SearcherBridge::SearcherBridge()
{
}

void SearcherBridge::invokeSearch(Board board, WhichPlayer whichPlayer, int movesToGo,
                    int &r1, int &c1, int &r2, int &c2)
{
    MoveSearcher *searcher = &gSearcher;

    searcher->SearchGoodMoves(board,
                             whichPlayer,
                             movesToGo);

    r1 = searcher->GetDMove().GetPoint1()._row;
    c1 = searcher->GetDMove().GetPoint1()._col;
    r2 = searcher->GetDMove().GetPoint2()._row;
    c2 = searcher->GetDMove().GetPoint2()._col;
}
