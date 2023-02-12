#include "DialogAbout.h"
#include "ui_DialogAbout.h"
/** ***************************************************************************
 * \brief DialogAbout::DialogAbout
 * \param parent
 */
DialogAbout::DialogAbout(QWidget *parent) : QDialog(parent), ui(new Ui::DialogAbout)
{
    ui->setupUi(this);
    connect(ui->spinBoxIcon, &QSpinBox::valueChanged, this, &DialogAbout::onSpinBoxIconValueChanged);
    loadIcon();
}
/** ***************************************************************************
 * \brief DialogAbout::~DialogAbout
 */
DialogAbout::~DialogAbout()
{
    delete ui;
}
/** ***************************************************************************
 * \brief DialogAbout::loadIcon
 */
void DialogAbout::loadIcon()
{
    QPixmap pix(":images/MyQtWidgetApp.png");
    ui->labelPic->setPixmap(pix);

    // Set the scaled Pixmap to a width x height window keeping its aspect ratio
    ui->labelPic->setPixmap(pix.scaled(ui->spinBoxIcon->value(), ui->spinBoxIcon->value(), Qt::KeepAspectRatio));
}
/** ***************************************************************************
 * \brief DialogAbout::onSpinBoxIconValueChanged
 * \param arg1 not used
 */
void DialogAbout::onSpinBoxIconValueChanged(int arg1)
{
    Q_UNUSED(arg1)
    loadIcon();
}
// ***************************** End of File **********************************
