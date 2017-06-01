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

#ifdef SV_USE_RAYTRACING
	#ifndef _WIN32
		#warning Volume drawing by use of SV_USE_RAYTRACING
	#else
		#pragma message ("Volume drawing by use of SV_USE_RAYTRACING")
	#endif
#elif SV_USE_SLICING
	#ifndef _WIN32
		#warning Volume drawing by use of SV_USE_SLICING
	#else
		#pragma message ("Volume drawing by use of SV_USE_SLICING")
	#endif
#else
    #error No volume drawing methode defined! Insert DEFINE += SV_USE_SLICING. Possibile options are SV_USE_RAYTRACING and SV_USE_SLICING.
#endif

#include <QApplication>
#include "gui/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    int ret = a.exec();
    return ret;
}
