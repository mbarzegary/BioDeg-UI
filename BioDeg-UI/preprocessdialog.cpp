#include "preprocessdialog.h"
#include "ui_preprocessdialog.h"

#include <QFileDialog>

preprocessdialog::preprocessdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::preprocessdialog)
{
    ui->setupUi(this);
}

preprocessdialog::~preprocessdialog()
{
    delete ui;
}

void preprocessdialog::on_meshFileBrowseButton_clicked()
{
    QFileDialog* qfd = new QFileDialog(this, "Select Mesh File", "", "*.mesh");
    QString s = qfd->getOpenFileName(this, "Select Mesh File", "", "*.mesh");
    delete qfd;

    if (s.toStdString().empty())
        return;

    ui->meshFileNameEdit->setText(s);
}

void preprocessdialog::on_outputFileBrowseButton_clicked()
{
    QFileDialog* qfd = new QFileDialog(this, "Choose Output File", "", "*.mesh");
    QString s = qfd->getOpenFileName(this, "Choose Output File", "", "*.mesh");
    delete qfd;

    if (s.toStdString().empty())
        return;

    ui->outputFileEdit->setText(s);
}

void preprocessdialog::on_refineInitMeshCheck_toggled(bool checked)
{
    ui->refineErrorSpin->setEnabled(checked);
    ui->refineErrorLabel->setEnabled(checked);
    ui->refineMinSpin->setEnabled(checked);
    ui->refineMinLabel->setEnabled(checked);
    ui->refineMaxSpin->setEnabled(checked);
    ui->refineMaxLabel->setEnabled(checked);
    ui->thresholdLabel->setEnabled(checked);
    ui->thresholdSpin->setEnabled(checked);
    ui->hausdLabel->setEnabled(checked);
    ui->hausdSpin->setEnabled(checked);
    ui->hgradLabel->setEnabled(checked);
    ui->hgradSpin->setEnabled(checked);
    ui->iterationLabel->setEnabled(checked);
    ui->iterationSpin->setEnabled(checked);
}

void preprocessdialog::on_parallelCheck_toggled(bool checked)
{
    ui->parallelLabel->setEnabled(checked);
    ui->mpiSpin->setEnabled(checked);
}
