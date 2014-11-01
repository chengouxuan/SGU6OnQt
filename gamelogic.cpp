#include "gamelogic.h"
#include <vector>
#include <QDebug>

GameLogic::GameLogic()
{
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            cell[i][j] = NoStone;
        }
    }
}


GameLogic::WhichPlayer GameLogic::whichPlayersTurn()
{
    if (stack.size() == 0) {
        return Black;
    } else {
        if ((stack.size() - 1) / 2 % 2 == 0) {
            return White;
        } else {
            return Black;
        }
    }
}

GameLogic::CellType GameLogic::cellTypeAt(int i, int j) {
    if (0 <= i && i < rows && 0 <= j && j < columns) {
        return cell[i][j];
    } else {
        return NoStone;
    }
}

GameLogic::WhichPlayer GameLogic::whoWins()
{
    for (int i = 0; i < rows; ++i) {
        std::vector<CellType> line;
        for (int j = 0; j < columns; ++j) {
            line.push_back(cell[i][j]);
        }
        WhichPlayer who = checkSixStonesConnected(line);
        if (who != Unknown) {
            return who;
        }
    }

    for (int j = 0; j < columns; ++j) {
        std::vector<CellType> line;
        for (int i = 0; i < rows; ++i) {
            line.push_back(cell[i][j]);
        }
        WhichPlayer who = checkSixStonesConnected(line);
        if (who != Unknown) {
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
        if (who != Unknown) {
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
        if (who != Unknown) {
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
        if (who != Unknown) {
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
        if (who != Unknown) {
            return who;
        }
    }

    return Unknown;
}

GameLogic::WhichPlayer GameLogic::checkSixStonesConnected(std::vector<CellType> &line)
{
    if (line.size() < 6) {
        return Unknown;
    }

    int whiteCount = 0;
    int blackCount = 0;
    for (int i = 0; i < line.size(); ++i) {
//        qDebug() << (int)line[i] << ", ";
        switch (line[i]) {
        case WhiteStone:
            ++whiteCount;
            blackCount = 0;
            break;
        case BlackStone:
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
            return White;
        }
        if (6 <= blackCount) {
//            qDebug() << "black\n";
            return Black;
        }
    }

    if (columns * rows <= stack.size()) {
        return Draw;
    }

//    qDebug() << "no\n";

    return Unknown;
}

int GameLogic::goBack(int n)
{
    int count = 0;
    while (n-- > 0 && !stack.empty()) {
        int i = stack.top().i;
        int j = stack.top().j;
        cell[i][j] = NoStone;
        stack.pop();
        forwardStack.push(cell_position(i, j));
        ++count;
    }
    return count;
}


bool GameLogic::putStone(int i, int j) {

    if (whoWins() != Unknown) {
        return false;
    }

    if (i < 0 || rows <= i) {
        return false;
    }

    if (j < 0 || columns <= j) {
        return false;
    }

    if (cell[i][j] != NoStone) {
        return false;
    }

    WhichPlayer who = whichPlayersTurn();
    if (who == White) {
        cell[i][j] = WhiteStone;
    }
    if (who == Black) {
        cell[i][j] = BlackStone;
    }
    forwardStack = std::stack<cell_position>();
    stack.push(cell_position(i, j));

    return true;
}


