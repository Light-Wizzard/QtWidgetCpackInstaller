/** ***************************************************************************
 * \brief This is an example Application to show how to use cpack, WinDeployQt, and NSIS
 * \authors Qt Example Code, and Jeffrey Scott Flesher for Light-Wizzard
 * \copyright Copyright (C) 2016 The Qt Company Ltd.
 * SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
 */

#include "mainwindow.h"
#include "previewpage.h"
#include "DialogAbout.h"
#include "ui_mainwindow.h"

/** ***************************************************************************
 *  \brief MainWindow::MainWindow
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), mySpeech(nullptr)
{
    // Set up UI
    ui->setupUi(this);
    // Set up Settings
    mySetting = new MyOrgSettings(qApp);
    QLoggingCategory::setFilterRules(QStringLiteral("qt.speech.tts=true \n qt.speech.tts.*=true"));
    // Set Font we imported
    ui->plainTextEditor->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    ui->preview->setContextMenuPolicy(Qt::NoContextMenu);

    PreviewPage *page = new PreviewPage(this);
    ui->preview->setPage(page);

    connect(ui->plainTextEditor, &QPlainTextEdit::textChanged, this, [this]() { myDocumentContent.setText(ui->plainTextEditor->toPlainText()); });

    QWebChannel *channel = new QWebChannel(this);
    channel->registerObject(QStringLiteral("content"), &myDocumentContent);
    page->setWebChannel(channel);

    ui->preview->setUrl(QUrl("qrc:/resources/index.html"));

    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::onFileNew);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onFileOpen);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onFileSave);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::onFileSaveAs);
    connect(ui->actionExit, &QAction::triggered, this, &QWidget::close);
    connect(ui->plainTextEditor->document(), &QTextDocument::modificationChanged, ui->actionSave, &QAction::setEnabled);
    //
    // Push Button
    connect(ui->pushButtonFindFile, &QPushButton::clicked, this, &MainWindow::onPushButtonFindFileClicked);
    connect(ui->pushButtonReadFile, &QPushButton::clicked, this, &MainWindow::onPushButtonReadFileClicked);
    connect(ui->pushButtonIcon, &QPushButton::clicked, this, &MainWindow::onPushButtonIconClicked);
    connect(ui->pushButtonPath, &QPushButton::clicked, this, &MainWindow::onPushButtonPathClicked);
    // Radio Button
    connect(ui->radioButtonSettingsPlain, &QRadioButton::clicked, this, &MainWindow::onRadioButtonPlainClicked);
    connect(ui->radioButtonSettingsHTML, &QRadioButton::clicked, this, &MainWindow::onRadioButtonHtmlClicked);
    connect(ui->radioButtonSettingsMarkDown, &QRadioButton::clicked, this, &MainWindow::onRadioButtonMarkDownClicked);
    // Tab
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabWidgetCurrentChanged);
    // Spin
    connect(ui->spinBoxIcon, &QSpinBox::valueChanged, this, &MainWindow::onSpinBoxIconValueChanged);
    //
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onAbout);
    connect(ui->actionHelp, &QAction::triggered, this, &MainWindow::onHelp);
    // Read Default MarkDown file
    QFile defaultTextFile(":/resources/default.md");
    defaultTextFile.open(QIODevice::ReadOnly);
    ui->plainTextEditor->setPlainText(defaultTextFile.readAll());
    // Help Page
    PreviewPage *pageHelp = new PreviewPage(this);
    ui->webEngineViewHelp->setPage(pageHelp);
    // Help Channel
    QWebChannel *helpChannel = new QWebChannel(this);
    myHelpDocumentContent.setText(mySetting->readFile(":/README.md"));
    helpChannel->registerObject(QStringLiteral("content"), &myHelpDocumentContent);
    pageHelp->setWebChannel(helpChannel);
    ui->webEngineViewHelp->setUrl(QUrl("qrc:/resources/index.html"));
    //
    readSettingsFirst();
    //
    // Populate engine selection list
    ui->engine->addItem("Default", QString("default"));
    const auto engines = QTextToSpeech::availableEngines();
    for (const QString &engine : engines)
    {
        ui->engine->addItem(engine, engine);
    }
    ui->engine->setCurrentIndex(0);
    engineSelected(0);
    //
    connect(ui->speakButton, &QPushButton::clicked, this, &MainWindow::speak);
    connect(ui->pitch, &QSlider::valueChanged, this, &MainWindow::setPitch);
    connect(ui->rate, &QSlider::valueChanged, this, &MainWindow::setRate);
    connect(ui->volume, &QSlider::valueChanged, this, &MainWindow::setVolume);
    connect(ui->engine, &QComboBox::currentIndexChanged, this, &MainWindow::engineSelected);

}
/** ***************************************************************************
 *  \brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete ui;
}
/** ***************************************************************************
 *  \brief MainWindow::closeEvent
 *  \param e
 */
void MainWindow::closeEvent(QCloseEvent *e)
{
    doOnClosing();
    if (isModified())
    {
        QMessageBox::StandardButton button = QMessageBox::question(this, windowTitle(), tr("You have unsaved changes. Do you want to exit anyway?"));
        if (button != QMessageBox::Yes) { e->ignore(); }
    }
}
/** ***************************************************************************
 * \brief MainWindow::doOnClosing
 */
void MainWindow::doOnClosing()
{
    mySetting->writeSettings(MY_TITLE, ui->lineEditTitle->text());
    mySetting->writeSettings(MY_FILE_NAME, ui->lineEditFileName->text());

    mySetting->writeSettings(MY_PATH, ui->lineEditPath->text());
    mySetting->writeSettings(MY_ICON, ui->lineEditIcon->text());
    // Write Geometry on exit
    mySetting->setGeometry(saveGeometry());
    mySetting->setWindowState(saveState());
    // Last Tab
    mySetting->writeSettingsInt(MY_LAST_TAB_INDEX, ui->tabWidget->currentIndex());
    writeGeometrySettings();
}
/** ***************************************************************************
 * \brief MainWindow::readSettingsFirst
 */
void MainWindow::readSettingsFirst()
{
    ui->lineEditFileName->setText(mySetting->readSettings(MY_FILE_NAME, ""));
    if (mySetting->readSettingsBool(MY_FILE_MARKDOWN, true))
    {
        ui->radioButtonSettingsMarkDown->setChecked(true);
    }
    if (mySetting->readSettingsBool(MY_FILE_HTML, true))
    {
        ui->radioButtonSettingsHTML->setChecked(true);
    }
    if (mySetting->readSettingsBool(MY_FILE_PLAIN, true))
    {
        ui->radioButtonSettingsPlain->setChecked(true);
    }
    ui->lineEditTitle->setText(mySetting->readSettings(MY_TITLE, ""));
    ui->lineEditPath->setText(mySetting->readSettings(MY_PATH, ""));
    ui->lineEditIcon->setText(mySetting->readSettings(MY_ICON, ""));
    loadIcon();
    // Go to Tab
    int theIndex = mySetting->readSettingsInt(MY_LAST_TAB_INDEX, MY_DEFAULT_TAB_INDEX.toInt());
    if (theIndex < 0) { theIndex = 0; }
    ui->tabWidget->setCurrentIndex(theIndex);

    /*
    restoreGeometry(mySetting->getGeometry());

    const QByteArray geometry = mySetting->getGeometry();
    if (geometry.isEmpty())
    {
        QScreen *screen = this->screen();

        const QRect availableGeometry = screen->availableGeometry();
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2, (availableGeometry.height() - height()) / 2);
    }
    else
    {
        restoreGeometry(geometry);
    }
    */
}
/** ***************************************************************************
 * \brief MainWindow::writeGeometrySettings
 */
void MainWindow::writeGeometrySettings()
{
    QSettings qsettings(mySetting->getOrgDomain(), mySetting->getAppName());

    /*
    qsettings.beginGroup( "mainwindow" );

    qsettings.setValue( "geometry", saveGeometry() );
    qsettings.setValue( "savestate", saveState() );
    qsettings.setValue( "maximized", isMaximized() );
    if ( !isMaximized() ) {
        qsettings.setValue( "pos", pos() );
        qsettings.setValue( "size", size() );
    }

    qsettings.endGroup();
    */
}
/** ***************************************************************************
 * \brief MainWindow::readGeometrySettings
 */
void MainWindow::readGeometrySettings()
{
    QSettings qsettings(mySetting->getOrgDomain(), mySetting->getAppName());

    /*
    qsettings.beginGroup( "mainwindow" );

    restoreGeometry(qsettings.value( "geometry", saveGeometry() ).toByteArray());
    restoreState(qsettings.value( "savestate", saveState() ).toByteArray());
    move(qsettings.value( "pos", pos() ).toPoint());
    resize(qsettings.value( "size", size() ).toSize());
    if ( qsettings.value( "maximized", isMaximized() ).toBool() ) { showMaximized(); }

    qsettings.endGroup();
    */
}
/** ***************************************************************************
 *  \brief MainWindow::isModified
 */
bool MainWindow::isModified() const
{
    return ui->plainTextEditor->document()->isModified();
}
/** ***************************************************************************
 *  \brief MainWindow::onFileNew
 */
void MainWindow::onFileNew()
{
    if (isModified())
    {
        QMessageBox::StandardButton button = QMessageBox::question(this, windowTitle(), tr("You have unsaved changes. Do you want to create a new document anyway?"));
        if (button != QMessageBox::Yes) { return; }
    }

    myFilePath.clear();
    ui->plainTextEditor->setPlainText(tr("## New document"));
    ui->plainTextEditor->document()->setModified(false);
}
/** ***************************************************************************
 *  \brief MainWindow::openFile
 *  \param  path
 */
void MainWindow::openFile(const QString &path)
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, windowTitle(), tr("Could not open file %1: %2").arg(QDir::toNativeSeparators(path), f.errorString()));
        return;
    }
    myFilePath = path;
    ui->plainTextEditor->setPlainText(f.readAll());
    statusBar()->showMessage(tr("Opened %1").arg(QDir::toNativeSeparators(path)));
}
/** ***************************************************************************
 *  \brief MainWindow::onFileOpen
 */
void MainWindow::onFileOpen()
{
    if (isModified())
    {
        QMessageBox::StandardButton button = QMessageBox::question(this, windowTitle(), tr("You have unsaved changes. Do you want to open a new document anyway?"));
        if (button != QMessageBox::Yes) { return; }
    }

    QFileDialog dialog(this, tr("Open MarkDown File"));
    dialog.setMimeTypeFilters({"text/markdown"});
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    if (dialog.exec() == QDialog::Accepted) { openFile(dialog.selectedFiles().constFirst()); }
}
/** ***************************************************************************
 *  \brief MainWindow::onFileSave
 */
void MainWindow::onFileSave()
{
    if (myFilePath.isEmpty())
    {
        onFileSaveAs();
        return;
    }

    QFile f(myFilePath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, windowTitle(), tr("Could not write to file %1: %2").arg(QDir::toNativeSeparators(myFilePath), f.errorString()));
        return;
    }
    QTextStream str(&f);
    str << ui->plainTextEditor->toPlainText();

    ui->plainTextEditor->document()->setModified(false);

    statusBar()->showMessage(tr("Wrote %1").arg(QDir::toNativeSeparators(myFilePath)));
}
/** ***************************************************************************
 *  \brief MainWindow::onFileSaveAs
 */
void MainWindow::onFileSaveAs()
{
    QFileDialog dialog(this, tr("Save MarkDown File"));
    dialog.setMimeTypeFilters({"text/markdown"});
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix("md");
    if (dialog.exec() != QDialog::Accepted) { return; }

    myFilePath = dialog.selectedFiles().constFirst();
    onFileSave();
}
/** ***************************************************************************
 * \brief MainWindow::onHelp
 */
void MainWindow::onHelp()
{
    // Go to Tab
    ui->tabWidget->setCurrentIndex(ui->tabWidget->indexOf(ui->tabWidget->findChild<QWidget*>("tabHelp")));
}
/** ***************************************************************************
 * \brief MainWindow::onAbout
 */
void MainWindow::onAbout()
{
    aboutMe = new DialogAbout(this);
    aboutMe->show();
}
/** ***************************************************************************
 * \brief MainWindow::onTextEditContentTextChanged
 */
void MainWindow::onPushButtonReadFileClicked()
{
    if (mySetting->isFileExists(ui->lineEditFileName->text()))
    {
        ui->plainTextEditor->setPlainText(mySetting->readFile(ui->lineEditFileName->text()));
    }
}
/** ***************************************************************************
 * \brief MainWindow::onPushButtonFindFileClicked
 */
void MainWindow::onPushButtonFindFileClicked()
{
    QString file1Name = QFileDialog::getOpenFileName(this, tr("Open File"), "/home", tr("Supported Files (*.txt *.htm *.html *.md)"));
    ui->lineEditFileName->setText(file1Name);
    mySetting->writeSettings(MY_FILE_NAME, file1Name);
    onPushButtonReadFileClicked();
}
/** ***************************************************************************
 * \brief MainWindow::onRadioButtonPlainClicked
 */
void MainWindow::onRadioButtonPlainClicked()
{
    mySetting->writeSettingsBool(MY_FILE_PLAIN, true);
    mySetting->writeSettingsBool(MY_FILE_MARKDOWN, false);
    mySetting->writeSettingsBool(MY_FILE_HTML, false);
}
/** ***************************************************************************
 * \brief MainWindow::onRadioButtonHtmlClicked
 */
void MainWindow::onRadioButtonHtmlClicked()
{
    mySetting->writeSettingsBool(MY_FILE_HTML, true);
    mySetting->writeSettingsBool(MY_FILE_MARKDOWN, false);
    mySetting->writeSettingsBool(MY_FILE_PLAIN, false);
}
/** ***************************************************************************
 * \brief MainWindow::onRadioButtonMarkDownClicked
 */
void MainWindow::onRadioButtonMarkDownClicked()
{
    mySetting->writeSettingsBool(MY_FILE_MARKDOWN, true);
    mySetting->writeSettingsBool(MY_FILE_HTML, false);
    mySetting->writeSettingsBool(MY_FILE_PLAIN, false);
}
/** ***************************************************************************
 * \brief MainWindow::onTabWidgetCurrentChanged
 * \param  index not used
 */
void MainWindow::onTabWidgetCurrentChanged(int index)
{
    switch (index)
    {
        case 0:
            // Settings
            mySetting->writeSettingsInt(MY_TAB, 0);
            break;
        case 1:
            // Text Editor
            mySetting->writeSettingsInt(MY_TAB, 1);
            break;
        case 2:
            // Text To Speach
            mySetting->writeSettingsInt(MY_TAB, 2);
            break;
        case 3:
            // Help
            mySetting->writeSettingsInt(MY_TAB, 3);
            break;
    }
}
/** ***************************************************************************
 * \brief MainWindow::onPushButtonIconClicked
 */
void MainWindow::onPushButtonIconClicked()
{
    ui->lineEditIcon->setText(QFileDialog::getOpenFileName(this, tr("Get Icon"), "", tr("Image Icon Files (*.ico)")));
    ui->statusbar->showMessage("Set Icon", 6000);
}
/** ***************************************************************************
 * \brief MainWindow::speak
 */
void MainWindow::speak()
{
    mySpeech->say(ui->plainTextEdit->toPlainText());
}
/** ***************************************************************************
 * \brief MainWindow::stop
 */
void MainWindow::stop()
{
    mySpeech->stop();
}
/** ***************************************************************************
 * \brief MainWindow::setRate
 * \param rate
 */
void MainWindow::setRate(int rate)
{
    mySpeech->setRate(rate / 10.0);
}
/** ***************************************************************************
 * \brief MainWindow::setPitch
 * \param pitch
 */
void MainWindow::setPitch(int pitch)
{
    mySpeech->setPitch(pitch / 10.0);
}
/** ***************************************************************************
 * \brief MainWindow::setVolume
 * \param volume
 */
void MainWindow::setVolume(int volume)
{
    mySpeech->setVolume(volume / 100.0);
}
/** ***************************************************************************
 * \brief MainWindow::stateChanged
 * \param state
 */
void MainWindow::stateChanged(QTextToSpeech::State state)
{
    if (state == QTextToSpeech::Speaking)
    {
        ui->statusbar->showMessage("Speech started...");
    }
    else if (state == QTextToSpeech::Ready)
    {
        ui->statusbar->showMessage("Speech stopped...", 2000);
    }
    else if (state == QTextToSpeech::Paused)
    {
        ui->statusbar->showMessage("Speech paused...");
    }
    else
    {
        ui->statusbar->showMessage("Speech error!");
    }

    ui->pauseButton->setEnabled(state == QTextToSpeech::Speaking);
    ui->resumeButton->setEnabled(state == QTextToSpeech::Paused);
    ui->stopButton->setEnabled(state == QTextToSpeech::Speaking || state == QTextToSpeech::Paused);
}
/** ***************************************************************************
 * \brief MainWindow::engineSelected
 * \param index
 */
void MainWindow::engineSelected(int index)
{
    QString engineName = ui->engine->itemData(index).toString();
    delete mySpeech;
    if (engineName == "default")  { mySpeech = new QTextToSpeech(this); }
    else                          { mySpeech = new QTextToSpeech(engineName, this); }
    disconnect(ui->language, &QComboBox::currentIndexChanged, this, &MainWindow::languageSelected);
    ui->language->clear();
    // Populate the languages combobox before connecting its signal.
    const QList<QLocale> locales = mySpeech->availableLocales();
    QLocale current = mySpeech->locale();
    for (const QLocale &locale : locales)
    {
        QString name(QString("%1 (%2)").arg(QLocale::languageToString(locale.language()), QLocale::countryToString(locale.country())));
        QVariant localeVariant(locale);
        ui->language->addItem(name, localeVariant);
        if (locale.name() == current.name()) { current = locale; }
    }
    setRate(ui->rate->value());
    setPitch(ui->pitch->value());
    setVolume(ui->volume->value());
    connect(ui->stopButton, &QPushButton::clicked, mySpeech, [this]{ mySpeech->stop(); });
    connect(ui->pauseButton, &QPushButton::clicked, mySpeech, [this]{ mySpeech->pause(); });
    connect(ui->resumeButton, &QPushButton::clicked, mySpeech, &QTextToSpeech::resume);

    connect(mySpeech, &QTextToSpeech::stateChanged, this, &MainWindow::stateChanged);
    connect(mySpeech, &QTextToSpeech::localeChanged, this, &MainWindow::localeChanged);

    connect(ui->language, &QComboBox::currentIndexChanged, this, &MainWindow::languageSelected);
    localeChanged(current);
}
/** ***************************************************************************
 * \brief MainWindow::languageSelected
 * \param language
 */
void MainWindow::languageSelected(int language)
{
    QLocale locale = ui->language->itemData(language).toLocale();
    mySpeech->setLocale(locale);
}
/** ***************************************************************************
 * \brief MainWindow::voiceSelected
 * \param index
 */
void MainWindow::voiceSelected(int index)
{
    mySpeech->setVoice(myVoices.at(index));
}
/** ***************************************************************************
 * \brief MainWindow::localeChanged
 * \param locale
 */
void MainWindow::localeChanged(const QLocale &locale)
{
    QVariant localeVariant(locale);
    ui->language->setCurrentIndex(ui->language->findData(localeVariant));

    disconnect(ui->voice, &QComboBox::currentIndexChanged, this, &MainWindow::voiceSelected);
    ui->voice->clear();

    myVoices = mySpeech->availableVoices();
    QVoice currentVoice = mySpeech->voice();
    for (const QVoice &voice : std::as_const(myVoices))
    {
        ui->voice->addItem(QString("%1 - %2 - %3").arg(voice.name(), QVoice::genderName(voice.gender()), QVoice::ageName(voice.age())));
        if (voice.name() == currentVoice.name()) { ui->voice->setCurrentIndex(ui->voice->count() - 1); }
    }
    connect(ui->voice, &QComboBox::currentIndexChanged, this, &MainWindow::voiceSelected);
}
/** ***************************************************************************
 * \brief MainWindow::onPushButtonPathClicked
 */
void MainWindow::onPushButtonPathClicked()
{
    ui->lineEditPath->setText(QFileDialog::getExistingDirectory(this, tr("Get File Folder")));
    setStatus(tr("Set Path"), Normal);
} // end onPushButtonPathClicked
/** ***************************************************************************
 * \brief MainWindow::setStatus
 * \param theStatus
 * \param theStatusType
 */
void MainWindow::setStatus(const QString &theStatus, MyStatusTypes theStatusType)
{
    switch (theStatusType)
    {
        case Debug:
        {
            ui->statusbar->setStyleSheet("QLabel { background-color : white ; color : black; }");
            ui->statusbar->showMessage(theStatus, 2000);
            break;
        }
        case Information:
        {
            ui->statusbar->setStyleSheet("QLabel { background-color : white ; color : black; }");
            ui->statusbar->showMessage(theStatus, 2000);
            break;
        }
        case Warning:
        {
            ui->statusbar->setStyleSheet("QLabel { background-color : yellow ; color : black; }");
            ui->statusbar->showMessage(theStatus, 2000);
            break;
        }
        case Critical:
        {
            ui->statusbar->setStyleSheet("QLabel { background-color : red ; color : white; }");
            ui->statusbar->showMessage(theStatus, 2000);
            break;
        }
        case Normal:
        {
            ui->statusbar->setStyleSheet("QLabel { background-color : white ; color : black; }");
            ui->statusbar->showMessage(theStatus, 2000);
            break;
        }
    }
}
/** ***************************************************************************
 * \brief MainWindow::onSpinBoxIconValueChanged
 * \param arg1
 */
void MainWindow::onSpinBoxIconValueChanged(int arg1)
{
    Q_UNUSED(arg1)
    loadIcon();
}
/** ***************************************************************************
 * \brief MainWindow::loadIcon
 */
void MainWindow::loadIcon()
{
    if (!mySetting->isFileExists(ui->lineEditIcon->text()))
    {
        setStatus(tr("Icon Path Not Found"), Critical);
        return;
    }
    // Load the Pixmap
    QPixmap pix(ui->lineEditIcon->text());
    // Set the scaled Pixmap to a width x height window keeping its aspect ratio
    ui->labelIcon->setPixmap(pix.scaled(ui->spinBoxIcon->value(), ui->spinBoxIcon->value(), Qt::KeepAspectRatio));
}
/** ***************************************************************************
 * \brief MainWindow::addFontsFromResources
 * \param filters
 * \return
 */
QStringList MainWindow::addFontsFromResources(const QStringList& filters)
{
  QStringList addedFonts;
  QDirIterator iter(QStringLiteral(":/fonts"), filters, QDir::Files|QDir::Readable, QDirIterator::Subdirectories);
  while (iter.hasNext())
  {
    const QString& entry = iter.next();
    if ( QFontDatabase::addApplicationFont(entry) >= 0 )
    {
      addedFonts << entry;
    }
  }
  return addedFonts;
}
// ***************************** End of File **********************************
