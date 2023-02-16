// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFile>
#include <QFileDialog>
#include <QFontDatabase>
#include <QMessageBox>
#include <QStatusBar>
#include <QTextStream>
#include <QWebChannel>
#include <QPrinter>
#include <QPdfWriter>
#include <QPainter>

#include <QDirIterator>
#include <QStringList>
#include <QFontDatabase>

#include <QtTextToSpeech>

#include <QLoggingCategory>

#include "document.h"
#include "DialogAbout.h"
#include "MySettings.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
/** ***************************************************************************
 *  \brief class MainWindow
 */
class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
        ///
        /// \brief The MyMessageTypes enum
        ///
        enum MyStatusTypes
        {
            Information = 100, //!< \c Information  @brief Information
            Warning     = 101, //!< \c Warning      @brief Warning
            Critical    = 102, //!< \c Critical     @brief Critical
            Debug       = 103, //!< \c Debug        @brief Debug
            Normal      = 104  //!< \c Normal       @brief Normal
        }; // end enum MyMessageTypes
        // Makes Messaging easier
        Q_ENUM(MyStatusTypes)
        // Open File
        void openFile(const QString &path);

        void onTabWidgetCurrentChanged(int index);
        void onRadioButtonMarkDownClicked();
        void onRadioButtonHtmlClicked();
        void onRadioButtonPlainClicked();
        void onPushButtonFindFileClicked();
        void onPushButtonReadFileClicked();
        void onPushButtonIconClicked();
        void speak();
        void stop();
        void setRate(int rate);
        void setPitch(int pitch);
        void setVolume(int volume);
        void stateChanged(QTextToSpeech::State state);
        void engineSelected(int index);
        void languageSelected(int language);
        void voiceSelected(int index);
        void localeChanged(const QLocale &locale);
        void onPushButtonPathClicked();
        void setStatus(const QString &theStatus, MyStatusTypes theStatusType);
        void readSettingsFirst();
        void doOnClosing();
        void readGeometrySettings();
        void writeGeometrySettings();
        void onSpinBoxIconValueChanged(int arg1);
        void loadIcon();
        QStringList addFontsFromResources(const QStringList &filters = {QStringLiteral("*.ttf"),QStringLiteral("*.otf")});

    protected:
        void closeEvent(QCloseEvent *e) override;

    private slots:
        void onFileNew();
        void onFileOpen();
        void onFileSave();
        void onFileSaveAs();
        void onAbout();
        void onHelp();

    private:
        bool isModified() const;
        Ui::MainWindow  *ui;
        DialogAbout     *aboutMe;
        QString         myFilePath;
        Document        myDocumentContent;
        Document        myHelpDocumentContent;
        MyOrgSettings   *mySetting;                         //!< \c mySetting \brief my Setting
        QString         MY_FILE_NAME            = "File";
        QString         MY_FILE_PLAIN           = "Plain";
        QString         MY_FILE_HTML            = "HTML";
        QString         MY_FILE_MARKDOWN        = "MarkDown";
        QString         MY_TAB                  = "TabIndex";
        QString         MY_TITLE                = "Title";
        QString         MY_PATH                 = "Path";
        QString         MY_ICON                 = "Icon";
        const QString   MY_LAST_TAB_INDEX       = "LastTabInex"; //!< \c MY_LAST_TAB          \brief Field Name for Current Last Tab Index.
        const QString   MY_DEFAULT_TAB_INDEX    = "0";           //!< \c MY_DEFAULT_TAB_INDEX  \brief Default Tab Index.
        QTextToSpeech   *mySpeech;
        QList<QVoice>   myVoices;

}; // end class MainWindow
#endif // MAINWINDOW_H
// ***************************** End of File **********************************
