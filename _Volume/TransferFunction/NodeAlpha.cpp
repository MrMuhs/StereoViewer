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

#include "_Volume/TransferFunction/NodeAlpha.h"

NodeAlpha::NodeAlpha(TransferFunctionViewAlpha *graphWidget, QMenu* contmenue, unsigned char gray, unsigned char data)
{
    m_TranferFunctionView = graphWidget;
    m_ContextMenu = contmenue;
    m_ucGray = gray;
    m_ucData = data;

    setAcceptDrops(true);
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    //setZValue(-1);
    m_vSetPos(gray, data);
}

QRectF NodeAlpha::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(-(m_dItemRadius/2) - adjust, -(m_dItemRadius/2) - adjust,
                  m_dItemRadius*2 + adjust, m_dItemRadius*2 + adjust);
}

void NodeAlpha::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * widget)
{
    m_vSetPos(m_ucGray, m_ucData);

    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    //painter->drawEllipse(-(m_dItemRadius/2), -(m_dItemRadius/2), m_dItemRadius*2, m_dItemRadius*2);
    painter->drawEllipse( (0-m_dItemRadius), (0-m_dItemRadius), m_dItemRadius*2, m_dItemRadius*2);

    QRadialGradient gradient(0-m_dItemRadius, 0-m_dItemRadius, 10);
    if (option->state & QStyle::State_Sunken) {
        gradient.setCenter(m_dItemRadius, m_dItemRadius);
        gradient.setFocalPoint(m_dItemRadius, m_dItemRadius);
        gradient.setColorAt(1, QColor(Qt::yellow).light(120));
        gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
    } else {
        gradient.setColorAt(0, Qt::yellow);
        gradient.setColorAt(1, Qt::darkYellow);
    }
    painter->setBrush(gradient);
    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse((0-m_dItemRadius), (0-m_dItemRadius), m_dItemRadius*2, m_dItemRadius*2);
}

void NodeAlpha::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
    QGraphicsItem::mouseDoubleClickEvent(event);
}

void NodeAlpha::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
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
                           QString::number(m_TranferFunctionView->m_ucGetGrayFromPosWidth(pos().x())) + " " + QString::number(m_TranferFunctionView->m_ucGetDataFromPosHeight(pos().y())),
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

unsigned char NodeAlpha::m_ucGetGray()
{
    return m_ucGray;
}

unsigned char NodeAlpha::m_ucGetData()
{
    return m_ucData;
}

void NodeAlpha::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    QGraphicsItem::mousePressEvent(event);
}

void NodeAlpha::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    m_ContextMenu->exec(event->screenPos());
}

void NodeAlpha::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    double oldgray = m_ucGray;
    m_ucGray = m_TranferFunctionView->m_ucGetGrayFromPosWidth(pos().x());
    m_ucData = m_TranferFunctionView->m_ucGetDataFromPosHeight(pos().y());
    QGraphicsItem::mouseReleaseEvent(event);
    m_TranferFunctionView->m_bAlphaNodeChanged(oldgray, m_ucGray, m_ucData);
}

void NodeAlpha::m_vSetPos(double gray, double data)
{
    setPos(m_TranferFunctionView->m_dGetWidthPosFromGray(gray), m_TranferFunctionView->m_dGetHeightPosFromData(data));
}
