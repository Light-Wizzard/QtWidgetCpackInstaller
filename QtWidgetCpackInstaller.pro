TEMPLATE = app

TARGET = "QtWidgetCpackInstaller"

QT += webenginewidgets webchannel

HEADERS += src/mainwindow.h src/previewpage.h src/document.h src/MySettings.h src/DialogAbout.h

SOURCES = src/main.cpp \
    src/mainwindow.cpp \
    src/previewpage.cpp \
    src/document.cpp src/DialogAbout.cpp src/MySettings.cpp

RESOURCES = QtWidgetCpackInstaller.qrc

# Disable Qt Quick compiler because the example doesn't use QML, but more importantly so that
# the source code of the .js files is not removed from the embedded qrc file.
CONFIG -= qtquickcompiler

FORMS += src/mainwindow.ui src/DialogAbout.ui
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
DISTFILES += resources/3rdparty/MARKDOWN-LICENSE.txt \
    resources/3rdparty/MARKED-LICENSE.txt

# install
target.path = $$[QT_INSTALL_EXAMPLES]/webenginewidgets/markdowneditor
INSTALLS += target
