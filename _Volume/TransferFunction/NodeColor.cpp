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

#include "_Volume/TransferFunction/NodeColor.h"

NodeColor::NodeColor(TransferFunctionViewColor *graphWidget, QMenu* contmenue, unsigned char gray, unsigned char red, unsigned char green, unsigned char blue)
{
    m_TranferFunctionView = graphWidget;
    m_ContextMenu = contmenue;
    m_ucGray = gray;
    m_ucRed = red;
    m_ucGreen = green;
    m_ucBlue = blue;

    setAcceptDrops(true);
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    //setZValue(-1);
    m_vSetPos(m_ucGray);
}

QRectF NodeColor::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(-(m_dItemRadius/2) - adjust, -(m_dItemRadius/2) - adjust,
                  m_dItemRadius*2 + adjust, m_dItemRadius*2 + adjust);
}

void NodeColor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * widget)
{
    m_vSetPos(m_ucGray);

    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    //painter->drawEllipse(-(m_dItemRadius/2), -(m_dItemRadius/2), m_dItemRadius*2, m_dItemRadius*2);
    painter->drawEllipse( (0-m_dItemRadius), (0-m_dItemRadius), m_dItemRadius*2, m_dItemRadius*2);

    QRadialGradient gradient(0-m_dItemRadius, 0-m_dItemRadius, 10);
    if (option->state & QStyle::State_Sunken) {
        gradient.setCenter(m_dItemRadius, m_dItemRadius);
        gradient.setFocalPoint(m_dItemRadius, m_dItemRadius);
        gradient.setColorAt(1, QColor(m_ucRed, m_ucGreen, m_ucBlue).light(120));
        gradient.setColorAt(0, QColor(m_ucRed, m_ucGreen, m_ucBlue).light(120));
    } else {
        gradient.setColorAt(0, QColor(m_ucRed, m_ucGreen, m_ucBlue));
        gradient.setColorAt(1, QColor(m_ucRed, m_ucGreen, m_ucBlue));
    }
    painter->setBrush(gradient);
    painter->setPen(QPen(QColor(255-m_ucRed, 255-m_ucGreen, 255-m_ucBlue), 2));
    painter->drawEllipse((0-m_dItemRadius), (0-m_dItemRadius), m_dItemRadius*2, m_dItemRadius*2);
}

void NodeColor::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
    QGraphicsItem::mouseDoubleClickEvent(event);
}

void NodeColor::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    // Position begrenzen auf Breite/Höhe der Szene
    static bool bJustOneTime = false;
    if(bJustOneTime == false)
    {
        bJustOneTime = true;

        bool weitergeben = true;
        if(this->pos().x() > m_TranferFunctionView->viewport()->rect().width() - m_TranferFunctionView->m_dBorderSize)
        {
            this->setPos(m_TranferFunctionView->viewport()->rect().width() - m_TranferFunctionView->m_dBorderSize, this->pos().y());
            weitergeben = false;
        }
        if(this->pos().x() < 0 + m_TranferFunctionView->m_dBorderSize)
        {
            this->setPos(0 + m_TranferFunctionView->m_dBorderSize, this->pos().y());
            weitergeben = false;
        }
        if(this->pos().y() > m_TranferFunctionView->viewport()->rect().height() - m_TranferFunctionView->m_dBorderSize)
        {
            this->setPos(this->pos().x(), m_TranferFunctionView->viewport()->rect().height() - m_TranferFunctionView->m_dBorderSize);
            weitergeben = false;
        }
        if(this->pos().y() < 0 + m_TranferFunctionView->m_dBorderSize)
        {
            this->setPos(this->pos().x(), 0 + m_TranferFunctionView->m_dBorderSize);
            weitergeben = false;
        }

        QToolTip::showText(QPoint(event->screenPos().x(), event->screenPos().y()),
                           QString::number(m_TranferFunctionView->m_ucGetGrayFromPosWidth(pos().x())),
                           m_TranferFunctionView);

        if (weitergeben == true)
            QGraphicsItem::mouseMoveEvent(event);
    }
    else
    {
        bJustOneTime = false;
        return;
    }
}

unsigned char NodeColor::m_ucGetGray()
{
    return m_ucGray;
}

unsigned char NodeColor::m_ucGetRed()
{
    return m_ucRed;
}

unsigned char NodeColor::m_ucGetGreen()
{
    return m_ucGreen;
}

unsigned char NodeColor::m_ucGetBlue()
{
    return m_ucBlue;
}

void NodeColor::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    QGraphicsItem::mousePressEvent(event);
}

void NodeColor::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    m_ContextMenu->exec(event->screenPos());
}

void NodeColor::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    double oldgray = m_ucGray;
    m_ucGray = m_TranferFunctionView->m_ucGetGrayFromPosWidth(pos().x());
    QGraphicsItem::mouseReleaseEvent(event);
    m_TranferFunctionView->m_bColorNodeChanged(oldgray, m_ucGray, m_ucRed, m_ucGreen, m_ucBlue);
}

void NodeColor::m_vSetPos(double gray)
{
    setPos(m_TranferFunctionView->m_dGetWidthPosFromGray(gray), m_TranferFunctionView->m_dGetHeightPosFromData(gray));
}
