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

#include "_Volume/TransferFunction/NodesList.h"

NodesList::NodesList()
{
}

void NodesList::m_vAddNode(unsigned char Gray, unsigned char Data)
{
    TFNode x; x.Gray = Gray; x.Data = Data;
    this->m_vAddNode(x);
}

void NodesList::m_vAddNode(TFNode node)
{
    m_Nodes.append(node);
}

int NodesList::m_iAddNodeSorted(unsigned char Gray, unsigned char Data)
{
    TFNode x; x.Gray = Gray; x.Data = Data;
    return this->m_iAddNodeSorted(x);
}

int NodesList::m_iAddNodeSorted(TFNode node)
{
    // TODO: Sortierung ändern das nur ein Platz gesucht wird und nicht alles neu sortiert wird
    int index;
    m_Nodes.append(node);
    this->m_vSortNodes();
    index = this->m_iFindIndexToGray(node.Gray);
    return index;
}

bool NodesList::m_bChangeNode(unsigned char Gray, unsigned char Data)
{
    int index = this->m_iFindIndexToGray(Gray);
    if(index >= 0)
    {
        m_Nodes[index].Data = Data;
        return true;
    }
    else
        return false;
}

bool NodesList::m_bChangeNode(TFNode node)
{
    return this->m_bChangeNode(node.Gray, node.Data);
}

bool NodesList::m_bRemoveNode(int index)
{
    if(m_Nodes.count() < index) return false;
    m_Nodes.removeAt(index);
    return true;
}

int NodesList::m_iFindIndexToGray(unsigned char Gray)
{
    for(int x = 0; x < m_Nodes.count(); x++)
    {
        if(m_Nodes.at(x).Gray == Gray) return x;
    }
    return -1;
}

unsigned char NodesList::m_ucFindDataToGray(unsigned char Gray)
{
    for(int x = 0; x < m_Nodes.count(); x++)
    {
        if(m_Nodes.at(x).Gray == Gray) return m_Nodes.at(x).Data;
    }
    return false;
}

void NodesList::m_vSortNodes()
{
    // Bubble-Sorting the List
    int i, j;
    TFNode temp;
    for (i = (m_Nodes.count() - 1); i > 0; i--)
    {
        for (j = 1; j <= i; j++)
        {
            if (m_Nodes[j-1].Gray > m_Nodes[j].Gray)
            {
                temp = m_Nodes[j-1];
                m_Nodes[j-1] = m_Nodes[j];
                m_Nodes[j] = temp;
            }
        }
    }
}
