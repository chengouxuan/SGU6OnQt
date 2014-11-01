#ifndef BOARDPAINTER_H
#define BOARDPAINTER_H

#include <QWidget>

class StoneData {

public:

    enum StoneType {
        None, White, Black, WhiteHighlighted, BlackHighlighted
    };


    virtual StoneType getStoneTypeAt(int row, int column) = 0;

};

class BoardPainter
{
public:

    BoardPainter(StoneData *sd);

    void paint(QWidget *widget);

private:

    void paintCell(QWidget *widget, int x, int y, int width, int height, int positionSpec, StoneData::StoneType stoneType);

    const static int numOfRows = 19;
    const static int numOfColumns = 19;

    StoneData *stoneDataProvider;
};

#endif // BOARDPAINTER_H
