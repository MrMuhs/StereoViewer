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

#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H

#include <QString>
#include <QMap>
#include <QDir>
#include <QDomDocument>
#include <QMessageBox>
#include <QTextStream>
#include "_Misc/_Defines.h"
#include "_Misc/LogFile.h"

class GlobalSettings
{
public:
    static GlobalSettings* m_GlobalSettingsGetInstanz();
    QString m_sGetField(QString fieldname);
    bool m_bSetField(QString fieldname, QString value);
    bool m_bLoadFromXML(QString filename);
    bool m_bSaveToXML(QString filename = QDir::currentPath() + "/Config.xml", bool writedefaults = false);
    bool m_bSetToDefaults();

private:
    LogFile* m_LogFile;
    static GlobalSettings* SingleTonInstanz;
    QMap<QString, QString> m_ValidDefaultFields;
    QMap<QString, QString> m_FieldsList;
    QString m_sStandardConfigPath;

    GlobalSettings(); // this is a singleton class, constructor is private, access just by m_GlobalSettingsGetInstanz
    bool m_bValidFieldName(QString fieldname);
    QString m_sGetDefaultValue(QString fieldname);
};

#endif // GLOBALSETTINGS_H

