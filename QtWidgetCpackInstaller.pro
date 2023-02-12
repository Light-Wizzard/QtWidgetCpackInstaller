# QtWidgetCpackInstaller.pro
# Specifies the name of the template to use when generating the project.
# The allowed values are: app, lib, subdirs, aux, vcapp or vclib
TEMPLATE = "app"
# This pro(ject) file is based on a pattern
# Do not use Spaces in Target Name
TARGET = "QtWidgetCpackInstaller"
#
QT += core gui widgets
#
#CONFIG *= strict_c++
#CONFIG *= "c++11"
#CONFIG *= "c++14"
#CONFIG *= "c++1z"
 CONFIG *= "c++17"
#CONFIG *= "c++2a"
#CONFIG *= "c++latest"
#
#win32:CONFIG       += static
#win32:DEFINES      += STATIC
#win32:QMAKE_LFLAGS += -static
#
#win32:CONFIG    *= windeployqt windows
#win32:QMAKE_CC  += "E:\Qt\Tools\$env:MY_QT_TOOLS_MINGW64\bin\gcc.exe"
#win32:QMAKE_CXX += "E:\Qt\Tools\$env:MY_QT_TOOLS_MINGW64\bin\g++.exe"
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
# Specifies the name of the target file. Contains the base name of the project file by default.
##-------------------------------------------------------------------------------------
INCLUDEPATH     += .
INCLUDEPATH     += src
INCLUDEPATH     += "${QT_INSTALL_LIBS}/qt"
##-------------------------------------------------------------------------------------
#
HEADERS +=  src/mainwindow.h src/DialogAbout.h
SOURCES +=  src/main.cpp src/mainwindow.cpp src/DialogAbout.cpp
#
FORMS += src/mainwindow.ui src/DialogAbout.ui
#
INCLUDEPATH += $$PWD
#
# Resources
RESOURCES   += QtWidgetCpackInstaller.qrc
#
# Translations
#
CONFIG += localize_deployment
SYMBIAN_SUPPORTED_LANGUAGES += zh_CN
SYMBIAN_LANG.zh_CN = 31
CONFIG += lrelease
CONFIG += embed_translations
LRELEASE_DIR=./translations
QM_FILES_RESOURCE_PREFIX=./translations
# ar,de,en,es,fr,it,ja,no,ru,sv,zh_CN
TRANSLATIONS += translations/QtWidgetCpackInstaller_ar.ts \
                translations/QtWidgetCpackInstaller_de.ts \
                translations/QtWidgetCpackInstaller_en.ts \
                translations/QtWidgetCpackInstaller_es.ts \
                translations/QtWidgetCpackInstaller_fr.ts \
                translations/QtWidgetCpackInstaller_it.ts \
                translations/QtWidgetCpackInstaller_ja.ts \
                translations/QtWidgetCpackInstaller_no.ts \
                translations/QtWidgetCpackInstaller_ru.ts \
                translations/QtWidgetCpackInstaller_sv.ts \
                translations/QtWidgetCpackInstaller_zh_CN.ts
#
###############################################################################
#
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
#DEFINES += QT_DEPRECATED_WARNINGS
#
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#
#
macos:QMAKE_INFO_PLIST = macos/Info.plist
ios:QMAKE_INFO_PLIST   = ios/Info.plist
#-------------------------------------------------------------------------------------
#contains(QMAKE_HOST.arch, x86_64) { #x64
#    BUILD_ARCH = x64
#} else { #x32
#    BUILD_ARCH += x32
#}
#CONFIG(release, debug|release) {
#    BUILD_TYPE = release
#} else {
#    BUILD_TYPE = debug
#}
#GENF_ROOT   = $${BUILD_ARCH}/_output
#BIN_OUTPUT  = $${GENF_ROOT}/_bin
#DESTDIR     = $${BIN_OUTPUT}/$${BUILD_TYPE}
#OBJECTS_DIR = $${GENF_ROOT}/$${TARGET}/$${BUILD_TYPE}/_build
#MOC_DIR     = $${GENF_ROOT}/$${TARGET}/$${BUILD_TYPE}/_moc
#UI_DIR      = $${GENF_ROOT}/$${TARGET}/$${BUILD_TYPE}/_ui
#RCC_DIR     = $${GENF_ROOT}/$${TARGET}/$${BUILD_TYPE}/_rc
#-------------------------------------------------------------------------------------
#
#Release:DESTDIR     = release
#Release:OBJECTS_DIR = release/.obj
#Release:MOC_DIR     = release/.moc
#Release:RCC_DIR     = release/.rcc
#Release:UI_DIR      = release/.ui

#Debug:DESTDIR     = debug
#Debug:OBJECTS_DIR = debug/.obj
#Debug:MOC_DIR     = debug/.moc
#Debug:RCC_DIR     = debug/.rcc
#Debug:UI_DIR      = debug/.ui


win32-g++{
    contains(QMAKE_HOST.arch, x86_64) { #x64
        DEFINES += MINGW_X64
    } else { #x32
        DEFINES += MINGW_X32
    }

    CONFIG(release, debug|release) {
        #release
        QMAKE_CXXFLAGS += -std=c++0x -O2 -Os -msse2 -ffp-contract=fast -fpic
    }
    else {
        #debug
        DEFINES += _DEBUG
        QMAKE_CXXFLAGS += -std=c++0x -O0 -g3 -msse2 -fpic
    }
}

#
unix {
    isEmpty(PREFIX) {
        PREFIX = /usr
    }
    target.path         = "$${PREFIX}/bin"
    shortcutfiles.files = "usr/share/applications/$${TARGET}.desktop"
    shortcutfiles.path  = usr/share/applications
    data.files         += "usr/share/icons/hicolor/48x48/apps/$${TARGET}.png"
    data.path           = usr/share/pixmaps
    INSTALLS           += shortcutfiles
    INSTALLS           += data
    INSTALLS           += target
    macx {
        RC_FILE = macos/Icon.icns
    }
} else {
    isEmpty(PREFIX) {
        PREFIX = /usr
    }
    target.path         = "$${PREFIX}/bin"
    shortcutfiles.files = "usr/share/applications/$${TARGET}.desktop"
    shortcutfiles.path  = usr/share/applications
    data.files         += "usr/share/icons/hicolor/48x48/apps/$${TARGET}.png"
    data.path           = usr/share/pixmaps
    INSTALLS           += shortcutfiles
    INSTALLS           += data
    INSTALLS           += target
}
################################ End of File ##################################
