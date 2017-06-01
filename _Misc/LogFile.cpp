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

#include "_Misc/LogFile.h"

LogFile* LogFile::SingleTonInstanz = 0;
const QString LogFile::cm_sStackStringPrePart = QString("  ");

LogFile* LogFile::m_LogFileGetInstanz()
{
    if( SingleTonInstanz == 0 )
        SingleTonInstanz = new LogFile();
    return SingleTonInstanz;
}

LogFile::LogFile()
{
    m_bSetLogFilename(QDir::currentPath() + "/LogFile.txt");
    m_iLogMode = Warning;
    m_iStackCounter = 0;
}

QString LogFile::m_sGetLogFilename()
{
    return m_sLogFile;
}

LogMessageTyp LogFile::m_iGetLogFileMode()
{
    return m_iLogMode;
}

bool LogFile::m_bSetLogFilename(QString file)
{
    if(!file.isEmpty())
    {
        m_sLogFile = file;
        return true;
    }
    else
        return false;
}

bool LogFile::m_bSetLogFileMode(LogMessageTyp typ)
{
    m_iLogMode = typ;
    return true;
}

bool LogFile::m_bAppendMessage(LogMessageTyp typ, QString msg, LogStackMovment stack)
{
    if (!m_sLogFile.isEmpty())
    {
        QDateTime date = QDateTime::currentDateTime();

        QFile MyFile(m_sLogFile);
        if(MyFile.open(QIODevice::WriteOnly | QIODevice::Append))
        {
            QTextStream stream(&MyFile);
            if(typ >= m_iLogMode)
            {
                // process stack counter
                // to show the functioncall stack in the log file, setup tabs or spacechars infornt of the msg.
                // @TODO: check and maybe change EVERY function call in the program, this isnt done complete. functions which can abort with an error and return false/ect. should not use the counter to avoid errors when counting -> HANDLE THIS WITH CARE!!!!!
                QString l_sStackPreString = "";
                if(stack == FuncStayAt)
                {
                    // @TODO: maybe do it with symbols like + / - / | or arrows from asci-table ?!
                    for(int i = 0; i < m_iStackCounter; i++) l_sStackPreString = l_sStackPreString + cm_sStackStringPrePart;
                }
                else if(stack == FuncUp)
                {
                    for(int i = 0; i < m_iStackCounter; i++) l_sStackPreString = l_sStackPreString + cm_sStackStringPrePart;
                    m_iStackCounter = m_iStackCounter + 1;
                }
                else if(stack == FuncDown)
                {
                    m_iStackCounter = m_iStackCounter - 1;
                    for(int i = 0; i < m_iStackCounter; i++) l_sStackPreString = l_sStackPreString + cm_sStackStringPrePart;
                }

                // put the messageline tougether
                switch(typ)
                {
                    case Error:
                        stream << date.toString("dd.MM.yyyy hh:mm:ss") << " ERROR:   " << l_sStackPreString << msg << EndLine;
                        break;
                    case Warning:
                        stream << date.toString("dd.MM.yyyy hh:mm:ss") << " WARNUNG: " << l_sStackPreString << msg << EndLine;
                        break;
                    case Debug:
                        stream << date.toString("dd.MM.yyyy hh:mm:ss") << " DEBUG:   " << l_sStackPreString << msg << EndLine;
                        break;
                    case Load:
                        stream << date.toString("dd.MM.yyyy hh:mm:ss") << " LOADING: " << l_sStackPreString << msg << EndLine;
                        break;
                    default:
                        return false;
                }
                MyFile.close();
                return true;
            }
            else
                return false;
        }
        else
            return false;
    }
    else
        return false;
}
