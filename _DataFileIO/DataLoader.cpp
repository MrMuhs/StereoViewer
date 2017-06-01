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

#include "DataLoader.h"

DataLoader::DataLoader(iDataFile* ToLoad)
{
    m_TargetToLoad = ToLoad;
    m_Result = 0;
    m_ProcessInfo = new ProcessInfo();
    QObject::connect(m_ProcessInfo, SIGNAL(m_vUserClickAbort()), this, SLOT(m_vIfUserClickedAbort()));
}

DataLoader::~DataLoader()
{
    if(m_ProcessInfo != 0)
    {
        QObject::disconnect(m_ProcessInfo, SIGNAL(m_vUserClickAbort()), this, SLOT(m_vIfUserClickedAbort()));
        delete m_ProcessInfo;
    }
    delete m_TargetToLoad;
}

void DataLoader::m_vStartLoading()
{
    this->start();
}

void DataLoader::run()
{
    this->setTerminationEnabled(false);
    QMetaObject::invokeMethod(m_ProcessInfo, "show", Qt::QueuedConnection);
    this->m_Result = this->m_TargetToLoad->m_GetTexArray(m_ProcessInfo);
    QMetaObject::invokeMethod(m_ProcessInfo, "close", Qt::QueuedConnection);
}

void DataLoader::m_vIfUserClickedAbort()
{
    //while(isRunning() == true) wait(500);
    //m_ProcessInfo->hide();
    QObject::disconnect(m_ProcessInfo, SIGNAL(m_vUserClickAbort()), this, SLOT(m_vIfUserClickedAbort()));
    // first tried to terminate() thread here, but this causes deadlock caus qt uses son mutex stuff intern?!
    // abort is now handled by signal/slot form the parameter m_ProcessInfo, given to the m_GetTexArray(..)
    // each sub class of the decorator pattern has to stop his reading/calculatin/... function on its own
    // and has to return 0
    emit terminate();
}
