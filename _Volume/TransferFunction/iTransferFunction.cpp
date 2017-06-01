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

#include "_Volume/TransferFunction/iTransferFunction.h"

iTransferFunction::iTransferFunction()
{
    m_LogFile = LogFile::m_LogFileGetInstanz();
}

bool iTransferFunction::m_bGenerateTFFromXMLFile(QString filename)
{
    if(filename.isEmpty())
    {
        m_LogFile->m_bAppendMessage(Error, "(iTransferFunction::m_bGenerateTFFromXMLFile) filename.isEmpty()");
        return false;
    }

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        m_LogFile->m_bAppendMessage(Error, "(iTransferFunction::m_bGenerateTFFromXMLFile) Can't open file for reading");
        return false;
    }
    m_sFilename = filename;

    QDomDocument xmldoc;
    QString errorStr;
    int errorLine;
    int errorColumn;
    if (!xmldoc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
    {
        m_LogFile->m_bAppendMessage(Error, "(iTransferFunction::m_bGenerateTFFromXMLFile) Can't read this file as XML content(" +
                                    errorStr + " Line: " + QString::number(errorLine) + " Col: " + QString::number(errorColumn) + ")");
        file.close();
        return false;
    }
    file.close();

    QDomElement root = xmldoc.documentElement();
    if (root.tagName() != "Transferfunction")
    {
        m_LogFile->m_bAppendMessage(Error, "(iTransferFunction::m_bGenerateTFFromXMLFile) This is not a Transferfunction XML-File");
        return false;
    }

    QDomNode n = root.firstChild();
    QString sFileVersion = "";
    while(!n.isNull())
    {
        QDomElement e = n.toElement();
        if(!e.isNull())
        {
            if(e.tagName() == "Header")
            {
                QDomNode subnode1 = e.firstChild();
                while(!subnode1.isNull())
                {
                    QDomElement subele1 = subnode1.toElement();
                    if(!subele1.isNull())
                    {
                        if(subele1.tagName() == "Fileversion")
                        {
                            sFileVersion = subele1.attribute("Version");
                            m_LogFile->m_bAppendMessage(Debug, "(iTransferFunction::m_bGenerateTFFromXMLFile) Detected Transferfunction fileversion: " + sFileVersion);
                        }
                        else if(subele1.tagName() == "Description")
                        {
                            m_sDescription = subele1.attribute("Text");
                        }
                        else if(subele1.tagName() == "Savedate")
                        {
                            m_LogFile->m_bAppendMessage(Debug, "(iTransferFunction::m_bGenerateTFFromXMLFile) Detected Transferfunction saveddate: " + subele1.attribute("Timestamp"));
                        }
                        else
                        {
                            m_LogFile->m_bAppendMessage(Error, "(iTransferFunction::m_bGenerateTFFromXMLFile) Unknown field in Header-Node(Line:" + QString::number(subele1.lineNumber()) + " Col:" + QString::number(subele1.columnNumber()) + ")");
                        }
                    }
                    else
                    {
                        m_LogFile->m_bAppendMessage(Error, "(iTransferFunction::m_bGenerateTFFromXMLFile) Can't convert node to a element in Header(Line:" + QString::number(subnode1.lineNumber()) + " Col:" + QString::number(subnode1.columnNumber()) + ")");
                    }
                    subnode1 = subnode1.nextSibling();
                }
            }
            else if(e.tagName() == "Alphanodes")
            {
                m_bAlphaNodeClear();
                if(sFileVersion == "1.0")
                {
                    QDomNode subnode1 = e.firstChild();
                    while(!subnode1.isNull())
                    {
                        QDomElement subele1 = subnode1.toElement();
                        if(!subele1.isNull())
                        {
                            if(subele1.tagName() == "Alphanode")
                            {
                                if(m_bAlphaNodeAdd((unsigned char)subele1.attribute("Gray").toInt(), (unsigned char)subele1.attribute("Alpha").toInt()))
                                    m_LogFile->m_bAppendMessage(Debug, "(iTransferFunction::m_bGenerateTFFromXMLFile) Adding Alphanode Gray= " + subele1.attribute("Gray") + " Alpha=" + subele1.attribute("Alpha") + " .. ok");
                                else
                                    m_LogFile->m_bAppendMessage(Error, "(iTransferFunction::m_bGenerateTFFromXMLFile) Can't add Alphanode Gray= " + subele1.attribute("Gray") + " Alpha=" + subele1.attribute("Alpha"));
                            }
                            else
                            {
                                m_LogFile->m_bAppendMessage(Error, "(iTransferFunction::m_bGenerateTFFromXMLFile) Converted node is not an Alphanode(Line:" + QString::number(subele1.lineNumber()) + " Col:" + QString::number(subele1.columnNumber()) + ")");
                            }
                        }
                        else
                        {
                            m_LogFile->m_bAppendMessage(Error, "(iTransferFunction::m_bGenerateTFFromXMLFile) Can't convert node to a element in Alphanodes(Line:" + QString::number(subele1.lineNumber()) + " Col:" + QString::number(subele1.columnNumber()) + ")");
                        }
                        subnode1 = subnode1.nextSibling();
                    }
                }
                else
                {
                    m_LogFile->m_bAppendMessage(Error, "(iTransferFunction::m_bGenerateTFFromXMLFile) Can't read Alphanodes unknown Fileversion");
                }
            }
            else if(e.tagName() == "Colornodes")
            {
                m_bColorNodeClear();
                if(sFileVersion == "1.0")
                {
                    QDomNode subnode1 = e.firstChild();
                    while(!subnode1.isNull())
                    {
                        QDomElement subele1 = subnode1.toElement();
                        if(!subele1.isNull())
                        {
                            if(subele1.tagName() == "Colornode")
                            {
                                if(m_bColorNodeAdd((unsigned char)subele1.attribute("Gray").toInt(), (unsigned char)subele1.attribute("Red").toInt(), (unsigned char)subele1.attribute("Green").toInt(), (unsigned char)subele1.attribute("Blue").toInt()))
                                    m_LogFile->m_bAppendMessage(Debug, "(iTransferFunction::m_bGenerateTFFromXMLFile) Adding Colornode Gray= " + subele1.attribute("Gray") + " Red=" + subele1.attribute("Red") + " Green=" + subele1.attribute("Green") + " Blue=" + subele1.attribute("Blue") + " .. ok");
                                else
                                    m_LogFile->m_bAppendMessage(Error, "(iTransferFunction::m_bGenerateTFFromXMLFile) Can't add Colornode Gray= " + subele1.attribute("Gray") + " Red=" + subele1.attribute("Red") + " Green=" + subele1.attribute("Green") + " Blue=" + subele1.attribute("Blue"));
                            }
                            else
                            {
                                m_LogFile->m_bAppendMessage(Error, "(iTransferFunction::m_bGenerateTFFromXMLFile) Converted node is not an Colornode(Line:" + QString::number(subele1.lineNumber()) + " Col:" + QString::number(subele1.columnNumber()) + ")");
                            }
                        }
                        else
                        {
                            m_LogFile->m_bAppendMessage(Error, "(iTransferFunction::m_bGenerateTFFromXMLFile) Can't convert node to a element in Colornodes(Line:" + QString::number(subele1.lineNumber()) + " Col:" + QString::number(subele1.columnNumber()) + ")");
                        }
                        subnode1 = subnode1.nextSibling();
                    }
                }
                else
                {
                    m_LogFile->m_bAppendMessage(Error, "(iTransferFunction::m_bGenerateTFFromXMLFile) Can't read Colornodes unknown Fileversion");
                }
            }
            else
            {
                m_LogFile->m_bAppendMessage(Error, "(iTransferFunction::m_bGenerateTFFromXMLFile) Unknown node in XML-File(Line:" + QString::number(n.lineNumber()) + " Col:" + QString::number(n.columnNumber()) + ")");
            }
        }
        else
        {
            m_LogFile->m_bAppendMessage(Error, "(iTransferFunction::m_bGenerateTFFromXMLFile) Can't convert node to a element(Line:" + QString::number(n.lineNumber()) + " Col:" + QString::number(n.columnNumber()) + ")");
        }
        n = n.nextSibling();
    }

    m_bBuildColorTableTexture();
    emit m_vTransferfunctionLoaded();
    m_LogFile->m_bAppendMessage(Debug, "(iTransferFunction::m_bGenerateTFFromXMLFile) Reading Transferfunction-XML-File .. ok");
    return true;
}

bool iTransferFunction::m_bGenerateTFToXMLFile(QString filename, QString description)
{
    if (!filename.isEmpty())
    {
        m_sFilename = filename;
        m_sDescription = description;

        QDomDocument xmldoc("Transferfunction");

        // Add root node
        QDomElement rootnode = xmldoc.createElement("Transferfunction");
        xmldoc.appendChild(rootnode);

        // Add Header-Infos
        QDomElement header = xmldoc.createElement("Header");
        rootnode.appendChild(header);

        QDomElement node = xmldoc.createElement("Fileversion");
        header.appendChild(node);
        node.setAttribute("Version", "1.0");

        node = xmldoc.createElement("Description");
        header.appendChild(node);
        node.setAttribute("Text", description);

        node = xmldoc.createElement("Savedate");
        header.appendChild(node);
        QDateTime date = QDateTime::currentDateTime();
        node.setAttribute("Timestamp", date.toString("dd.MM.yyyy hh:mm:ss"));

        // Add Alphanodes to XML
        QDomElement alphanodes = xmldoc.createElement("Alphanodes");
        rootnode.appendChild(alphanodes);
        QList<TFNode> nodesalpha = m_GetAllAlphaNodes().m_Nodes;
        for(int i=0; i<nodesalpha.count(); i++)
        {
            QDomElement node = xmldoc.createElement("Alphanode");
            alphanodes.appendChild(node);
            node.setAttribute("Gray", nodesalpha.at(i).Gray);
            node.setAttribute("Alpha", nodesalpha.at(i).Data);
        }

        // Add Colornodes to XML
        QDomElement colornodes = xmldoc.createElement("Colornodes");
        rootnode.appendChild(colornodes);
        const NodesList* nodescolor = m_GetAllColorNodes();
        for(int i = 0; i < nodescolor[0].m_Nodes.count(); i++)
        {
            QDomElement node = xmldoc.createElement("Colornode");
            colornodes.appendChild(node);
            node.setAttribute("Gray", nodescolor[0].m_Nodes[i].Gray);
            node.setAttribute("Red", nodescolor[0].m_Nodes[i].Data);
            node.setAttribute("Green", nodescolor[1].m_Nodes[i].Data);
            node.setAttribute("Blue", nodescolor[2].m_Nodes[i].Data);
        }

        // Save to File
        QString sxml = xmldoc.toString();

        if(!sxml.isEmpty())
        {
            QFile MyFile(filename);
            if(MyFile.open(QIODevice::WriteOnly))
            {
                QTextStream stream(&MyFile);
                const int IndentSize = 4;
                xmldoc.save(stream, IndentSize);
                //stream << sxml;
                MyFile.close();

                m_LogFile->m_bAppendMessage(Debug, "(iTransferFunction::m_bGenerateTFToXMLFile) Saving Transferfunction-XML-File .. ok");
                return true;
            }
            else
            {
                m_LogFile->m_bAppendMessage(Error, "(iTransferFunction::m_bGenerateTFToXMLFile) Can't open File(Filename=" + filename + ")");
                return false;
            }
        }
        else
        {
            m_LogFile->m_bAppendMessage(Error, "(iTransferFunction::m_bGenerateTFToXMLFile) Can't generate XML-String");
            return false;
        }
    }
    else
    {
        m_LogFile->m_bAppendMessage(Error, "(iTransferFunction::m_bGenerateTFToXMLFile) Filename is null");
        return false;
    }
}

QString iTransferFunction::m_sGetFilename()
{
    return m_sFilename;
}

QString iTransferFunction::m_sGetDescription()
{
    return m_sDescription;
}
