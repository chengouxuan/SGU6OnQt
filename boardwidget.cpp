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

#include <QPainter>
#include <QMouseEvent>
//#include <QDebug>

BoardWidget::BoardWidget(QWidget *parent)
    : QWidget(parent), boardPainter(NULL)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setMouseTracking(true);
}

QSize BoardWidget::minimumSizeHint() const
{
    return QSize(589, 589);
}

QSize BoardWidget::sizeHint() const
{
    return QSize(589, 589);
}


void BoardWidget::setStoneData(StonePaintData *stoneData)
{
    boardPainter.setStoneData(stoneData);
}


void BoardWidget::paintEvent(QPaintEvent * /* event */)
{
    boardPainter.paint(this);
}

void BoardWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        mouseDownPoint = event->pos();
    }
}

void BoardWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && !mouseDownPoint.isNull()) {
        int dx = event->pos().x() - mouseDownPoint.x();
        int dy = event->pos().y() - mouseDownPoint.y();
        if (dx * dx + dy * dy <= 5 * 5) {
            onClicked(event->pos());
        }
        mouseDownPoint = QPoint();
    }
}

void BoardWidget::mouseMoveEvent(QMouseEvent *event) {
//    qDebug() << event->pos().x() << ", " << event->pos().y() << "\n";
    for (int i = 0; i < 19; ++i) {
        for (int j = 0; j < 19; ++j) {
            if (isInsideCell(i, j, event->pos())) {
//                qDebug() << "highlight: " << i << ", " << j << "\n";
                boardPainter.setHighlightedCell(i, j);
                repaint();
                return;
            }
        }
    }
}

void BoardWidget::onClicked(const QPoint &point)
{
    for (int i = 0; i < 19; ++i) {
        for (int j = 0; j < 19; ++j) {
            if (isInsideCell(i, j, point)) {
                emit cellClicked(i, j);
                break;
            }
        }
    }
}

bool BoardWidget::isInsideCell(int i, int j, const QPoint &point) {
    int w = width() / 19;
    int h = height() / 19;
    int left = w * j;
    int top = h * i;
    QPoint center(left + w / 2, top + h / 2);
//    qDebug() << "cell(" << i << ", " << j << ") center: " << center.x() << ", " << center.y() << "\n";
    int dx = center.x() - point.x();
    int dy = center.y() - point.y();
    if (dx * dx + dy * dy < w * h / 5) {
        return true;
    } else {
        return false;
    }
}

