#include "MySettings.h"

MyOrgSettings::MyOrgSettings(QObject *parent) : QObject{parent}
{
    if (!isFileExists(getIniFullPath()))
    {
        if (!isFileMake(getAppDataLocation(), QString("%1.%2").arg(getIniFileName(), getIniFileExtension())))
        {
            setMessage("get Settings", QString("%1: %2").arg(QObject::tr("Failed to make File in getSettings"), getIniFullPath()), Critical);
        }
    }
    mySettings = new QSettings(getIniFullPath(), QSettings::IniFormat);

}
/** ***************************************************************************
 * \brief set Message.
 * setMessage
*/
QVariant MyOrgSettings::setMessage(const QString &thisTitle, const QString &thisMessage, MyMessageTypes thisMessageType) const
{
    switch (thisMessageType)
    {
        case Debug:
        {
            if (isDebugMessage)
            {
                #ifdef QT_DEBUG
                qDebug() << thisMessage;
                #else
                std::cout << thisMessage.toStdString() << std::endl;
                #endif
            }
            return "";
        }
        case Information:
        {
            return QMessageBox::information(nullptr, thisTitle, thisMessage, QMessageBox::Ok);
        }
        case Question:
        {
            QMessageBox theMsgBox;
            theMsgBox.setWindowTitle(QObject::tr(thisTitle.toLocal8Bit()));
            theMsgBox.setText(QObject::tr(thisMessage.toLocal8Bit()));
            theMsgBox.setStandardButtons(QMessageBox::Yes);
            theMsgBox.addButton(QMessageBox::No);
            theMsgBox.setDefaultButton(QMessageBox::No);
            if(theMsgBox.exec() == QMessageBox::Yes)
            {
                setMessage("", QObject::tr("Yes was clicked"), Debug);
                return true;
            }
            else
            {
                setMessage("", QObject::tr("Yes was not clicked"), Debug);
                return false;
            }
        }
        case Warning:
        {
            return QMessageBox::warning(nullptr, thisTitle, thisMessage, QMessageBox::Ok);
        }
        case Critical:
        {
            return QMessageBox::critical(nullptr, thisTitle, thisMessage, QMessageBox::Ok);
        }
    } // end switch (thisMessageType)
    return QVariant();
} // end setMessage
/** ***************************************************************************
 * \brief is File Exists.
 * isFileExists
*/
bool MyOrgSettings::isFileExists(const QString &thisFile)
{
    // check if file exists and if yes: Is it really a file and not directory?
    return QFileInfo::exists(thisFile) && QFileInfo(thisFile).isFile();
} // end isFileExists
/** ***************************************************************************
 * \brief set Ini Full Path.
 * getIniFullPath
*/
QString MyOrgSettings::getIniFullPath()
{
    if (myIniFullPath.isEmpty())
    {
        // organizationName, organizationDomain, applicationName and applicationName
        // are set in main.cpp, and passed into Constuctor, so they are set
        QCoreApplication::setOrganizationName(getOrgName());
        QCoreApplication::setOrganizationDomain(getOrgDomain());
        QCoreApplication::setApplicationName(getAppName());
        //QCoreApplication::setApplicationDisplayName(getAppName());
        // see *.pro file where it is: DEFINES *= APP_VERSION=$${VERSION}
        // cmake does not have this yet, this is a hack till I FIXME
        QCoreApplication::setApplicationVersion(getVersion());
        setIniFullPath(QString("%1%2%3.%4").arg(getAppDataLocation(), QDir::separator(), getIniFileName(), getIniFileExtension()));
    }
    return myIniFullPath;
}
/** ***************************************************************************
 * \brief get Ini File Extension.
 * getIniFileExtension
*/
QString MyOrgSettings::getIniFileExtension()
{
    if (myIniFileExtension.isEmpty())
    { setIniFileExtension(MY_INI_EXTENSION); }
    return myIniFileExtension;
}
/** ***************************************************************************
 * \brief set Ini File Extension.
 * setIniFileExtension
*/
void MyOrgSettings::setIniFileExtension(const QString &thisIniFileNameExtension)
{
    if (QString::compare(myIniFileExtension, thisIniFileNameExtension, Qt::CaseInsensitive))
    {
        myIniFileExtension = thisIniFileNameExtension;
    }
}
/** ***************************************************************************
 * \brief set Ini Full Path.
 * setIniFullPath
*/
void MyOrgSettings::setIniFullPath(const QString &thisIniFileName)
{
    if (QString::compare(myIniFullPath, thisIniFileName, Qt::CaseInsensitive))
    {
        myIniFullPath = thisIniFileName;
    }
}
/** ***************************************************************************
 * \brief read File.
 * readFile
*/
QString MyOrgSettings::readFile(const QString &thisFileName)
{
    QFile file(thisFileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        return "";
    }
    QTextStream in(&file);
    QString thisText = in.readAll();
    file.close();
    return thisText;
}
/** ***************************************************************************
 * \brief write File.
 * @param thisFileName QString File Name
 * @param thisContent  QString Content
 * writeFile
*/
bool MyOrgSettings::writeFile(const QString &thisFileName, const QString &thisContent)
{
    QFile theFile(thisFileName);
    // Trying to open in WriteOnly and Text mode and Truncate file if contents exists
    if(!theFile.open(QFile::WriteOnly | QFile::Text | QIODevice::Truncate))
    {
        return false;
    }
    // Write to file
    QTextStream theFileStream(&theFile);
    theFileStream << thisContent;
    theFile.flush();
    theFile.close();
    return true;
} // end writeFile
/** ***************************************************************************
 * \brief is App Data Location Good.
 * isAppDataLocationGood
*/
bool MyOrgSettings::isAppDataLocationGood(const QString &thisFolder)
{
    bool isGood = true;
    if (isFileMake(thisFolder, "ReadMe.txt"))
    {
        if (getFileInfo(IsWritable, combinePathFileName(thisFolder, "ReadMe.txt")) == "false")
        { isGood = false; }
    }
    else
    { isGood = false; }
    if (getFileInfo(IsFolder, thisFolder) == "false")
    { isGood = false; }
    return isGood;
} // end isAppDataLocationGood
/** ***************************************************************************
 * \brief get File Info.
 * getFileInfo
*/
QString MyOrgSettings::getFileInfo(MyOrgSettings::MyFileinfo thisInfo, const QString &thisFileFolder)
{
    if (thisFileFolder.isEmpty()) { return ""; }
    //
    QFileInfo theFileInfo(thisFileFolder);
    //
    switch (thisInfo)
    {
        case AbsoluteFilePath:
            // The absolute path name consists of the full path and the file name.
            return theFileInfo.absoluteFilePath();
        case AbsolutePath:
            // Returns a file's path absolute path. This doesn't include the file name..
            return theFileInfo.absolutePath();
        case BaseName:
            // Returns the base name of the file without the path.
            return theFileInfo.baseName();
        case BirthTime:
            // Returns the date and time when the file was created / born.
            return theFileInfo.birthTime().toString();
        case CanonicalPath:
            // Returns the file's path canonical path (excluding the file name), i.e. an absolute path without symbolic links or redundant "." or ".." elements.
            return theFileInfo.canonicalPath();
        case CanonicalFilePath:
            // Returns the canonical path including the file name, i.e. an absolute path without symbolic links or redundant "." or ".." elements.
            return theFileInfo.canonicalFilePath();
        case CompleteBaseName:
            // Returns the complete base name of the file without the path.
            // "/tmp/archive.tar.gz" == "archive.tar"
            return theFileInfo.completeBaseName();
        case CompleteSuffix:
            // Returns the complete suffix (extension) of the file.
            // "/tmp/archive.tar.gz" == "tar.gz"
            return theFileInfo.completeSuffix();
        case Directory:
            // Returns the path of the object's parent directory as a QDir object.
            return theFileInfo.dir().currentPath();
        case FileName:
            // Returns the name of the file, excluding the path. getFileInfo(FileName, "")
            return theFileInfo.fileName();
        case FilePath:
            // Returns the file name, including the path (which may be absolute or relative).
            return theFileInfo.filePath();
        case IsWritable:
            // Returns true if the user can write to the file; otherwise returns false.
            if (theFileInfo.isWritable()) return "true"; else return "false";
        case FileSize:
            return QString::number(theFileInfo.size());
        case IsFile:
            if (isFileExists(thisFileFolder)) return "true"; else return "false";
        case IsFolder:
            if (isPathExists(thisFileFolder)) return "true"; else return "false";
        case IsSymLink:
            if (theFileInfo.isSymLink()) return "true"; else return "false";
        case SymLinkTarget:
            if (theFileInfo.isSymLink()) return theFileInfo.symLinkTarget(); else return "";
    }
    return "";
}
/** ***************************************************************************
 * \brief is File Make.
 * isFileMake
*/
bool MyOrgSettings::isFileMake(const QString &thisPath, const QString &thisFileName)
{
    isCreated = false;
    QString thePath = combinePathFileName(thisPath, thisFileName);
    if (!isMakeDir(thisPath)) return false;
    if (!isFileExists(thePath))
    {
        QFile theFile(thePath);
        if (theFile.open(QIODevice::WriteOnly))
        {
            /* Point a QTextStream object at the file */
            QTextStream outStream(&theFile);
            /* Write the line to the file */
            outStream << QString("%1 %2").arg(QObject::tr("This file was created to test if this folder is writeable by"), QCoreApplication::applicationName()).toLocal8Bit();
            /* Close the file */
            theFile.close();
            if (isFileExists(thePath)) { isCreated = true; }
            return isCreated;
        }
        else return false;
    }
    return true;
} // end isFileMake
/** ***************************************************************************
 * \brief combine Path File Name.
 * combinePathFileName
*/
QString MyOrgSettings::combinePathFileName(const QString &thisPath,const  QString &thisFileName)
{
    return QString("%1%2%3").arg(thisPath, QDir::separator(), thisFileName);
} // end combinePathFileName
/** ***************************************************************************
 * \brief is Path Exists.
 * isPathExists
*/
bool MyOrgSettings::isPathExists(const QString &thisPath)
{
    return QDir(thisPath).exists() && QFileInfo(thisPath).isDir();
} // end isPathExists
/** ***************************************************************************
 * \brief is Make Dir.
 * isMakeDir
*/
bool MyOrgSettings::isMakeDir(const QString &thisPath)
{
    QDir dir(thisPath);
    if (!dir.exists())
    {
        if (dir.mkpath(".")) { return true; }
    }
    return isPathExists(thisPath);
} // end isMakeDir
/** ***************************************************************************
 * \brief get App Data Location.
 * getAppDataLocation
*/
QString MyOrgSettings::getAppDataLocation()
{
    if (myAppDataLocation.isEmpty())
    {
        QString theAppDataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        if (theAppDataLocation.isEmpty())
        {
            theAppDataLocation = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
            if (theAppDataLocation.isEmpty())
            { theAppDataLocation = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation); }
            if (theAppDataLocation.isEmpty())
            { theAppDataLocation = QDir::currentPath(); }
        }
        if (!isAppDataLocationGood(theAppDataLocation))
        {
            theAppDataLocation = QDir::currentPath(); // FIXME
        }
        setAppDataLocation(theAppDataLocation);
    }
    return myAppDataLocation;
} // end getAppDataLocation
/** ***************************************************************************
 * \brief set App Data Location.
 * setAppDataLocation
*/
void MyOrgSettings::setAppDataLocation(const QString &thisAppDataLocation)
{
    if (QString::compare(myAppDataLocation, thisAppDataLocation, Qt::CaseInsensitive))
    {
        myAppDataLocation = thisAppDataLocation;
    }
} // end setAppDataLocation
/** ***************************************************************************
 * \brief This value is used for Qt Settings: GitHub Account Name is one example.
 * getOrgName
*/
QString MyOrgSettings::getOrgName()
{
    if (myOrganizationName.isEmpty()) { setOrgName(qApp->organizationName()); }
    return myOrganizationName;
} // end getOrgName
/** ***************************************************************************
 * \brief set Org Name.
 * setOrgName
*/
void MyOrgSettings::setOrgName(const QString &thisOrgName)
{
    if (QString::compare(myOrganizationName, thisOrgName, Qt::CaseInsensitive))
    {
        myOrganizationName = thisOrgName;
    }
} // end setOrgName
/** ***************************************************************************
 * \brief get Org Domain.
 * getOrgDomain
*/
QString MyOrgSettings::getOrgDomain()
{
    if (myOrganizationDomain.isEmpty()) { setOrgDomain(qApp->organizationDomain()); }
    return myOrganizationDomain;
} // end getOrgDomain
/** ***************************************************************************
 * \brief set Org Domain.
 * setOrgDomain
*/
void MyOrgSettings::setOrgDomain(const QString &thisOrgDomain)
{
    if (QString::compare(myOrganizationDomain, thisOrgDomain, Qt::CaseInsensitive))
    {
        myOrganizationDomain = thisOrgDomain;
    }
} // end setOrgDomain
/** ***************************************************************************
 * \brief get App Name.
 * getAppName
*/
QString MyOrgSettings::getAppName()
{
    if (myApplicationName.isEmpty()) { setAppName(qApp->applicationName()); }
    return myApplicationName;
} // end getAppName
/** ***************************************************************************
 * \brief set App Name.
 * setAppName
*/
void MyOrgSettings::setAppName(const QString &thisAppName)
{
    if (QString::compare(myApplicationName, thisAppName, Qt::CaseInsensitive))
    {
        myApplicationName = thisAppName;
    }
} // end setAppName
/** ***************************************************************************
 * \brief get Version is set in header as a default.
 * getVersion
*/
QString MyOrgSettings::getVersion()
{
    return myVersion;
}
/** ***************************************************************************
 * \brief set Version.
 * setVersion
 * theLanguageModel->mySetting->setVersion(VERSION);
*/
void MyOrgSettings::setVersion(const QString &thisVersion)
{
    if (QString::compare(myVersion, thisVersion, Qt::CaseInsensitive))
    {
        myVersion = thisVersion;
    }
}
/** ***************************************************************************
 * \brief get Ini File Name.
 * getIniFileName
*/
QString MyOrgSettings::getIniFileName()
{
    if (myIniFileName.isEmpty())
    { setIniFileName(getAppName()); }
    return myIniFileName;
}
/** ***************************************************************************
 * \brief set Ini File Name.
 * setIniFileName
*/
void MyOrgSettings::setIniFileName(const QString &thisIniFileName)
{
    if (QString::compare(myIniFileName, thisIniFileName, Qt::CaseInsensitive))
    {
        myIniFileName = thisIniFileName;
    }
}
/** ***************************************************************************
 * \brief get Settings.
 * getSettings
*/
QSettings *MyOrgSettings::getSettings()
{
    setMessage("get Settings", "getSettings", Debug);
    if (!isFileExists(getIniFullPath()))
    {
        if (!isFileMake(getAppDataLocation(), QString("%1.%2").arg(getIniFileName(), getIniFileExtension())))
        {
            setMessage("get Settings", QString("%1: %2").arg(QObject::tr("Failed to make File in getSettings"), getIniFullPath()), Critical);
        }
    }
    return new QSettings(getIniFullPath(), QSettings::IniFormat);
} // end qSettingsInstance
/** ***************************************************************************
 * \brief is Setting.
 * isSetting
*/
bool MyOrgSettings::isSetting(const QString &thisFieldName)
{
    setMessage("is Setting", QString("isSetting(%1)").arg(thisFieldName), Debug);
    return mySettings->contains(thisFieldName);
} // end isSetting
/** ***************************************************************************
 * \brief read Settings.
 * readSettings
*/
QString MyOrgSettings::readSettings(const QString &thisSetting, const QString &thisDefault)
{
    setMessage("read Settings", QString("readSettings(%1, %2)").arg(thisSetting, thisDefault), Debug);
    mySettings->beginGroup("General");
    QString theSetting = mySettings->value(thisSetting, thisDefault).toString();
    mySettings->endGroup();
    return theSetting;
}
/** ***************************************************************************
 * \brief read Settings.
 * readSettings
*/
QString MyOrgSettings::readSettings(const QString &thisSetting)
{
    setMessage("read Settings", QString("readSettings(%1)").arg(thisSetting), Debug);
    mySettings->beginGroup("General");
    QString theSetting = mySettings->value(thisSetting).toString();
    mySettings->endGroup();
    return theSetting;
}
/** ***************************************************************************
 * \brief read Settings Bool.
 * readSettingsBool
*/
bool MyOrgSettings::readSettingsBool(const QString &thisSetting, bool thisDefault)
{
    setMessage("read Settings Bool", QString("readSettingsBool(%1, %2)").arg(thisSetting, thisDefault), Debug);
    mySettings->beginGroup("General");
    bool theSetting = mySettings->value(thisSetting, thisDefault).toBool();
    mySettings->endGroup();
    return theSetting;
}
/** ***************************************************************************
 * \brief read Settings Int.
 * readSettingsInt
*/
int MyOrgSettings::readSettingsInt(const QString &thisSetting, int thisDefault)
{
    setMessage("read Settings Int", QString("readSettingsInt(%1, %2)").arg(thisSetting, thisDefault), Debug);
    mySettings->beginGroup("General");
    int theSetting = mySettings->value(thisSetting, thisDefault).toInt();
    mySettings->endGroup();
    return theSetting;
}
/** ***************************************************************************
 * \brief write Settings.
 * writeSettings
*/
void MyOrgSettings::writeSettings(const QString &thisSetting, const QString &thisValue)
{
    setMessage("write Settings", QString("writeSettings(%1, %2)").arg(thisSetting, thisValue), Debug);
    mySettings->beginGroup("General");
    mySettings->setValue(thisSetting, thisValue);
    mySettings->endGroup();
}
/** ***************************************************************************
 * \brief write Settings.
 * writeSettings
*/
void MyOrgSettings::writeSettingsInt(const QString &thisSetting, int thisValue)
{
    setMessage("write Settings Int", QString("writeSettingsInt(%1, %2)").arg(thisSetting, thisValue), Debug);
    mySettings->beginGroup("General");
    mySettings->setValue(thisSetting, thisValue);
    mySettings->endGroup();
}
/** ***************************************************************************
 * \brief write Settings bool.
 * writeSettingsBool
*/
void MyOrgSettings::writeSettingsBool(const QString &thisSetting, bool thisValue)
{
    setMessage("write Settings Int", QString("writeSettingsInt(%1, %2)").arg(thisSetting, thisValue), Debug);
    mySettings->beginGroup("General");
    mySettings->setValue(thisSetting, thisValue);
    mySettings->endGroup();
}
/** ***************************************************************************
 * \brief set Geometry.
 * setGeometry
*/
void MyOrgSettings::setGeometry(QByteArray thisGeometry)
{
    setMessage("set Geometry", "setGeometry", Debug);
    mySettings->beginGroup("Geometry");
    mySettings->setValue(MY_GEOMETRY, thisGeometry);
    mySettings->endGroup();
} // end setGeometry
/** ***************************************************************************
 * \brief get Geometry.
 * getGeometry
*/
QByteArray MyOrgSettings::getGeometry()
{
    setMessage("get Geometry", "getGeometry", Debug);
    // Geometry
    mySettings->beginGroup("Geometry");
    myGeometry = mySettings->value(MY_GEOMETRY).toByteArray();
    mySettings->endGroup();
    return myGeometry;
} // end getGeometry
/** ***************************************************************************
 * \brief set Window State.
 * setWindowState
*/
void MyOrgSettings::setWindowState(QByteArray thisWindowState)
{
    setMessage("set Window State", "setWindowState", Debug);
    mySettings->beginGroup("Geometry");
    mySettings->setValue(MY_WINDOW_STATES, thisWindowState);
    mySettings->endGroup();
}
/** ***************************************************************************
 * \brief get Window State.
 * getWindowState
*/
QByteArray MyOrgSettings::getWindowState()
{
    setMessage("get Window State", "getWindowState", Debug);
    // Window State
    mySettings->beginGroup("Geometry");
    myWindowState = mySettings->value(MY_WINDOW_STATES).toByteArray();
    mySettings->endGroup();
    return myWindowState;
}
// ***************************** End of File **********************************
