#include "boardpainter.h"
#include <QPainter>
#include <cmath>

#define CELL_POSITION_SPEC_LEFT 1
#define CELL_POSITION_SPEC_RIGHT 2
#define CELL_POSITION_SPEC_TOP 4
#define CELL_POSITION_SPEC_BOTTOM 8

BoardPainter::BoardPainter(): highlightedCellRow(0), highlightedCellColumn(0)
{
}

void BoardPainter::paint(QWidget *widget, StonePaintData *stoneData)
{
    int cellWidth = widget->width() / numOfColumns;
    int x = 0;

    int cellHeight = widget->height() / numOfRows;
    int y = 0;
    int resetY = y;

    for (int col = 0; col < numOfColumns; ++col) {
        for (int row = 0; row < numOfRows; ++row) {
            int spec = 0;
            if (col == 0) {
                spec |= CELL_POSITION_SPEC_LEFT;
            }
            if (col == numOfColumns - 1) {
                spec |= CELL_POSITION_SPEC_RIGHT;
            }
            if (row == 0) {
                spec |= CELL_POSITION_SPEC_TOP;
            }
            if (row == numOfRows - 1) {
                spec |= CELL_POSITION_SPEC_BOTTOM;
            }
            StonePaintData::StonePaintType stoneType = StonePaintData::None;
            if (stoneData != NULL) {
                stoneType = stoneData->stonePaintTypeAt(row, col);
            }
            bool drawRedCross = (row == highlightedCellRow && col == highlightedCellColumn);
            paintCell(widget, x, y, cellWidth, cellHeight, spec, stoneType, drawRedCross);
            y += cellHeight;
        }
        y = resetY;
        x += cellWidth;
    }
}

void BoardPainter::setHighlightedCell(int i, int j)
{
    highlightedCellRow = i;
    highlightedCellColumn = j;
}

void BoardPainter::paintCell(QWidget *widget, int x, int y, int width, int height, int positionSpec,
                             StonePaintData::StonePaintType stoneType, bool drawRedCross)
{

    QPainter painter(widget);

    QRect rect(x, y, width, height);

    if (positionSpec & CELL_POSITION_SPEC_BOTTOM) {
        rect.setBottom(y + height / 2);
    }

    if (positionSpec & CELL_POSITION_SPEC_TOP) {
        rect.setTop(y + height / 2);
    }

    if (positionSpec & CELL_POSITION_SPEC_LEFT) {
        rect.setLeft(x + width / 2);
    }

    if (positionSpec & CELL_POSITION_SPEC_RIGHT) {
        rect.setRight(x + width / 2);
    }

    painter.fillRect(rect.x(), rect.y(), rect.width(), rect.height(), Qt::lightGray);

    QPen pen;
    pen.setBrush(Qt::darkMagenta);

    QPoint start(x + width / 2, y);
    QPoint end(x + width / 2, y + height);

    if (positionSpec & CELL_POSITION_SPEC_BOTTOM) {\
        end.setY(y + height / 2);
    }

    if (positionSpec & CELL_POSITION_SPEC_TOP) {
        start.setY(y + height / 2);
    }

    if ((positionSpec & CELL_POSITION_SPEC_LEFT)
            || (positionSpec & CELL_POSITION_SPEC_RIGHT)) {
        pen.setWidth(2);
    } else {
        pen.setWidth(1);
    }

    painter.setPen(pen);
    painter.drawLine(start, end);

    start = QPoint(x, y + height / 2);
    end = QPoint(x + width, y + height / 2);

    if (positionSpec & CELL_POSITION_SPEC_LEFT) {
        start.setX(x + width / 2);
    }

    if (positionSpec & CELL_POSITION_SPEC_RIGHT) {
        end.setX(x + width / 2);
    }

    if ((positionSpec & CELL_POSITION_SPEC_BOTTOM) ||
            (positionSpec & CELL_POSITION_SPEC_TOP)) {
        pen.setWidth(2);
    } else {
        pen.setWidth(1);
    }

    painter.setPen(pen);
    painter.drawLine(start, end);

    QPointF centerPoint(x + width / 2.0, y + height / 2.0);
    QPointF focalPoint = QPointF(x + width * 0.7, y + height * 0.7);
    qreal eclipseRadius = sqrt(width * height) * 0.35;

    painter.setRenderHint(QPainter::Antialiasing);

    if (stoneType != StonePaintData::None) {
        if (stoneType == StonePaintData::Black || stoneType == StonePaintData::BlackHighlighted) {
            QRadialGradient grad(centerPoint, eclipseRadius, focalPoint);
            grad.setColorAt(0, QColor(Qt::darkGray));
            grad.setColorAt(1, QColor(Qt::black));
            QBrush brush(grad);
            painter.setBrush(brush);
        } else if (stoneType == StonePaintData::White || stoneType == StonePaintData::WhiteHighlighted) {
            QRadialGradient grad(centerPoint, eclipseRadius, focalPoint);
            grad.setColorAt(0, QColor(Qt::white));
            grad.setColorAt(1, QColor(Qt::lightGray));
            QBrush brush(grad);
            painter.setBrush(brush);
        }
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(centerPoint, eclipseRadius, eclipseRadius);
    }
    if (stoneType == StonePaintData::BlackHighlighted || stoneType == StonePaintData::WhiteHighlighted) {
        painter.setBrush(Qt::NoBrush);
        painter.setPen(Qt::green);
        painter.drawEllipse(centerPoint, eclipseRadius, eclipseRadius);
    }

    if (drawRedCross) {
        pen.setWidth(2);
        pen.setColor(Qt::red);
        painter.setPen(pen);
        painter.drawLine(centerPoint.x() - 4, centerPoint.y(), centerPoint.x() + 4, centerPoint.y());
        painter.drawLine(centerPoint.x(), centerPoint.y() - 4, centerPoint.x(), centerPoint.y() + 4);
    }
}
