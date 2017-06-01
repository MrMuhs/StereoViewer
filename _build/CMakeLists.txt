cmake_minimum_required (VERSION 3.3)

# 1) Generate make files:
#	- QNX: cmake -G "Unix Makefiles" .
#	- WIN32: cmake -G "Visual Studio 10 2010" .
#            cmake -G "Visual Studio 14 2015" .
#            cmake -G "Visual Studio 14 2015 Win64" .

# 2) Execute the build:
# cmake --build .

project(StereoViewer)

#include(${CMAKE_BINARY_DIR}/DebugCmakeStuff.wtf)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)  

list(APPEND CMAKE_PREFIX_PATH "E:/Qt/Qt5.8.0/5.8/msvc2015_64/lib/cmake")

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Xml REQUIRED)

# according to the Qt doc (http://doc.qt.io/qt-5/cmake-manual.html)
# this shouldnt be needed, but the includes are not found without?!
include_directories(${Qt5Core_INCLUDE_DIRS})
add_definitions(${Qt5Core_DEFINITIONS})

include_directories(${Qt5Widgets_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS})

include_directories(${Qt5OpenGL_INCLUDE_DIRS})
add_definitions(${Qt5OpenGL_DEFINITIONS})

include_directories(${Qt5Xml_INCLUDE_DIRS})
add_definitions(${Qt5Xml_DEFINITIONS})

#add_definitions(-DSV_USE_RAYTRACING)
add_definitions(-DSV_USE_SLICING)

#output directories
#set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
#set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

#set(OUTPUT_CMAKE_FOLDER ${CMAKE_BINARY_DIR}/../output)
set(OUTPUT_FOLDER ${CMAKE_BINARY_DIR}/../output)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_FOLDER}/_lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_FOLDER}/_lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_FOLDER}/_lib)
#Message(CMAKE_LIBRARY_OUTPUT_DIRECTORY=${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_FOLDER}/_bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_FOLDER}/_bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_FOLDER}/_bin)
#Message(CMAKE_RUNTIME_OUTPUT_DIRECTORY=${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_FOLDER}/_lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_FOLDER}/_lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_FOLDER}/_lib)
#Message(CMAKE_ARCHIVE_OUTPUT_DIRECTORY=${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

set(EXECUTABLE_OUTPUT_PATH ${OUTPUT_FOLDER}/_bin)
set(LIBRARY_OUTPUT_PATH ${OUTPUT_FOLDER}/_lib)

link_directories(${OUTPUT_FOLDER}/_lib)

qt5_wrap_ui(DataPreProcessingDialog_ui_moc "../gui/DataPreProcessingDialog.ui")
qt5_wrap_ui(OpenImgStackDialog_ui_moc "../gui/OpenImgStackDialog.ui")
qt5_wrap_ui(OpenVgiVolFilesDialog_ui_moc "../gui/OpenVgiVolFilesDialog.ui")
qt5_wrap_ui(ProcessInfo_ui_moc "../gui/ProcessInfo.ui")
qt5_wrap_ui(SaveTransferfunctionDialog_ui_moc "../gui/SaveTransferfunctionDialog.ui")
qt5_wrap_ui(MedianFilterDialog_ui_moc "../gui/MedianFilterDialog.ui")
qt5_wrap_ui(UnsharpMaskingDialog_ui_moc "../gui/UnsharpMaskingDialog.ui")
qt5_wrap_ui(MainWindow_ui_moc "../gui/MainWindow.ui")
qt5_wrap_ui(MultiWidget_ui_moc "../gui/MultiWidget.ui")
qt5_wrap_ui(VolumeWidget_ui_moc "../gui/VolumeWidget.ui")
qt5_wrap_ui(Infomation_ui_moc "../gui/Infomation.ui")
qt5_wrap_ui(SettingsDialog_ui_moc "../gui/SettingsDialog.ui")
qt5_wrap_ui(EditNodeAlphaDialog_ui_moc "../gui/EditNodeAlphaDialog.ui")
qt5_wrap_ui(EditNodeColorDialog_ui_moc "../gui/EditNodeColorDialog.ui")

file(GLOB allCodeFilesUI
	"../gui/*.h"
	"../gui/*.cpp"
)

set(allTheUiStuff
	${allCodeFilesUI}
	${DataPreProcessingDialog_ui_moc}
	${OpenImgStackDialog_ui_moc}
	${OpenVgiVolFilesDialog_ui_moc}
	${ProcessInfo_ui_moc}
	${SaveTransferfunctionDialog_ui_moc}
	${MedianFilterDialog_ui_moc}
	${UnsharpMaskingDialog_ui_moc}
    ${MainWindow_ui_moc}
	${MultiWidget_ui_moc}
	${VolumeWidget_ui_moc}
	${Infomation_ui_moc}
	${SettingsDialog_ui_moc}
	${EditNodeAlphaDialog_ui_moc}
	${EditNodeColorDialog_ui_moc}
)

# ${CMAKE_CURRENT_BINARY_DIR} is needed to include the "ui_..." files generated for the ui files
include_directories("../" "../" ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB allCodeFiles
	${allTheUiStuff}
	"../main.cpp"
    "../_DataFileIO/*.h"
	"../_DataFileIO/*.cpp"
	"../_DataFileIO/ImagePreprocessing/*.h"
	"../_DataFileIO/ImagePreprocessing/*.cpp"
	"../_DataFileIO/ImagePreprocessing/ConfigurationDialogs/*.h"
	"../_DataFileIO/ImagePreprocessing/ConfigurationDialogs/*.cpp"
	"../_Misc/*.h"
	"../_Misc/*.cpp"
	"../_OpenGL/*.h"
	"../_OpenGL/*.cpp"
	"../_Volume/*.h"
	"../_Volume/*.cpp"
	"../_Volume/TransferFunction/*.h"
	"../_Volume/TransferFunction/*.cpp"
	"../_Volume/VolumeData/*.h"
	"../_Volume/VolumeData/*.cpp"
)

add_executable(${PROJECT_NAME} ${allCodeFiles} ${allCodeFilesUI})
#add_dependencies(${PROJECT_NAME} duktape)
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets Qt5::OpenGL Qt5::Xml opengl32)

# Move the shaders to the output directory as well
file(GLOB files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/../" "../*.glsl")
foreach(aa ${files})
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/../${aa}" "${EXECUTABLE_OUTPUT_PATH}/${aa}" COPYONLY)
endforeach()

# just for convenience of having them in the project files, like vs, create a seperate project for the shader files
file(GLOB allShaderFiles
	"../*.glsl"
)
add_library(StereoViewerShaders ${allShaderFiles})