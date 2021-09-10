#include "aboutdialog.h"
#include "ui_aboutdialog.h"

aboutdialog::aboutdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutdialog)
{
    ui->setupUi(this);

    QString buildDate = __DATE__;
    QString buildTime = __TIME__;
    ui->buildLabel->setText(QString("Built on %1 %2").arg(buildDate).arg(buildTime));

    ui->qtVersionLabel->setText(QString("Based on Qt %1").arg(QT_VERSION_STR));

    this->setFixedSize(this->size());
}

aboutdialog::~aboutdialog()
{
    delete ui;
}
