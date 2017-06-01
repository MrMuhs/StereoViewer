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

#include "_Volume/TransferFunction/TransferFunctionViewAlpha.h"

TransferFunctionViewAlpha::TransferFunctionViewAlpha(QWidget *parent) : QGraphicsView(parent)
{
    m_LogFile = LogFile::m_LogFileGetInstanz();
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::TransferFunctionViewAlpha) Executing", FuncUp);
    m_bVolumeIsSet = false; // asychroner refresh aufruf aus der ui löst sonst im refresh ein sigseg-vault aus!?
    m_iHistogrammScaling = Linear;
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
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::TransferFunctionViewAlpha) OK", FuncDown);
}

bool TransferFunctionViewAlpha::m_bSetVolume(Volume* vol)
{
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_bSetVolume) Executing", FuncUp);
    m_Volume = vol;
    m_bVolumeIsSet = true;
    QObject::connect(m_Volume, SIGNAL(m_vVolumeDataChanged()), this, SLOT(m_vSlotVolumeDataChanged()));
    QObject::connect(m_Volume->m_GetTransferfuction(), SIGNAL(m_vTransferfunctionLoaded()), this, SLOT(m_vSlotIfTransferfunctionLoaded()));
    m_vRepaintSceneContent();
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_bSetVolume) OK", FuncDown);
    return true;
}

bool TransferFunctionViewAlpha::m_bAlphaNodeChanged(unsigned char oldgray, unsigned char gray, unsigned char alpha)
{
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_bAlphaNodeChanged) Executing", FuncUp);
    m_Volume->m_GetTransferfuction()->m_bAlphaNodeRemove(oldgray);
    bool res = (m_Volume->m_GetTransferfuction()->m_bAlphaNodeAdd(gray, alpha) &&
                m_Volume->m_GetTransferfuction()->m_bBuildColorTableTexture());

    if(res == true)
        emit m_vTransferfunctionHasChanged();
    m_vRepaintSceneContent();
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_bAlphaNodeChanged) OK", FuncDown);
    return res;
}

void TransferFunctionViewAlpha::m_vSlotIfTransferfunctionLoaded()
{
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_vSlotIfTransferfunctionLoaded) Executing", FuncUp);
    emit m_vTransferfunctionHasChanged();
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_vSlotIfTransferfunctionLoaded) OK", FuncDown);
}

void TransferFunctionViewAlpha::m_vSlotHistogramScalingChanged(int index)
{
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_vSlotHistogramScalingChanged) Executing", FuncUp);
    switch(index)
    {
    case Linear:
        m_LogFile->m_bAppendMessage(Debug, "TransferFunctionViewAlpha::m_vSlotHistogramScalingChanged(int index) Setting internal field to Linear");
        m_iHistogrammScaling = index;
        m_vGenerateHistogram();
        m_vRepaintSceneContent();
        break;
    case Logarithmical:
        m_LogFile->m_bAppendMessage(Debug, "TransferFunctionViewAlpha::m_vSlotHistogramScalingChanged(int index) Setting internal field to Logarithmical");
        m_iHistogrammScaling = index;
        m_vGenerateHistogram();
        m_vRepaintSceneContent();
        break;
    default:
        m_LogFile->m_bAppendMessage(Error, "TransferFunctionViewAlpha::m_vSlotHistogramScalingChanged(int index) Undefined index");
    }
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_vSlotHistogramScalingChanged) OK", FuncDown);
}

void TransferFunctionViewAlpha::m_vRepaintSceneContent()
{
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_vRepaintSceneContent) Executing", FuncUp);
    m_scene->clear();
    if(m_bVolumeIsSet)
    {
        m_vPaintHistogram();
        m_scene->addRect(m_dBorderSize, m_dBorderSize,
                        viewport()->rect().width() - 2*m_dBorderSize,
                        viewport()->rect().height() - 2*m_dBorderSize, QPen(QBrush(Qt::gray), m_dBorderWidth));
        m_vPaintLines();
        m_vPaintNodes();
    }
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_vRepaintSceneContent) OK", FuncDown);
}

void TransferFunctionViewAlpha::m_vPaintHistogram()
{
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_vPaintHistogram) Executing", FuncUp);
    m_scene->addPixmap(m_DrawAbleHistogram);
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_vPaintHistogram) OK", FuncDown);
}

void TransferFunctionViewAlpha::m_vGenerateHistogram()
{
    if(!m_bVolumeIsSet)
    {
        m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_vGenerateHistogram) No Volume set");
        return;
    }

    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_vGenerateHistogram) Executing", FuncUp);
    QList<double>* histogram = m_Volume->m_GetHistogram();
    if(histogram != 0)
    {
        double w = viewport()->rect().width();
        double h = viewport()->rect().height();
        QImage img(w, h, QImage::Format_RGB32);
        QPainter painter(&img);

        painter.setCompositionMode(QPainter::CompositionMode_Source);
        painter.fillRect(0, 0, w, h, Qt::white);

        // Paint it
        double dHistogrammMaxValue = 0;
        for(int i = 0; i < histogram->count(); i++)
        {
            if(histogram->at(i) > dHistogrammMaxValue) dHistogrammMaxValue = histogram->at(i);
        }
        for(int i = 0; i < histogram->count(); i++)
        {
            int gray1 = m_dGetWidthPosFromGray(i);
            int gray2;
            //qDebug() << i;
            if (i != 255)
                gray2 = m_dGetWidthPosFromGray(i+1);
            else
                gray2 = w;

            int x, y, balkenW, balkenH;
            x = gray1 - ((gray2-gray1)/2);
            y = h-m_dBorderSize;
            balkenW = gray2-gray1;

            // check the scaling mode
            switch(m_iHistogrammScaling)
            {
                case Linear:
                    balkenH = -((h-(2*m_dBorderSize))*(histogram->at(i)/dHistogrammMaxValue));
                    break;
                case Logarithmical:
                    if(histogram->at(i) > 0)
                        balkenH = -((h-(2*m_dBorderSize))*(log(histogram->at(i))/log(dHistogrammMaxValue)));
                    else
                        balkenH = 0;
                    break;
                default: balkenH = 0;
            }
            painter.fillRect(x, y, balkenW, balkenH, Qt::lightGray);
        }

        m_DrawAbleHistogram = QPixmap::fromImage(img);
    }
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_vGenerateHistogram) OK", FuncDown);
}

void TransferFunctionViewAlpha::m_vPaintNodes()
{
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_vPaintNodes) Executing", FuncUp);
    const NodesList x = m_Volume->m_GetTransferfuction()->m_GetAllAlphaNodes();
    for(int i = 0; i < x.m_Nodes.count(); i++)
    {
        NodeAlpha* n = new NodeAlpha(this, m_ContextMenu, x.m_Nodes[i].Gray, x.m_Nodes[i].Data);
        m_scene->addItem(n);
    }
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_vPaintNodes) OK", FuncDown);
}

void TransferFunctionViewAlpha::m_vPaintLines()
{
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_vPaintLines) Executing", FuncUp);
    NodesList x = m_Volume->m_GetTransferfuction()->m_GetAllAlphaNodes();
    if(x.m_Nodes.count() < 2 )
    {
        m_LogFile->m_bAppendMessage(Error, "(TransferFunctionViewAlpha::m_vPaintLines) Not enough nodes");
        m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_vPaintLines) FAIL", FuncDown);
        return;
    }
    for(int i = 0; i < x.m_Nodes.count() - 1; i++)
    {
        double x1 = m_dGetWidthPosFromGray(x.m_Nodes.at(i).Gray);
        double y1 = m_dGetHeightPosFromData(x.m_Nodes.at(i).Data);
        double x2 = m_dGetWidthPosFromGray(x.m_Nodes.at(i+1).Gray);
        double y2 = m_dGetHeightPosFromData(x.m_Nodes.at(i+1).Data);
        m_scene->addLine(x1, y1, x2, y2);
    }
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_vPaintLines) OK", FuncDown);
}

void TransferFunctionViewAlpha::m_vCreateContextMenue()
{
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_vCreateContextMenue) Executing", FuncUp);
    QMenuBar* itemMenu = new QMenuBar();
    itemMenu->setVisible(false);
    //itemMenu->addMenu("Alphanode");

    m_AddNodeAlpha = new QAction(QIcon(":/Add32.png"), "&Hinzufügen", this);
    m_AddNodeAlpha->setStatusTip("Fügt einen neuen Alphanode an dieser Position ein");
    QObject::connect(m_AddNodeAlpha, SIGNAL(triggered()), this, SLOT(m_vSlotAddNodeAlpha()));

    m_EditNodeAlpha = new QAction(QIcon(":/Edit32.png"), "&Bearbeiten", this);
    m_EditNodeAlpha->setStatusTip("Ermöglicht das explizite setzen von Werten für diesen Alphanode");
    QObject::connect(m_EditNodeAlpha, SIGNAL(triggered()), this, SLOT(m_vSlotEditNodeAlpha()));

    m_DeleteNodeAlpha = new QAction(QIcon(":/Delete32.png"), "&Löschen", this);
    m_DeleteNodeAlpha->setStatusTip("Entfernt diesen Alphanode aus der Transferfunktion");
    QObject::connect(m_DeleteNodeAlpha, SIGNAL(triggered()), this, SLOT(m_vSlotDeleteNodeAlpha()));

    m_ContextMenu = itemMenu->addMenu("Alphanode");
    m_ContextMenu->addAction(m_AddNodeAlpha);
    m_ContextMenu->addAction(m_EditNodeAlpha);
    m_ContextMenu->addAction(m_DeleteNodeAlpha);
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_vCreateContextMenue) OK", FuncDown);
}

void TransferFunctionViewAlpha::m_vSlotDeleteNodeAlpha()
{
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_vSlotDeleteNodeAlpha) Executing", FuncUp);
    unsigned char gray = static_cast<NodeAlpha*>(this->scene()->selectedItems().first())->m_ucGetGray();
    bool res = (m_Volume->m_GetTransferfuction()->m_bAlphaNodeRemove(gray) &&
                m_Volume->m_GetTransferfuction()->m_bBuildColorTableTexture());
    if(res == true)
        emit m_vTransferfunctionHasChanged();
    m_vRepaintSceneContent();
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_vSlotDeleteNodeAlpha) OK", FuncDown);
}

void TransferFunctionViewAlpha::m_vSlotAddNodeAlpha()
{
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_vSlotAddNodeAlpha) Executing", FuncUp);
    unsigned char gray = m_ucGetGrayFromPosWidth(m_LastContextPos.x());
    unsigned char alpha = m_ucGetDataFromPosHeight(m_LastContextPos.y());
    bool res = (m_Volume->m_GetTransferfuction()->m_bAlphaNodeAdd(gray, alpha) &&
                m_Volume->m_GetTransferfuction()->m_bBuildColorTableTexture());
    if(res == true)
        emit m_vTransferfunctionHasChanged();
    m_vRepaintSceneContent();
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_vSlotAddNodeAlpha) OK", FuncDown);
}

void TransferFunctionViewAlpha::m_vSlotEditNodeAlpha()
{
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_vSlotEditNodeAlpha) Executing", FuncUp);
    QGraphicsItem* n1 = this->scene()->selectedItems().first();
    //unsigned char oldgray = static_cast<NodeAlpha*>(this->scene()->selectedItems().first())->m_ucGetGray();
    unsigned char oldgray = ((NodeAlpha*)n1)->m_ucGetGray();
    //unsigned char oldalpha = static_cast<NodeAlpha*>(this->scene()->selectedItems().first())->m_ucGetData();
    unsigned char oldalpha = ((NodeAlpha*)n1)->m_ucGetData();
    EditNodeAlphaDialog dia;
    dia.m_vSetPreValues(oldgray, oldalpha);
    if(dia.exec() == QDialog::Accepted)
    {
        bool res;
        if(oldgray == 0 || oldgray == 255)
            res = (m_Volume->m_GetTransferfuction()->m_bAlphaNodeAdd(dia.m_ucGray, dia.m_ucAlpha) &&
                   m_Volume->m_GetTransferfuction()->m_bBuildColorTableTexture());
        else
            res = (m_Volume->m_GetTransferfuction()->m_bAlphaNodeRemove(oldgray) &&
                   m_Volume->m_GetTransferfuction()->m_bAlphaNodeAdd(dia.m_ucGray, dia.m_ucAlpha) &&
                   m_Volume->m_GetTransferfuction()->m_bBuildColorTableTexture());
        if(res == true)
            emit m_vTransferfunctionHasChanged();
        m_vRepaintSceneContent();
    }
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_vSlotEditNodeAlpha) OK", FuncDown);
}

void TransferFunctionViewAlpha::m_vSlotVolumeDataChanged()
{
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_vSlotVolumeDataChanged) Executing", FuncUp);
    m_vGenerateHistogram();
    m_vRepaintSceneContent();
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_vSlotVolumeDataChanged) OK", FuncDown);
}

void TransferFunctionViewAlpha::resizeEvent(QResizeEvent* event)
{
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::resizeEvent) Executing", FuncUp);
    double w = viewport()->rect().width();
    double h = viewport()->rect().height();
    setSceneRect(0,0,w,h);
    m_vGenerateHistogram(); // weil Histogram Pixmap größen abhängig
    m_vRepaintSceneContent();
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::resizeEvent) OK", FuncDown);
}

void TransferFunctionViewAlpha::contextMenuEvent(QContextMenuEvent* event)
{
    QGraphicsItem *item = itemAt(event->pos());
    if (item == 0) return;

    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::contextMenuEvent) Executing", FuncUp);
    if (item->type() != 3 && item->type() != 6) // 3 ist der typ des vierecks, 6 ist linie!
    {
        // Klick geht nicht ins "freie" -> an andere weitergeben
        m_AddNodeAlpha->setEnabled(false);
        QGraphicsView::contextMenuEvent(event);
        m_AddNodeAlpha->setEnabled(true);
    }
    else
    {
        if(m_bClickIsInMyTFArea(event->pos()))
        {
            m_DeleteNodeAlpha->setEnabled(false);
            m_EditNodeAlpha->setEnabled(false);
            m_LastContextPos = event->pos();
            m_ContextMenu->exec(event->globalPos());
            m_EditNodeAlpha->setEnabled(true);
            m_DeleteNodeAlpha->setEnabled(true);
        }
    }
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::contextMenuEvent) OK", FuncDown);
}

bool TransferFunctionViewAlpha::m_bClickIsInMyTFArea(QPoint pos)
{
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_bClickIsInMyTFArea) Executing", FuncUp);
    bool erg = false;
    if(pos.x() > m_dBorderSize && pos.x() < viewport()->rect().width() - 2 * m_dBorderSize &&
       pos.y() > m_dBorderSize && pos.y() < viewport()->rect().height() - 2 * m_dBorderSize)
        erg = true;
    m_LogFile->m_bAppendMessage(Debug, "(TransferFunctionViewAlpha::m_bClickIsInMyTFArea) OK", FuncDown);
    return erg;
}

double TransferFunctionViewAlpha::m_dGetHeightPosFromData(unsigned char data)
{
    double hei = viewport()->rect().height() - 2* m_dBorderSize;
    double fak = data/255.0;
    double ret = hei-(hei*fak) + m_dBorderSize;
    return ret;
}

double TransferFunctionViewAlpha::m_dGetWidthPosFromGray(unsigned char gray)
{
    double wid = viewport()->rect().width() - 2* m_dBorderSize;
    double fak = gray/255.0;
    double ret = wid*fak + m_dBorderSize;
    return ret;
}

unsigned char TransferFunctionViewAlpha::m_ucGetDataFromPosHeight(double Height)
{
    double hei = viewport()->rect().height() - 2* m_dBorderSize;
    double fak = 255-((255.0/hei)*(Height - m_dBorderSize));
    unsigned char ret = (unsigned char)fak;
    return ret;
}

unsigned char TransferFunctionViewAlpha::m_ucGetGrayFromPosWidth(double Width)
{
    double wid = viewport()->rect().width() - 2* m_dBorderSize;
    double fak = 255.0/wid*(Width - m_dBorderSize);
    unsigned char ret = (unsigned char)fak;
    return ret;
}
