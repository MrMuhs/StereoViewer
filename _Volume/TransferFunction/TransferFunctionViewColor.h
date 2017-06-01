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

#ifndef TRANSFERFUNCTIONVIEWCOLOR_H
#define TRANSFERFUNCTIONVIEWCOLOR_H

#include <QGraphicsView>
#include "_Volume/Volume.h"
#include "_Volume/TransferFunction/NodeColor.h"
#include "gui/EditNodeColorDialog.h"

class TransferFunctionViewColor : public QGraphicsView
{
    Q_OBJECT
public:
    TransferFunctionViewColor(QWidget *parent = 0);
    void m_vRepaintSceneContent();
    bool m_bSetVolume(Volume* tf);
    bool m_bColorNodeChanged(unsigned char oldgray, unsigned char gray, unsigned char red, unsigned char green, unsigned char blue);
    double m_dGetHeightPosFromData(unsigned char data);
    double m_dGetWidthPosFromGray(unsigned char gray);
    unsigned char m_ucGetDataFromPosHeight(double data);
    unsigned char m_ucGetGrayFromPosWidth(double gray);

    const double m_dBorderSize = 5.0;
    const double m_dBorderWidth = 1.0;

signals:
    void m_vTransferfunctionHasChanged();

public slots:
    void m_vSlotAddNodeColor();
    void m_vSlotEditNodeColor();
    void m_vSlotDeleteNodeColor();
    void m_vSlotVolumeDataChanged();
    void m_vSlotIfTransferfunctionLoaded();

protected:
    virtual void contextMenuEvent(QContextMenuEvent* event);
    virtual void resizeEvent(QResizeEvent* event);

private:
    LogFile* m_LogFile;
    QGraphicsScene* m_scene;
    QMenu* m_ContextMenu;               // Context-Menue
    QAction* m_AddNodeColor;            // Context-Menue
    QAction* m_EditNodeColor;           // Context-Menue
    QAction* m_DeleteNodeColor;         // Context-Menue
    QPoint m_LastContextPos;            // Context-Menue
    bool m_bVolumeIsSet;
    Volume* m_Volume;
    QPixmap m_DrawAbleHistogram;

    void m_vPaintColorGradient();
    void m_vPaintNodes();
    void m_vCreateContextMenue();           // Context-Menue
    bool m_bClickIsInMyTFArea(QPoint pos);  // Context-Menue
    void m_vGenerateColorGradient();
};

#endif // TRANSFERFUNCTIONVIEWCOLOR_H
