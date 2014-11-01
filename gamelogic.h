#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <stack>
#include <vector>

class GameLogic
{
public:
    GameLogic();

    enum WhichPlayer {
        White, Black, Unknown, Draw
    };

    WhichPlayer whoWins();
    int goBack(int n);
    bool putStone(int i, int j);
    WhichPlayer whichPlayersTurn();

private:
    enum CellType {
        WhiteStone, BlackStone, NoStone
    };

    const static int rows = 19;
    const static int columns = 19;

    CellType cell[rows][columns];
    WhichPlayer checkSixStonesConnected(std::vector<CellType> &line);

    struct cell_position {
        int i;
        int j;
        cell_position(): i(0), j(0) {}
        cell_position(int ii, int jj): i(ii), j(jj) {}
    };

    std::stack<cell_position> stack;
    std::stack<cell_position> forwardStack;
};

#endif // GAMELOGIC_H
