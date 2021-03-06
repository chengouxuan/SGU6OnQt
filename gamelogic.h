#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <stack>
#include <vector>
#include "globaldef.h"

class GameLogic
{
public:
    GameLogic();

    WhichPlayer whoWins();
    int goBack(int n);
    bool putStone(int i, int j);
    WhichPlayer whichPlayersTurn();
    int stonesToDo();


    CellType cellTypeAt(int i, int j);

private:

    const static int rows = 19;
    const static int columns = 19;

    CellType cell[rows][columns];
    WhichPlayer checkSixStonesConnected(std::vector<CellType> &line);

    struct CellPositionStruct {
        int i;
        int j;
        CellPositionStruct(): i(0), j(0) {}
        CellPositionStruct(int ii, int jj): i(ii), j(jj) {}
    };

    std::stack<CellPositionStruct> stack;
    std::stack<CellPositionStruct> forwardStack;
};

#endif // GAMELOGIC_H
