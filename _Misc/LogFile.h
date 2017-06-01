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

#ifndef LOGFILE_H
#define LOGFILE_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDate>
#include <QDir>
#include "_Misc/_Defines.h"

class LogFile
{
public:
    static LogFile* m_LogFileGetInstanz();
    QString m_sGetLogFilename();
    LogMessageTyp m_iGetLogFileMode();
    bool m_bSetLogFilename(QString file);
    bool m_bSetLogFileMode(LogMessageTyp typ);
    bool m_bAppendMessage(LogMessageTyp typ, QString msg, LogStackMovment stack = FuncStayAt);

private:
    LogFile();
    static LogFile *SingleTonInstanz;
    LogMessageTyp m_iLogMode;
    QString m_sLogFile;
    QTextStream m_LogFileStream;
    int m_iStackCounter;
    static const QString cm_sStackStringPrePart;
};

#endif // LOGFILE_H
