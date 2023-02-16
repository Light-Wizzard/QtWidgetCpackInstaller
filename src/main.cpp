// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

// #include "document.h"
#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QLocale>
#include <QTranslator>

#if Q_OS_MSDOS || defined(Q_OS_WIN32) || defined(Q_OS_WINCE)
    #define VERSION "1.0.0"
#else
    #define VERSION "1.0"
#endif
/** ***************************************************************************
 * \brief qMain
 * \param argc
 * \param argv
 * \return
 */
int main(int argc, char *argv[])
{
    // Environment Variables must be set before anything else
    // We need to turn the Sandbox off for Chrome and Qt Web Engine
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--no-sandbox");
    qputenv("QTWEBENGINE_DISABLE_SANDBOX", "1");
    //
    QString theAppName = "QtWidgetCpackInstaller";
    QString theAppDisplayName = "Qt Widget cpack Installer";
    // Load Resource File *.qrc before creating Application
    Q_INIT_RESOURCE(QtWidgetCpackInstaller);
    QApplication theApplication(argc, argv);
    QApplication::setWindowIcon(QIcon(":/images/logo32.png"));
    // Setup the Application for MyOrgSettings
    theApplication.setOrganizationName(theAppName);
    theApplication.setApplicationName(theAppName);
    theApplication.setApplicationDisplayName(theAppDisplayName);
    theApplication.setApplicationVersion(VERSION);
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages)
    {
        QString baseName;
        const QStringList languageName = QLocale(locale).name().split("_");
        /** *******************************************************************
         *  I do not want to make every culture file like en_US, en_AU, en_GB...
         *  yet when it comes to Chinesse you need spite them like zh_CN, zh_TW
        */
        if (languageName.at(0) == "zh") { baseName = QString("%1_%2").arg(theAppName, QLocale(locale).name()); }
        else  { baseName = QString("%1_%2").arg(theAppName, languageName.at(0)); }
        /** *******************************************************************
         * For all languages except Chinese, we only need to look at the first one,
         * unless you want to support more cultures.
         */
        QString theTranslationFile = QString(":/translations/%1").arg(baseName);
        if (translator.load(theTranslationFile))
        {
            theApplication.installTranslator(&translator);
            break;
        }
    } // end for
    MainWindow w;
    QStringList addedFonts = w.addFontsFromResources();
    for ( const QString& font : addedFonts ) { qDebug() << "Added font:" << font; }
    w.show();
    return theApplication.exec();
}
// ***************************** End of File **********************************
