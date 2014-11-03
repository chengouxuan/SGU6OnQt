#ifndef SEARCHERBRIDGE_H
#define SEARCHERBRIDGE_H

#include "globaldef.h"

class SearcherBridge
{
public:
    SearcherBridge();

    static void invokeSearch(Board board, WhichPlayer whichPlayer, int movesToGo,
                             int alphdBetaDepth, int timeLimitationMS, int dtssDepth, int idDtssDepth,
                             int &r1, int &c1, int &r2, int &c2);
public:
    static void test();
};

#endif // SEARCHERBRIDGE_H
