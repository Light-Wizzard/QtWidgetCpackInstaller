#ifndef DIALOGABOUT_H
#define DIALOGABOUT_H

#include <QDialog>

namespace Ui { class DialogAbout; }
/** ***************************************************************************
 * \brief The DialogAbout class
 */
class DialogAbout : public QDialog
{
        Q_OBJECT

    public:
        explicit DialogAbout(QWidget *parent = nullptr);
        ~DialogAbout();

        void loadIcon();

    private slots:
        void onSpinBoxIconValueChanged(int arg1);

    private:
        Ui::DialogAbout *ui;
};
#endif // DIALOGABOUT_H
// ***************************** End of File **********************************
