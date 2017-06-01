Dependencies:
- cmake
- Qt
- opengl

Current test/dev build environment:
- win64
- vs2015 community
- cmake 3.5.2
- qt 5.8.0 (https://download.qt.io/official_releases/qt/5.8/5.8.0/qt-opensource-windows-x86-msvc2015_64-5.8.0.exe)
- windows sdk 8.1

How to build the source:
- (prerequisite installed Qt, cmake and a compiler of your choise)
- download the repo
- open command line inside "_build"
- execute cmake to have makefiles, e.g.: cmake -G "Visual Studio 14 2015 Win64" . (NOTE: if you get errors that cmake cant find qt, you need to configure the CMAKE_PREFIX_PATH for your qt installation)
- run the build: e.g.: cmake --build .
- find the binary in "output\_bin" - hopefully ;)

Running the binary:
- Open a command line inside "output\_bin"
- You need to have the Qt libs in your Path variable (if you dont have, you get an error like, "bla bla... Qt5OpenGLd.dll no found". On windows you can now just add the path tempory, e.g. like this: set Path=%Path%;E:\Qt\Qt5.8.0\5.8\msvc2015_64\bin (attention if you have spaces in YOUR path, you need to add some "" around)
- Now you should be able to just run the binary, e.g.: StereoViewer.exe

Working with the program:
Pleaes have a look at: http://pascalmues.de/StereoViewer/index.php?site=userguide
- Loading volume data (tbd)
- Using the transfer function (tbd)
- Configuring the camera windows (tbd)

Working with the code:
Edit code, open it with notepad++ or use cmake to generate what ever kind of project files you want to have
Changing the GUI stuff, using the QtCreator is quite handy, but a text editor works as well

License:
The sources was pubished on GPL-3.0, you can find the "Lizenz GPL-3.0.pdf" next to this readme

About:
The software was developed as part of my bachelor thesis and following, you can find it all the documents and some design stuff at "http://pascalmues.de/StereoViewer/"