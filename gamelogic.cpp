#include "gamelogic.h"
#include <vector>
//#include <QDebug>

GameLogic::GameLogic()
{
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            cell[i][j] = CellTypeEmpty;
        }
    }
}


WhichPlayer GameLogic::whichPlayersTurn()
{
    if (stack.size() == 0) {
        return BlackPlayer;
    } else {
        if ((stack.size() - 1) / 2 % 2 == 0) {
            return WhitePlayer;
        } else {
            return BlackPlayer;
        }
    }
}

int GameLogic::stonesToDo()
{
    if (stack.size() == 0) {
        return 1;
    } else {
        if ((stack.size() - 1) % 2 == 0) {
            return 2;
        } else {
            return 1;
        }
    }
}

CellType GameLogic::cellTypeAt(int i, int j) {
    if (0 <= i && i < rows && 0 <= j && j < columns) {
        return cell[i][j];
    } else {
        return CellTypeEmpty;
    }
}

WhichPlayer GameLogic::whoWins()
{
    for (int i = 0; i < rows; ++i) {
        std::vector<CellType> line;
        for (int j = 0; j < columns; ++j) {
            line.push_back(cell[i][j]);
        }
        WhichPlayer who = checkSixStonesConnected(line);
        if (who != UnknownPlayer) {
            return who;
        }
    }

    for (int j = 0; j < columns; ++j) {
        std::vector<CellType> line;
        for (int i = 0; i < rows; ++i) {
            line.push_back(cell[i][j]);
        }
        WhichPlayer who = checkSixStonesConnected(line);
        if (who != UnknownPlayer) {
            return who;
        }
    }

    for (int i = 0; i < rows; ++i) {
        std::vector<CellType> line;
        int j = 0;
        while(i + j < rows && j < columns) {
            line.push_back(cell[i + j][j]);
            ++j;
        }
        WhichPlayer who = checkSixStonesConnected(line);
        if (who != UnknownPlayer) {
            return who;
        }
    }

    for (int i = 0; i < rows; ++i) {
        std::vector<CellType> line;
        int j = 0;
        while(0 <= i - j && j < columns) {
            line.push_back(cell[i - j][j]);
            ++j;
        }
        WhichPlayer who = checkSixStonesConnected(line);
        if (who != UnknownPlayer) {
            return who;
        }
    }

    for (int i = 0; i < rows; ++i) {
        std::vector<CellType> line;
        int j = 0;
        while(0 <= columns - 1 - j && i + j < rows) {
            line.push_back(cell[i + j][columns - 1 - j]);
            ++j;
        }
        WhichPlayer who = checkSixStonesConnected(line);
        if (who != UnknownPlayer) {
            return who;
        }
    }

    for (int i = 0; i < rows; ++i) {
        std::vector<CellType> line;
        int j = 0;
        while(0 <= columns - 1 - j && 0 <= i - j) {
            line.push_back(cell[i - j][columns - 1 - j]);
            ++j;
        }
        WhichPlayer who = checkSixStonesConnected(line);
        if (who != UnknownPlayer) {
            return who;
        }
    }

    return UnknownPlayer;
}

WhichPlayer GameLogic::checkSixStonesConnected(std::vector<CellType> &line)
{
    if (line.size() < 6) {
        return UnknownPlayer;
    }

    int whiteCount = 0;
    int blackCount = 0;
    for (unsigned int i = 0; i < line.size(); ++i) {
//        qDebug() << (int)line[i] << ", ";
        switch (line[i]) {
        case CellTypeWhite:
            ++whiteCount;
            blackCount = 0;
            break;
        case CellTypeBlack:
            ++blackCount;
            whiteCount = 0;
            break;
        default:
            whiteCount = 0;
            blackCount = 0;
            break;
        }
        if (6 <= whiteCount) {
//            qDebug() << "white\n";
            return WhitePlayer;
        }
        if (6 <= blackCount) {
//            qDebug() << "black\n";
            return BlackPlayer;
        }
    }

    if (columns * rows <= stack.size()) {
        return ResultDraw;
    }

//    qDebug() << "no\n";

    return UnknownPlayer;
}

int GameLogic::goBack(int n)
{
    int count = 0;
    while (n-- > 0 && !stack.empty()) {
        int i = stack.top().i;
        int j = stack.top().j;
        cell[i][j] = CellTypeEmpty;
        stack.pop();
        forwardStack.push(CellPositionStruct(i, j));
        ++count;
    }
    return count;
}


bool GameLogic::putStone(int i, int j) {

    if (whoWins() != UnknownPlayer) {
        return false;
    }

    if (i < 0 || rows <= i) {
        return false;
    }

    if (j < 0 || columns <= j) {
        return false;
    }

    if (cell[i][j] != CellTypeEmpty) {
        return false;
    }

    WhichPlayer who = whichPlayersTurn();
    if (who == WhitePlayer) {
        cell[i][j] = CellTypeWhite;
    }
    if (who == BlackPlayer) {
        cell[i][j] = CellTypeBlack;
    }
    forwardStack = std::stack<CellPositionStruct>();
    stack.push(CellPositionStruct(i, j));

    return true;
}


