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

#include "GlobalSettings.h"

GlobalSettings* GlobalSettings::SingleTonInstanz = 0;

GlobalSettings::GlobalSettings()
{
    m_LogFile = LogFile::m_LogFileGetInstanz();

    // Define valid fiels with default values here
    m_ValidDefaultFields.insert("LogFileMode", QString::number(Error));

    m_sStandardConfigPath = QDir::currentPath() + "/Config.xml";
    if(!m_bLoadFromXML(m_sStandardConfigPath))
    {
        m_bSaveToXML(m_sStandardConfigPath, true);
        if(!m_bLoadFromXML(m_sStandardConfigPath))
        {
            m_LogFile->m_bAppendMessage(Error, "(GlobalSettings::GlobalSettings) Can't write/read default configuration");
        }
    }

    m_LogFile->m_bSetLogFileMode((LogMessageTyp)m_sGetField("LogFileMode").toInt());
}

GlobalSettings* GlobalSettings::m_GlobalSettingsGetInstanz()
{
    if(SingleTonInstanz == 0)
        SingleTonInstanz = new GlobalSettings();
    return SingleTonInstanz;
}

QString GlobalSettings::m_sGetField(QString fieldname)
{
    if(m_bValidFieldName(fieldname))
    {
        if(!m_FieldsList.contains(fieldname))
            m_FieldsList.insert(fieldname, m_sGetDefaultValue(fieldname));

        return m_FieldsList.value(fieldname);
    }
    else
    {
        // Logfile Error !?
        return "";
    }
}

bool GlobalSettings::m_bSetField(QString fieldname, QString value)
{
    if(m_bValidFieldName(fieldname))
    {
        m_FieldsList.insert(fieldname, value);
        return true;
    }
    else
    {
        // Logfile Error !?
        return false;
    }
}

bool GlobalSettings::m_bLoadFromXML(QString filename)
{
    if(filename.isEmpty())
    {
        m_LogFile->m_bAppendMessage(Error, "(GlobalSettings::m_bLoadFromXML) filename.isEmpty()");
        return false;
    }

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        m_LogFile->m_bAppendMessage(Error, "(GlobalSettings::m_bLoadFromXML) Can't open file for reading");
        return false;
    }

    QDomDocument xmldoc;
    QString errorStr;
    int errorLine;
    int errorColumn;
    if (!xmldoc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
    {
        m_LogFile->m_bAppendMessage(Error, "(GlobalSettings::m_bLoadFromXML) Can't read this file as XML content(" +
                                    errorStr + " Line: " + QString::number(errorLine) + " Col: " + QString::number(errorColumn) + ")");
        file.close();
        return false;
    }
    file.close();

    QDomElement root = xmldoc.documentElement();
    if (root.tagName() != "Configuration")
    {
        m_LogFile->m_bAppendMessage(Error, "(GlobalSettings::m_bLoadFromXML) This is not a Configuration XML-File");
        return false;
    }
    else
    {
        QDomNode n = root.firstChild();
        m_FieldsList.clear();
        while(!n.isNull())
        {
            QDomElement e = n.toElement();
            if(!e.isNull())
            {
                if(m_bValidFieldName(e.tagName()))
                {
                    m_bSetField(e.tagName(), e.attribute("Data"));
                    m_LogFile->m_bAppendMessage(Load, "(GlobalSettings::m_bLoadFromXML) Reading Configuration-Field: " + e.tagName() + " = " + e.attribute("Data"));
                }
                else
                {
                    m_LogFile->m_bAppendMessage(Error, "(GlobalSettings::m_bLoadFromXML) Unknown field " + e.tagName() + " in Configuration(Line:" + QString::number(e.lineNumber()) + " Col:" + QString::number(e.columnNumber()) + ")");
                }
            }
            else
            {
                m_LogFile->m_bAppendMessage(Error, "(GlobalSettings::m_bLoadFromXML) Can't convert node to a element(Line:" + QString::number(n.lineNumber()) + " Col:" + QString::number(n.columnNumber()) + ")");
            }
            n = n.nextSibling();
        }

        if(!m_ValidDefaultFields.count() == m_FieldsList.count())
        {
//            int ret = QMessageBox::critical(0, "StereoViewer",
//                                            "Fieldcount form file does not equal with program parametercount",
//                                            QMessageBox::Close | QMessageBox::RestoreDefaults,
//                                            QMessageBox::RestoreDefaults);
//            if(ret == QMessageBox::RestoreDefaults)
            if(true)
            {
                QFile file(filename);
                int cnt = 0;
                if (!file.open(QIODevice::ReadOnly))
                {
                    while(QFile::exists(filename + ".backup" + QString::number(cnt)))
                        cnt++;
                    file.copy(filename + ".backup" + QString::number(cnt));
                }
                m_bSaveToXML(filename, true);
                QMessageBox::information(0, "StereoViewer", "Old file has been saved to " + filename + ".backup" + QString::number(cnt) + " and is overwritten with defaults");
                return m_bLoadFromXML(filename);
            }
            else
                return false;
            m_LogFile->m_bAppendMessage(Error, "(GlobalSettings::m_bLoadFromXML) Fieldcount form file does not equal with program defaults parametercount");
        }

        m_LogFile->m_bAppendMessage(Load, "(GlobalSettings::m_bLoadFromXML) Reading Configuration-XML-File .. ok");
        return true;
    }
}

bool GlobalSettings::m_bSaveToXML(QString filename, bool writedefaults)
{
    if (!filename.isEmpty())
    {
        QDomDocument xmldoc("StereoViewerConfiguration");

        // Add root node
        QDomElement rootnode = xmldoc.createElement("Configuration");
        xmldoc.appendChild(rootnode);

        if(writedefaults == false)
        {
            QMapIterator<QString, QString> i(m_FieldsList);
            while(i.hasNext())
            {
                i.next();
                QDomElement node = xmldoc.createElement(i.key());
                rootnode.appendChild(node);
                node.setAttribute("Data", i.value());
            }
        }
        else
        {
            QMapIterator<QString, QString> i(m_ValidDefaultFields);
            while(i.hasNext())
            {
                i.next();
                QDomElement node = xmldoc.createElement(i.key());
                rootnode.appendChild(node);
                node.setAttribute("Data", i.value());
            }
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

                m_LogFile->m_bAppendMessage(Load, "(GlobalSettings::m_bSaveToXML) Saving GlobalSettings-XML-File .. ok");
                return true;
            }
            else
            {
                m_LogFile->m_bAppendMessage(Error, "(GlobalSettings::m_bSaveToXML) Can't open File(Filename=" + filename + ")");
                return false;
            }
        }
        else
        {
            m_LogFile->m_bAppendMessage(Error, "(GlobalSettings::m_bSaveToXML) Can't generate XML-String");
            return false;
        }
    }
    else
    {
        m_LogFile->m_bAppendMessage(Error, "(GlobalSettings::m_bSaveToXML) Filename is null");
        return false;
    }
}

bool GlobalSettings::m_bValidFieldName(QString fieldname)
{
    return m_ValidDefaultFields.contains(fieldname);
}

bool GlobalSettings::m_bSetToDefaults()
{
    m_FieldsList = m_ValidDefaultFields;
    return true;
}

QString GlobalSettings::m_sGetDefaultValue(QString fieldname)
{
    if(m_bValidFieldName(fieldname))
    {
        return m_ValidDefaultFields.value(fieldname);
    }
    else
    {
        return "INVALID FIELDNAME";
    }
}
