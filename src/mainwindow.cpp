#include "mainwindow.h"
#include "DialogAbout.h"
#include "./ui_mainwindow.h"

/** ***************************************************************************
 * \brief MainWindow::MainWindow
 * \param parent
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::onQuit);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onAbout);
}
/** ***************************************************************************
 * \brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete ui;
}
/** ***************************************************************************
 * \brief MainWindow::onQuit
 */
void MainWindow::onQuit()
{
    QApplication::quit();
}
/** ***************************************************************************
 * \brief MainWindow::onAbout
 */
void MainWindow::onAbout()
{
    about = new DialogAbout(this);
    about->show();
}
// ***************************** End of File **********************************
