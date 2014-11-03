#ifndef SEARCHERBRIDGE_H
#define SEARCHERBRIDGE_H

#include "globaldef.h"

class SearcherBridge
{
public:
    SearcherBridge();

    static void invokeSearch(Board board, WhichPlayer whichPlayer, int movesToGo,
                             int &r1, int &c1, int &r2, int &c2);
};

#endif // SEARCHERBRIDGE_H
