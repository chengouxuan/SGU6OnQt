#ifndef BOARDPAINTER_H
#define BOARDPAINTER_H

#include <QWidget>


class StonePaintData {

public:

    enum StonePaintType {
        None, White, Black, WhiteHighlighted, BlackHighlighted
    };


    virtual StonePaintType stonePaintTypeAt(int row, int column) = 0;

};

class BoardPainter
{
public:


    BoardPainter(StonePaintData *sd);

    void paint(QWidget *widget);
    void setHighlightedCell(int i, int j);
    void setStoneData(StonePaintData *stoneData);

private:

    void paintCell(QWidget *widget, int x, int y, int width, int height, int positionSpec,
                   StonePaintData::StonePaintType stoneType, bool drawRedCross);

    const static int numOfRows = 19;
    const static int numOfColumns = 19;

    int highlightedCellRow;
    int highlightedCellColumn;

    StonePaintData *stoneDataProvider;
};

#endif // BOARDPAINTER_H
