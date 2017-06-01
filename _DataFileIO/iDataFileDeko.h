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

#ifndef IDATAFILEDEKO_H
#define IDATAFILEDEKO_H

#include "_DataFileIO/iDataFile.h"

class iDataFileDeko : public iDataFile
{
    Q_OBJECT
public:
    virtual bool m_SetObjektToDekorateLaterBind(iDataFile* ToDecorated) { m_DataFile = ToDecorated; return true; }

protected:
    iDataFile* m_DataFile;
};

#endif // IDATAFILEDEKO_H
