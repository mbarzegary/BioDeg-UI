#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->importMeshRadio->setChecked(true);
    ui->refineInitMeshCheck->toggled(false);
    ui->solveFluidCheck->toggled(false);
    ui->doRedistCheck->setChecked(true);
    ui->exportScaffoldCheck->toggled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_importMeshRadio_toggled(bool checked)
{
    ui->importMeshGroup->setEnabled(checked);
    ui->createMeshGroup->setEnabled(!checked);
}

void MainWindow::on_refineInitMeshCheck_toggled(bool checked)
{
    ui->refineErrorSpin->setEnabled(checked);
    ui->refineErrorLabel->setEnabled(checked);
    ui->refineMinSpin->setEnabled(checked);
    ui->refineMinLabel->setEnabled(checked);
    ui->refineMaxSpin->setEnabled(checked);
    ui->refineMaxLabel->setEnabled(checked);
}

void MainWindow::on_solveFluidCheck_toggled(bool checked)
{
    ui->solveFullNSCheck->setEnabled(checked);
    ui->writeFluidOutputCheck->setEnabled(checked);
    ui->solveFluidEachSpin->setEnabled(checked);
    ui->solveFluidEachLabel_1->setEnabled(checked);
    ui->solveFluidEachLabel_2->setEnabled(checked);
    ui->convectGroup->setEnabled(checked);
}

void MainWindow::on_doRedistCheck_toggled(bool checked)
{
    ui->redistTimeSpin->setEnabled(checked);
    ui->redistTimeLabel->setEnabled(checked);
}

void MainWindow::on_exportScaffoldCheck_toggled(bool checked)
{
    ui->exportTimeSpin->setEnabled(checked);
    ui->exportTimeLabel->setEnabled(checked);
    ui->exportScaffVolumeCheck->setEnabled(checked);
    ui->exportScaffSurfaceCheck->setEnabled(checked);
}

void MainWindow::on_meshFileBrowseButton_clicked()
{
    QFileDialog* qfd = new QFileDialog(this, "Select Mesh File", "", "*.mesh");
    QString s = qfd->getOpenFileName(this, "Select Mesh File", "", "*.mesh");
    delete qfd;

    if (s.toStdString().empty())
        return;

    ui->meshFileNameEdit->setText(s);
}

void MainWindow::on_outputDirBrowseButton_clicked()
{
    QFileDialog* qfd = new QFileDialog(this, "Select Output Directory", "", "");
    qfd->setFileMode(QFileDialog::Directory);
    qfd->setOption(QFileDialog::ShowDirsOnly, true);
    QString s = qfd->getExistingDirectory(this, "Select Output Directory", "");
    delete qfd;

    if (s.toStdString().empty())
        return;

    ui->outputDirEdit->setText(s);
}
