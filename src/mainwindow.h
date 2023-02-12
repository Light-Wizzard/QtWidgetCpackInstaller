#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "DialogAbout.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/** ***************************************************************************
 * \brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private slots:
        void onQuit();
        void onAbout();

    private:
        Ui::MainWindow *ui;
        DialogAbout *about;
}; // end MainWindow
#endif // MAINWINDOW_H
// ***************************** End of File **********************************
