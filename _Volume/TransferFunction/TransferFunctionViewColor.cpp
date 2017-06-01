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

#include "_Volume/TransferFunction/TransferFunctionViewColor.h"

TransferFunctionViewColor::TransferFunctionViewColor(QWidget *parent) : QGraphicsView(parent)
{
    m_LogFile = LogFile::m_LogFileGetInstanz();
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewColor::TransferFunctionViewColor) Executing", FuncUp);
    m_bVolumeIsSet = false; // asychroner refresh aufruf aus der ui löst sonst im refresh ein sigseg-vault aus!?
    m_scene = new QGraphicsScene(this);
    m_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    //horizontalScrollBar()->installEventFilter(this);
    //verticalScrollBar()->installEventFilter(this);

    setScene(m_scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorViewCenter);//AnchorUnderMouse);
    setResizeAnchor(AnchorViewCenter);

    m_vCreateContextMenue();
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewColor::TransferFunctionViewColor) OK", FuncDown);
}

bool TransferFunctionViewColor::m_bSetVolume(Volume* vol)
{
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewColor::m_bSetVolume) Executing", FuncUp);
    m_Volume = vol;
    m_bVolumeIsSet = true;
    QObject::connect(m_Volume, SIGNAL(m_vVolumeDataChanged()), this, SLOT(m_vSlotVolumeDataChanged()));
    QObject::connect(m_Volume->m_GetTransferfuction(), SIGNAL(m_vTransferfunctionLoaded()), this, SLOT(m_vSlotIfTransferfunctionLoaded()));
    m_vRepaintSceneContent();
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewColor::m_bSetVolume) OK", FuncDown);
    return true;
}

bool TransferFunctionViewColor::m_bColorNodeChanged(unsigned char oldgray, unsigned char gray, unsigned char red, unsigned char green, unsigned char blue)
{
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewColor::m_bColorNodeChanged) Executing", FuncUp);
    m_Volume->m_GetTransferfuction()->m_bColorNodeRemove(oldgray);
    bool res = (m_Volume->m_GetTransferfuction()->m_bColorNodeAdd(gray, red, green, blue) &&
                m_Volume->m_GetTransferfuction()->m_bBuildColorTableTexture());
    if(res == true)
        emit m_vTransferfunctionHasChanged();
    m_vRepaintSceneContent();
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewColor::m_bColorNodeChanged) OK", FuncDown);
    return res;
}

void TransferFunctionViewColor::m_vSlotIfTransferfunctionLoaded()
{
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewColor::m_vSlotIfTransferfunctionLoaded) Executing", FuncUp);
    emit m_vTransferfunctionHasChanged();
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewColor::m_vSlotIfTransferfunctionLoaded) OK", FuncDown);
}

void TransferFunctionViewColor::m_vRepaintSceneContent()
{
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewColor::m_vRepaintSceneContent) Executing", FuncUp);
    m_scene->clear();
    if(m_bVolumeIsSet)
    {
        m_vGenerateColorGradient();
        m_vPaintColorGradient();
        m_scene->addRect(m_dBorderSize, m_dBorderSize,
                        viewport()->rect().width() - 2*m_dBorderSize,
                        viewport()->rect().height() - 2*m_dBorderSize, QPen(QBrush(Qt::gray), m_dBorderWidth));
        m_vPaintNodes();
    }
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewColor::m_vRepaintSceneContent) OK", FuncDown);
}

void TransferFunctionViewColor::m_vPaintColorGradient()
{
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewColor::m_vPaintColorGradient) Executing", FuncUp);
    m_scene->addPixmap(m_DrawAbleHistogram);
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewColor::m_vPaintColorGradient) OK", FuncDown);
}

void TransferFunctionViewColor::m_vGenerateColorGradient()
{
    if(!m_bVolumeIsSet)
    {
        m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_vGenerateColorGradient) No Volume set");
        return;
    }
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewColor::m_vGenerateColorGradient) Executing", FuncUp);
    double w = viewport()->rect().width();
    double h = viewport()->rect().height();
    QImage img(w, h, QImage::Format_RGB32);
    QPainter painter(&img);

    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(0, 0, w, h, Qt::white);

    // Paint it
    for(int i = 0; i < 255; i++)
    {
        int gray1 = m_dGetWidthPosFromGray(i);
        int gray2;

        if (i != 255)
            gray2 = m_dGetWidthPosFromGray(i+1);
        else
            gray2 = w;

        unsigned char red = m_Volume->m_GetTransferfuction()->m_ucGetRedToGray(i);
        unsigned char green = m_Volume->m_GetTransferfuction()->m_ucGetGreenToGray(i);
        unsigned char blue = m_Volume->m_GetTransferfuction()->m_ucGetBlueToGray(i);

        //qDebug() << red;
        painter.fillRect(gray1, h-m_dBorderSize,
                         gray2-gray1, -(h-(2*m_dBorderSize)), QColor(red, green, blue));
    }

    m_DrawAbleHistogram = QPixmap::fromImage(img);
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewColor::m_vGenerateColorGradient) OK", FuncDown);
}

void TransferFunctionViewColor::m_vPaintNodes()
{
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewColor::m_vPaintNodes) Executing", FuncUp);
    const NodesList* x = m_Volume->m_GetTransferfuction()->m_GetAllColorNodes();
    for(int i = 0; i < x[0].m_Nodes.count(); i++)
    {
        NodeColor* n = new NodeColor(this, m_ContextMenu, x[0].m_Nodes[i].Gray, x[0].m_Nodes[i].Data, x[1].m_Nodes[i].Data, x[2].m_Nodes[i].Data);
        m_scene->addItem(n);
    }
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewColor::m_vPaintNodes) OK", FuncDown);
}

void TransferFunctionViewColor::m_vCreateContextMenue()
{
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewColor::m_vCreateContextMenue) Executing", FuncUp);
    QMenuBar* itemMenu = new QMenuBar();
    itemMenu->setVisible(false);
    //itemMenu->addMenu("Alphanode");

    m_AddNodeColor = new QAction(QIcon(":/Add32.png"), "&Hinzufügen", this);
    m_AddNodeColor->setStatusTip("Fügt einen neuen Colornode an dieser Position ein");
    QObject::connect(m_AddNodeColor, SIGNAL(triggered()), this, SLOT(m_vSlotAddNodeColor()));

    m_EditNodeColor = new QAction(QIcon(":/Edit32.png"), "&Bearbeiten", this);
    m_EditNodeColor->setStatusTip("Ermöglicht das explizite setzen von Werten für diesen Colornode");
    QObject::connect(m_EditNodeColor, SIGNAL(triggered()), this, SLOT(m_vSlotEditNodeColor()));

    m_DeleteNodeColor = new QAction(QIcon(":/Delete32.png"), "&Löschen", this);
    m_DeleteNodeColor->setStatusTip("Entfernt diesen Colornode aus der Transferfunktion");
    QObject::connect(m_DeleteNodeColor, SIGNAL(triggered()), this, SLOT(m_vSlotDeleteNodeColor()));

    m_ContextMenu = itemMenu->addMenu("Colornode");
    m_ContextMenu->addAction(m_AddNodeColor);
    m_ContextMenu->addAction(m_EditNodeColor);
    m_ContextMenu->addAction(m_DeleteNodeColor);
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewColor::m_vCreateContextMenue) OK", FuncDown);
}

void TransferFunctionViewColor::m_vSlotDeleteNodeColor()
{
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewColor::m_vSlotDeleteNodeColor) Executing", FuncUp);
    unsigned char gray = static_cast<NodeColor*>(this->scene()->selectedItems().first())->m_ucGetGray();
    bool res = (m_Volume->m_GetTransferfuction()->m_bColorNodeRemove(gray) &&
                m_Volume->m_GetTransferfuction()->m_bBuildColorTableTexture());
    if(res == true)
        emit m_vTransferfunctionHasChanged();
    m_vRepaintSceneContent();
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewColor::m_vSlotDeleteNodeColor) OK", FuncDown);
}

void TransferFunctionViewColor::m_vSlotAddNodeColor()
{
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewColor::m_vSlotAddNodeColor) Executing", FuncUp);
    unsigned char gray = m_ucGetGrayFromPosWidth(m_LastContextPos.x());
    unsigned char red = m_Volume->m_GetTransferfuction()->m_ucGetRedToGray(gray);
    unsigned char green = m_Volume->m_GetTransferfuction()->m_ucGetGreenToGray(gray);
    unsigned char blue = m_Volume->m_GetTransferfuction()->m_ucGetBlueToGray(gray);

    bool res = (m_Volume->m_GetTransferfuction()->m_bColorNodeAdd(gray, red, green, blue) &&
                m_Volume->m_GetTransferfuction()->m_bBuildColorTableTexture());
    if(res == true)
        emit m_vTransferfunctionHasChanged();
    m_vRepaintSceneContent();
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewColor::m_vSlotAddNodeColor) OK", FuncDown);
}

void TransferFunctionViewColor::m_vSlotEditNodeColor()
{
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewColor::m_vSlotEditNodeColor) Executing", FuncUp);
    unsigned char oldgray = static_cast<NodeColor*>(this->scene()->selectedItems().first())->m_ucGetGray();
    unsigned char oldred = static_cast<NodeColor*>(this->scene()->selectedItems().first())->m_ucGetRed();
    unsigned char oldgreen = static_cast<NodeColor*>(this->scene()->selectedItems().first())->m_ucGetGreen();
    unsigned char oldblue = static_cast<NodeColor*>(this->scene()->selectedItems().first())->m_ucGetBlue();
    EditNodeColorDialog dia;
    dia.m_vSetPreValues(oldgray, oldred, oldgreen, oldblue);
    if(dia.exec() == QDialog::Accepted)
    {
        bool res;
        if(oldgray == 0 || oldgray == 255)
        {
            res = (m_Volume->m_GetTransferfuction()->m_bColorNodeAdd(dia.m_ucGray, dia.m_ucRed, dia.m_ucGreen, dia.m_ucBlue) &&
                   m_Volume->m_GetTransferfuction()->m_bBuildColorTableTexture());
        }
        else
        {
            res = (m_Volume->m_GetTransferfuction()->m_bColorNodeRemove(oldgray) &&
                   m_Volume->m_GetTransferfuction()->m_bColorNodeAdd(dia.m_ucGray, dia.m_ucRed, dia.m_ucGreen, dia.m_ucBlue) &&
                   m_Volume->m_GetTransferfuction()->m_bBuildColorTableTexture());
        }
        if(res == true)
        {
            emit m_vTransferfunctionHasChanged();
        }
        m_vRepaintSceneContent();
    }
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewColor::m_vSlotEditNodeColor) OK", FuncDown);
}

void TransferFunctionViewColor::m_vSlotVolumeDataChanged()
{
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewColor::m_vSlotVolumeDataChanged) Executing", FuncUp);
    m_vGenerateColorGradient();
    m_vRepaintSceneContent();
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewColor::m_vSlotVolumeDataChanged) OK", FuncDown);
}

void TransferFunctionViewColor::resizeEvent(QResizeEvent* event)
{
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewColor::resizeEvent) Executing", FuncUp);
    double w = viewport()->rect().width();
    double h = viewport()->rect().height();
    setSceneRect(0,0,w,h);
    m_vGenerateColorGradient(); // weil Histogram Pixmap größen abhängig
    m_vRepaintSceneContent();
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewColor::resizeEvent) OK", FuncDown);
}

void TransferFunctionViewColor::contextMenuEvent(QContextMenuEvent* event)
{
    QGraphicsItem *item = itemAt(event->pos());
    if (item == 0) return;

    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewColor::contextMenuEvent) Executing", FuncUp);
    if (item->type() != 3 && item->type() != 6) // 3 ist der typ des vierecks, 6 ist linie!
    {
        // Klick geht nicht ins "freie" -> an andere weitergeben
        m_AddNodeColor->setEnabled(false);
        QGraphicsView::contextMenuEvent(event);
        m_AddNodeColor->setEnabled(true);
    }
    else
    {
        if(m_bClickIsInMyTFArea(event->pos()))
        {
            m_DeleteNodeColor->setEnabled(false);
            m_EditNodeColor->setEnabled(false);
            m_LastContextPos = event->pos();
            m_ContextMenu->exec(event->globalPos());
            m_EditNodeColor->setEnabled(true);
            m_DeleteNodeColor->setEnabled(true);
        }
    }
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewColor::contextMenuEvent) OK", FuncDown);
}

bool TransferFunctionViewColor::m_bClickIsInMyTFArea(QPoint pos)
{
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewColor::m_bClickIsInMyTFArea) Executing", FuncUp);
    bool erg = false;
    if(pos.x() > m_dBorderSize && pos.x() < viewport()->rect().width() - 2 * m_dBorderSize &&
       pos.y() > m_dBorderSize && pos.y() < viewport()->rect().height() - 2 * m_dBorderSize)
        erg = true;
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewColor::m_bClickIsInMyTFArea) OK", FuncDown);
    return erg;
}

double TransferFunctionViewColor::m_dGetHeightPosFromData(unsigned char data)
{
    double hei = viewport()->rect().height() - 2* m_dBorderSize;
    double ret = hei/2;
    return ret;
}

double TransferFunctionViewColor::m_dGetWidthPosFromGray(unsigned char gray)
{
    double wid = viewport()->rect().width() - 2* m_dBorderSize;
    double fak = gray/255.0;
    double ret = wid*fak + m_dBorderSize;
    return ret;
}

unsigned char TransferFunctionViewColor::m_ucGetDataFromPosHeight(double Height)
{
    double hei = viewport()->rect().height() - 2* m_dBorderSize;
    double fak = 255-((255.0/hei)*(Height - m_dBorderSize));
    unsigned char ret = (unsigned char)fak;
    return ret;
}

unsigned char TransferFunctionViewColor::m_ucGetGrayFromPosWidth(double Width)
{
    double wid = viewport()->rect().width() - 2* m_dBorderSize;
    double fak = 255.0/wid*(Width - m_dBorderSize);
    unsigned char ret = (unsigned char)fak;
    return ret;
}
