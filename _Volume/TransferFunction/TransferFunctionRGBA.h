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

#ifndef TRANSFERFUNCTION_H
#define TRANSFERFUNCTION_H

#include <QtGui>
#include <QtOpenGL>
#include <QImage>
#include "_Volume/TransferFunction/iTransferFunction.h"
#include "_Volume/TransferFunction/NodesList.h"
#include "_Misc/LogFile.h"

class TransferFunctionRGBA : public iTransferFunction
{
    Q_OBJECT
public:
    TransferFunctionRGBA();
    virtual ~TransferFunctionRGBA();
    virtual bool m_bColorNodeAdd(unsigned char gray, unsigned char red, unsigned char green, unsigned char blue);
    virtual bool m_bColorNodeChange(unsigned char gray, unsigned char red, unsigned char green, unsigned char blue);
    virtual bool m_bColorNodeRemove(unsigned char gray);
    virtual bool m_bColorNodeClear();
    virtual bool m_bAlphaNodeAdd(unsigned char gray, unsigned char alpha);
    virtual bool m_bAlphaNodeChange(unsigned char gray, unsigned char alpha);
    virtual bool m_bAlphaNodeRemove(unsigned char gray);
    virtual bool m_bAlphaNodeClear();
    virtual const NodesList* m_GetAllColorNodes() const;
    virtual const NodesList m_GetAllAlphaNodes() const;
    virtual const unsigned char m_ucGetRedToGray(unsigned char gray);
    virtual const unsigned char m_ucGetGreenToGray(unsigned char gray);
    virtual const unsigned char m_ucGetBlueToGray(unsigned char gray);
    virtual const unsigned char m_ucGetAlphaToGray(unsigned char gray);
    virtual bool m_bBuildColorTableTexture();
    virtual const unsigned int m_uiGetTextureName() const;

    enum ColorNodesIndicies {R = 0, G = 1, B = 2};

private:
    static const int m_ciSizeOfColor = 4;
    static const int m_ciArraySize = 256;
    unsigned int m_uiTexName;
    unsigned char m_aColorMap[m_ciArraySize * m_ciSizeOfColor];
    NodesList m_aColorNodes[3];
    NodesList m_AlphaNodes;

    void m_vSaveAsImage(QString filename);
    void m_vSaveAsFile(QString filename);
    bool m_bGenerateColorMap();
};

#endif // TRANSFERFUNCTION_H
