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

#ifndef NODESLIST_H
#define NODESLIST_H

#include <QList>
/*#ifdef _WIN32
    #include <windef.h>
#endif*/
// wtf, why?!
#include "_Misc/_Defines.h"

class NodesList
{
public:
    QList<TFNode> m_Nodes;
    NodesList();
    void m_vAddNode(unsigned char Gray, unsigned char Data);
    void m_vAddNode(TFNode node);
    int m_iAddNodeSorted(unsigned char Gray, unsigned char Data);
    int m_iAddNodeSorted(TFNode foo);
    bool m_bChangeNode(unsigned char Gray, unsigned char Data);
    bool m_bChangeNode(TFNode foo);
    bool m_bRemoveNode(int index);
    int m_iFindIndexToGray(unsigned char Gray);
    unsigned char m_ucFindDataToGray(unsigned char Gray);
    void m_vSortNodes();
};

#endif // NODESLIST_H
