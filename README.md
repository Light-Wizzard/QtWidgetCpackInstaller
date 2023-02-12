# QtWidgetCpackInstaller

This is an example application of a normal Qt Widget Application that can be to deployed using [cmake](https://cmake.org/), [cpack](https://cmake.org/cmake/help/book/mastering-cmake/chapter/Packaging%20With%20CPack.html), [WindeployQT](https://doc.qt.io/qt-6/windows-deployment.html) and [NSIS](https://nsis.sourceforge.io/Download).

I could not find a good example of a Deployable Qt Widget using C++, and cmake, using cpack, and NSIS, 
which is the Null Soft Installer, which I perfer due to how easy it is to have the Installer, 
uninstall the old version, before installing the new one.

Table of Content

* [Development](#Development)
* [cmake](#cmake)
* [WinDeployQt](#WinDeployQt)
* [MSVC](#MSVC)
* [MinGW](#MinGW)
* [Progress](#Progress)
* [](#)
* [](#)

## Development
I am using Windows 10 Pro, on an old XEON with 10 Cores or 20 Threads, and 32 GB of RAM, 
with a NVIDIA 3060 with 12 GB of VRAM.

I will be using the latest Qt Creator installed using the online installer, with everything selected. 

You have to adjust the path statements below to fit your enviroment, I use F drive, 
and and work under the root folder workspace, I create a folder in a folder with QtWidgetCpackInstaller,
so that the build folder is outside the Source, but inside the project folder, 
this gives me a root with all folders in it.

Once you have compiled your application in Release mode, you will need to run cpack.
The crash course on cpack is that it needs a lot of varibles to be set up correctly,
then you must call it from the command line, you can use any termianl, 
but I only use the ones that Qt provides, so I will use MSVC and MinGW, 
note that the version numbers of Qt change, so use the path that Qt Creator uses.

```
# MinGW
F:
cd F:\workspace\QtWidgetCpackInstaller\QtWidgetCpackInstaller\build-MyQtWidgetApp-Desktop_Qt_6_4_2_MinGW_64_bit-Release

# or for MSVC

F:
cd F:\workspace\QtWidgetCpackInstaller\QtWidgetCpackInstaller\build-MyQtWidgetApp-Desktop_Qt_6_4_2_MSVC2019_64bit-Release
```

Once inside the root of your project where your CMakeLists.txt is, run cpack, 
and I am not sure what switches are required, you can call it with none and it seems to work the same.

```
E:\Qt\Tools\CMake_64\bin\cpack.exe -C Release -G NSIS

```

I use [Doxygen](https://www.doxygen.nl/)

```
# To Generate or update Doxy Files:
cd F:\workspace\QtWidgetCpackInstaller\QtWidgetCpackInstaller
"C:\Program Files\doxygen\bin\doxygen" Doxyfile
```

## cmake

You need to manually set the Project Name and version

```
# Define Project and Version
project(QtWidgetCpackInstaller VERSION 0.1 LANGUAGES CXX)

# Set Short Description Name
set(MY_DESCRIPTION "Qt Widget cpack Installer")

# Set GitHub PUBLISHER Name
set(MY_PUBLISHER "Light-Wizzard")
set(MY_PROJECT "QtWidgetCpackInstaller")
```

Using the Variables MY_PUBLISHER and MY_PROJECT, that come from your GitHub account, 
if you do not use GitHub, you will have to change the folling lines:

```
# You will need to create all the Wiki Pages that the links point to
set(APP_URL "https://github.com/${MY_PUBLISHER}/${MY_PROJECT}")
set(APP_SUPPORT "https://github.com/${MY_PUBLISHER}/${MY_PROJECT}/wiki/Support")
set(APP_CONTACT "https://github.com/${MY_PUBLISHER}/${MY_PROJECT}/wiki/Contact")
set(APP_EMAIL "https://github.com/${MY_PUBLISHER}/${MY_PROJECT}/wiki/Email")
```

I use cmake Variables to find which OS I am compiling for like this:

```
if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
    set(OSS_WINDOWS TRUE)
elseif(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux|CYGWIN.*|MSYS.*|^GNU$|Android")
    set(OSS_LINUX TRUE)
elseif(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Darwin")
    set(OSS_MAC TRUE)
endif()
```

This allows me to work on the three major OS types: Windows, Linux, and Mac.

I am only working in Windows, so the other OS selections need to be set up,
but without switching machines, I cannot set them up,
and I do not own a Mac, and therefore, cannot set up that selection.

If you are working with these OSs, please file an issue, and post your code, 
and I will update this code to reflect known good values.

## WinDeployQt

I found this GitHub Project nitroshare, https://github.com/nitroshare/nitroshare-desktop/, 
and I modified their cmake that runs WinDeployQt

## MSVC

There are two options in Qt, MSVC or MinGW, if you use TTS, it will not work in MSVC, 
and I have not found out why.

## MinGW

There are two options in Qt, MSVC or MinGW, if you use TTS, it will work in MinGW, 
if you run it from within Qt Creator, but fails to run due to dynamic linking,
and I have not found out why.

## Progress

MSVC works for some Qt modules, TTS is one I have not been able to get working, 
and to make matters worse, I cannot get MinGW working in the deplyment.

### End of Document
