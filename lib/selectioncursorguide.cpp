/*
    Copyright 2020, Mitch Curtis

    This file is part of Slate.

    Slate is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Slate is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Slate. If not, see <http://www.gnu.org/licenses/>.
*/

#include "selectioncursorguide.h"

#include "canvaspane.h"
#include "imagecanvas.h"
#include "panedrawinghelper.h"

#include <QPainter>

// TODO: see if we can turn this into two thin items rather than one big one.
// could even just try having the two items as a child of this one

SelectionCursorGuide::SelectionCursorGuide(ImageCanvas *canvas) :
    QQuickPaintedItem(canvas),
    mCanvas(canvas)
{
    setObjectName("selectionCursorGuide");
}

SelectionCursorGuide::~SelectionCursorGuide()
{
}

void SelectionCursorGuide::paint(QPainter *painter)
{
    if (mCanvas->isSplitScreen()) {
        drawPane(painter, mCanvas->secondPane(), 1);
    }

    drawPane(painter, mCanvas->firstPane(), 0);
}

void SelectionCursorGuide::drawPane(QPainter *painter, const CanvasPane *pane, int paneIndex)
{
    PaneDrawingHelper paneDrawingHelper(mCanvas, painter, pane, paneIndex);

    painter->save();

    QPen pen;
    pen.setColor(Qt::gray);
    pen.setStyle(Qt::DotLine);
    painter->setPen(pen);

    // Draw the vertical cursor selection guide.
    painter->save();

    int guidePosition = mCanvas->cursorSceneX();
    qreal zoomedGuidePosition = (guidePosition * pane->integerZoomLevel()) + (painter->pen().widthF() / 2.0);
    painter->translate(0, -pane->integerOffset().y());
    painter->drawLine(QLineF(zoomedGuidePosition, 0, zoomedGuidePosition, height()));

    painter->restore();

    // Draw the horizontal cursor selection guide.
    guidePosition = mCanvas->cursorSceneY();
    zoomedGuidePosition = (guidePosition * pane->integerZoomLevel()) + (painter->pen().widthF() / 2.0);
    painter->translate(-pane->integerOffset().x(), 0);
    painter->drawLine(QLineF(0, zoomedGuidePosition, mCanvas->paneWidth(paneIndex), zoomedGuidePosition));

    painter->restore();
}
