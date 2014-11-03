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

#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>
#include "boardpainter.h"
#include <cstring>
#include "gamelogic.h"

class BoardWidget : public QWidget, public StonePaintData
{
    Q_OBJECT

public:
    enum Shape { Line, Points, Polyline, Polygon, Rect, RoundedRect, Ellipse, Arc,
                 Chord, Pie, Path, Text, Pixmap };

    BoardWidget(QWidget *parent = 0);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    struct StoneHighlightedStruct {
        int highlightedPosition1Row;
        int highlightedPosition1Column;
        int highlightedPosition2Row;
        int highlightedPosition2Column;
        StoneHighlightedStruct()
            : highlightedPosition1Row(-1)
            , highlightedPosition1Column(-1)
            , highlightedPosition2Row(-1)
            , highlightedPosition2Column(-1) {}
        StoneHighlightedStruct(const StoneHighlightedStruct &s) {
            memcpy(this, &s, sizeof(s));
        }
    };

    StonePaintType stonePaintTypeAt(int row, int column);

    StoneHighlightedStruct stoneHighlightedStruct() const;
    void setStoneHighlightedStruct(const StoneHighlightedStruct &stoneHighlightedStruct);

    void setGameLogic(GameLogic *value);

Q_SIGNALS:
    void cellClicked(int i, int j);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    BoardPainter boardPainter;
    QPoint mouseDownPoint;

    GameLogic *gameLogic;

    StoneHighlightedStruct _stoneHighlightedStruct;

    void onClicked(const QPoint &point);
    bool isInsideCell(int i, int j, const QPoint &point);
};

#endif // RENDERAREA_H
