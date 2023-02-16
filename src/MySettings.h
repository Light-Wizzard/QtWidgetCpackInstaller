#ifndef MYSETTINGS_H
#define MYSETTINGS_H

#include <QtCore>
#include <QCoreApplication>
#include <QSettings>
#include <QFileInfo>
#include <QObject>
#include <QTime>
#include <QtGlobal>
#include <QMessageBox>
#include <QTextStream>
#include <QtDebug>
#include <iostream>

/** ***************************************************************************
 * \brief MySettings::MySettings
 */
class MyOrgSettings : public QObject
{
        Q_OBJECT
    public:
        explicit MyOrgSettings(QObject *parent = nullptr);
        /*!
            \enum MyMessageTypes
            \brief Message Types
         */
        enum MyMessageTypes
        {
            Information = 100, //!< \c Information  \brief Information
            Question    = 101, //!< \c Question     \brief Question
            Warning     = 102, //!< \c Warning      \brief Warning
            Critical    = 103, //!< \c Critical     \brief Critical
            Debug       = 104, //!< \c Debug        \brief Debug
        }; // end enum MyFileinfo
        // Makes Messaging easier
        Q_ENUM(MyMessageTypes)
        /*!
            \enum MyFileinfo
            \brief File Information
         */
        enum MyFileinfo
        {
            AbsolutePath      = 100, //!< \c AbsolutePath      \brief The absolute path name consists of the full path and the file name.
            AbsoluteFilePath  = 101, //!< \c CanonicalPath     \brief Returns a file's path absolute path, does not include the file name.
            BaseName          = 102, //!< \c BaseName          \brief Returns the base name of the file without the path.
            BirthTime         = 103, //!< \c BirthTime         \brief Returns the date and time when the file was created / born.
            CanonicalPath     = 104, //!< \c CanonicalPath     \brief Returns the canonical path including the file name, i.e. an absolute path without symbolic links or redundant "." or ".." elements.
            CanonicalFilePath = 105, //!< \c CanonicalFilePath \brief Returns the file's path canonical path (excluding the file name), i.e. an absolute path without symbolic links or redundant "." or ".." elements.
            CompleteBaseName  = 106, //!< \c CompleteBaseName  \brief Returns the complete base name of the file without the path.
            CompleteSuffix    = 107, //!< \c CompleteSuffix    \brief Returns the complete suffix (extension) of the file.
            Directory         = 108, //!< \c Directory         \brief Returns the path of the object's parent directory as a QDir object.
            FileName          = 109, //!< \c FileName          \brief Returns the name of the file, excluding the path.
            FilePath          = 110, //!< \c FilePath          \brief Returns the file name, including the path (which may be absolute or relative).
            IsWritable        = 111, //!< \c IsWritable        \brief Returns true if the user can write to the file; otherwise returns false.
            IsFile            = 112, //!< \c IsFile            \brief Returns true if this object points to a file or to a symbolic link to a file. Returns false if the object points to something which isn't a file, such as a directory.
            IsFolder          = 113, //!< \c IsFolder          \brief Returns true if this object points to a directory or to a symbolic link to a directory; otherwise returns false.
            FileSize          = 114, //!< \c FileSize          \brief File Size
            IsSymLink         = 115, //!< \c IsSymLink         \brief Returns true if this object points to a symbolic link; otherwise returns false.
            SymLinkTarget     = 116, //!< \c SymLinkTarget     \brief Returns the absolute path to the file or directory a symbolic link points to, or an empty string if the object isn't a symbolic link.
        }; // end enum MyFileinfo
        // Makes getting file Info easier
        Q_ENUM(MyFileinfo)

        bool isFileExists(const QString &thisFile);
        QString getIniFullPath();
        void setIniFullPath(const QString &thisIniFileName);
        void setVersion(const QString &thisVersion);
        QString getVersion();
        void setAppName(const QString &thisAppName);
        QString getAppName();
        void setOrgDomain(const QString &thisOrgDomain);
        QString getOrgDomain();
        void setOrgName(const QString &thisOrgName);
        QString getOrgName();
        void setAppDataLocation(const QString &thisAppDataLocation);
        QString getAppDataLocation();
        bool writeFile(const QString &thisFileName, const QString &thisContent);
        QString readFile(const QString &thisFileName);
        bool isAppDataLocationGood(const QString &thisFolder);
        bool isMakeDir(const QString &thisPath);
        bool isFileMake(const QString &thisPath, const QString &thisFileName);
        QString combinePathFileName(const QString &thisPath, const QString &thisFileName);
        bool isPathExists(const QString &thisPath);
        QString getIniFileName();
        void setIniFileName(const QString &thisIniFileName);
        QString getIniFileExtension();
        void setIniFileExtension(const QString &thisIniFileNameExtension);
        QString getFileInfo(MyOrgSettings::MyFileinfo thisInfo, const QString &thisFileFolder);
        QVariant setMessage(const QString &thisTitle, const QString &thisMessage, MyMessageTypes thisMessageType) const;
        QSettings *getSettings();
        void writeSettingsInt(const QString &thisSetting, int thisValue);
        void writeSettings(const QString &thisSetting, const QString &thisValue);
        int readSettingsInt(const QString &thisSetting, int thisDefault);
        bool readSettingsBool(const QString &thisSetting, bool thisDefault);
        QString readSettings(const QString &thisSetting);
        QString readSettings(const QString &thisSetting, const QString &thisDefault);
        bool isSetting(const QString &thisFieldName);
        void writeSettingsBool(const QString &thisSetting, bool thisValue);

        void setWindowState(QByteArray thisWindowState);
        QByteArray getWindowState();
        void setGeometry(QByteArray thisGeometry);
        QByteArray getGeometry();
    private:
        QSettings           *mySettings             = nullptr;      //!< \c mySettings              \brief QSettings is used for this class.
        QString             myIniFullPath           = "";           //!< \c myIniFullPath           \brief my Ini Full File Path and Name.ext
        QString             myIniFileName           = "";           //!< \c myIniFile               \brief my Ini File
        QString             myIniFileExtension      = "";           //!< \c myIniFileExtension      \brief my Ini File Extension
        QString             myOrganizationName      = "";           //!< \c myOrganizationName      \brief Organization Name.
        QString             myOrganizationDomain    = "";           //!< \c myOrganizationDomain    \brief Organization Domain.
        QString             myApplicationName       = "";           //!< \c myApplicationName       \brief Application Name.
        QString             myAppDataLocation       = "";           //!< \c myAppDataLocation       \brief my App Data Location
        QString             myVersion               = "1.0.0";      //!< \c myVersion               \brief Version
        bool                isCreated               = false;        //!< \c isCreated               \brief if file is created
        const QString       MY_INI_EXTENSION        = "ini";        //!< \c MY_INI_EXTENSION        \brief Ini File Extension.
        bool                isDebugMessage          = false;        //!< \c isDebugMessage          \brief Debug Message
        QByteArray          myGeometry;                             //!< \c myGeometryPos           \brief Windows Geometry Pos
        QByteArray          myWindowState;                          //!< \c myGeometrySize          \brief Windows Geometry Size
        bool                myGeometryMax           = false;        //!< \c myGeometryMax           \brief Windows Geometry Max
        bool                myGeometryMin           = false;        //!< \c myGeometryMin           \brief Windows Geometry Min
        const QString       MY_GEOMETRY             = "Geometry";   //!< \c ConstSettingsGeometry   \brief QSettings Field Settings Geometry.
        const QString       MY_WINDOW_STATES        = "WidowState"; //!< \c MY_WINDOW_STATES        \brief QSettings Field Window State.

};
#endif // MYSETTINGS_H
// ***************************** End of File **********************************
