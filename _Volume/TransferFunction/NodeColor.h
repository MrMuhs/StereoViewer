// StereoViewer is a volume rendering program with a stereoscopic view
// Copyright (C) 2009 Pascal Mues
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#ifndef NODECOLOR_H
#define NODECOLOR_H

#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <QStyle>
#include "_Volume/TransferFunction/TransferFunctionViewColor.h"

class TransferFunctionViewColor;

class NodeColor : public QGraphicsItem
{
public:
    NodeColor(TransferFunctionViewColor* graphWidget, QMenu* contmenue, unsigned char gray, unsigned char red, unsigned char green, unsigned char blue);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    unsigned char m_ucGetGray();
    unsigned char m_ucGetRed();
    unsigned char m_ucGetGreen();
    unsigned char m_ucGetBlue();

protected:
    virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
    virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
    virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
    virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    TransferFunctionViewColor* m_TranferFunctionView;
    unsigned char m_ucGray;
    unsigned char m_ucRed;
    unsigned char m_ucGreen;
    unsigned char m_ucBlue;
    const double m_dItemRadius = 3.0;
    QMenu* m_ContextMenu;

    void m_vSetPos(double gray);
};

#endif // NODECOLOR_H
