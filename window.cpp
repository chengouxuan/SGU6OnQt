/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "boardwidget.h"
#include "window.h"

#include <QtWidgets>
#include <QMessageBox>

Window::Window()
    : boardWidget(NULL)
    , whiteUseAI(true)
    , blackUseAI(false)
    , aiWhite("qyerlouk-white")
    , aiBlack("qyerlouk-black")
{
    boardWidget = new BoardWidget;
    boardWidget->setGameLogic(&gameLogic);

    connect(boardWidget, SIGNAL(cellClicked(int, int)),
            this, SLOT(boardWidgetCellClicked(int,int)));

    QGridLayout *mainLayout = new QGridLayout;

    QLabel *columnLabels = new QLabel[19];
    QLabel *rowLabels = new QLabel[19];

    const char *(columnMarks[19]) = {
        "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S"
    };

    const char *(rowMarks[19]) = {
        "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19"
    };

    for (int i = 0; i < 19; ++i) {
        columnLabels[i].setText(columnMarks[i]);
        mainLayout->addWidget(&(columnLabels[i]), 0, i + 1, 1, 1, Qt::AlignCenter);
    }

    for (int i = 0; i < 19; ++i) {
        rowLabels[i].setText(rowMarks[i]);
        mainLayout->addWidget(&(rowLabels[i]), 1 + i, 0, 1, 1, Qt::AlignCenter);
    }

    mainLayout->addWidget(boardWidget, 1, 1, 19, 19);
    setLayout(mainLayout);

    setWindowTitle(tr("SGU6"));
}



void Window::onStonePlaced(WhichPlayer oldPlayer, int row, int col)
{
    WhichPlayer newPlayer = gameLogic.whichPlayersTurn();

    BoardWidget::StoneHighlightedStruct s = boardWidget->stoneHighlightedStruct();

    s.highlightedPosition1Row = s.highlightedPosition2Row;
    s.highlightedPosition1Column = s.highlightedPosition2Column;
    s.highlightedPosition2Row = row;
    s.highlightedPosition2Column = col;

    if (newPlayer == oldPlayer) {
        s.highlightedPosition1Row = -1;
        s.highlightedPosition1Column = -1;
    }

    boardWidget->setStoneHighlightedStruct(s);
    boardWidget->update();

    WhichPlayer whoWins = gameLogic.whoWins();
    QMessageBox messageBox;
    if (whoWins == WhitePlayer) {
        messageBox.setText(tr("White Wins"));
        messageBox.exec();
    } else if (whoWins == BlackPlayer) {
        messageBox.setText(tr("Black Wins"));
        messageBox.exec();
    } else if (whoWins == ResultDraw) {
        messageBox.setText(tr("Draw"));
        messageBox.exec();
    }

    if (whoWins == UnknownPlayer && oldPlayer != newPlayer) {
        if (newPlayer == WhitePlayer && whiteUseAI) {
            whiteRequestThinking();
        } else if (newPlayer == BlackPlayer && blackUseAI) {
            blackRequestThinking();
        }
    }
}

void Window::whiteRequestThinking()
{
    if (aiWhite.requestThinking(makeBoardDataStruct())) {
        QTimer::singleShot(500, this, SLOT(whiteGetThinkingResult()));
    } else {
        QTimer::singleShot(300, this, SLOT(whiteRequestThinking()));
    }
}

void Window::blackRequestThinking()
{
    if (aiBlack.requestThinking(makeBoardDataStruct())) {
        QTimer::singleShot(500, this, SLOT(blackGetThinkingResult()));
    } else {
        QTimer::singleShot(300, this, SLOT(blackRequestThinking()));
    }
}

void Window::whiteGetThinkingResult()
{
    int r1 = -1, c1 = -1, r2 = -1, c2 = -1;
    if (!aiWhite.getThinkingResult(r1, c1, r2, c2)) {
        QTimer::singleShot(500, this, SLOT(whiteGetThinkingResult()));
    } else {
        putStone(r1, c1);
        putStone(r2, c2);
    }
}

void Window::blackGetThinkingResult()
{
    int r1 = -1, c1 = -1, r2 = -1, c2 = -1;
    if (!aiBlack.getThinkingResult(r1, c1, r2, c2)) {
        QTimer::singleShot(500, this, SLOT(blackGetThinkingResult()));
    } else {
        putStone(r1, c1);
        putStone(r2, c2);
    }
}

AIController::BoardDataStruct Window::makeBoardDataStruct()
{
    AIController::BoardDataStruct st;
    for (int i = 0; i < RowMax; ++i) {
        for (int k = 0; k < ColumnMax; ++k) {
            st.board[i][k] = gameLogic.cellTypeAt(i, k);
        }
    }
    st.movesToGo = gameLogic.stonesToDo();
    st.whichPlayersTurn = gameLogic.whichPlayersTurn();
    return st;
}


bool Window::putStone(int row, int col)
{
    WhichPlayer player = gameLogic.whichPlayersTurn();
    if (0 <= row && row < RowMax && 0 <= col && col < ColumnMax) {
        if (gameLogic.putStone(row, col)) {
            onStonePlaced(player, row, col);
        }
    }
}

void Window::boardWidgetCellClicked(int row, int col)
{
    WhichPlayer player = gameLogic.whichPlayersTurn();

    if ((player == WhitePlayer && whiteUseAI) ||
            (player == BlackPlayer && blackUseAI)) {
        return;
    }

    putStone(row, col);
}
