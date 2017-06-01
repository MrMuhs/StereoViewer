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

#ifndef ITRANSFERFUNCTION_H
#define ITRANSFERFUNCTION_H
#include <QtOpenGL>
#include <QDomDocument>
#include <_Misc/LogFile.h>
#include "_Volume/TransferFunction/NodesList.h"

class iTransferFunction : public QObject
{
    Q_OBJECT
public:
    iTransferFunction();
    virtual ~iTransferFunction() {}
    virtual bool m_bColorNodeAdd(unsigned char gray, unsigned char red, unsigned char green, unsigned char blue) = 0;
    virtual bool m_bColorNodeChange(unsigned char gray, unsigned char red, unsigned char green, unsigned char blue) = 0;
    virtual bool m_bColorNodeRemove(unsigned char gray) = 0;
    virtual bool m_bColorNodeClear() = 0;
    virtual bool m_bAlphaNodeAdd(unsigned char gray, unsigned char alpha) = 0;
    virtual bool m_bAlphaNodeChange(unsigned char gray, unsigned char alpha) = 0;
    virtual bool m_bAlphaNodeRemove(unsigned char gray) = 0;
    virtual bool m_bAlphaNodeClear() = 0;
    virtual const NodesList* m_GetAllColorNodes() const = 0;
    virtual const NodesList m_GetAllAlphaNodes() const = 0;
    virtual const unsigned char m_ucGetRedToGray(unsigned char gray) = 0;
    virtual const unsigned char m_ucGetGreenToGray(unsigned char gray) = 0;
    virtual const unsigned char m_ucGetBlueToGray(unsigned char gray) = 0;
    virtual const unsigned char m_ucGetAlphaToGray(unsigned char gray) = 0;
    virtual bool m_bBuildColorTableTexture() = 0;
    virtual const unsigned int m_uiGetTextureName() const = 0;

    // open/save functions are the same foreach class using the interface
    bool m_bGenerateTFFromXMLFile(QString filename);
    bool m_bGenerateTFToXMLFile(QString filename, QString description);
    QString m_sGetFilename();
    QString m_sGetDescription();

signals:
    void m_vTransferfunctionLoaded();

protected:
    LogFile* m_LogFile;

private:
    QString m_sFilename;
    QString m_sDescription;

};
#endif // ITRANSFERFUNCTION_H
