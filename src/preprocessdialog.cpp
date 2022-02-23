#include "preprocessdialog.h"
#include "ui_preprocessdialog.h"

#include <QFileDialog>
#include <QTextStream>
#include <QProcess>
#include <QMessageBox>

QProcess *meshProcess;

PreprocessDialog::PreprocessDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreprocessDialog)
{
    ui->setupUi(this);

    connect(this, SIGNAL(sendMessage(QString, bool)), this->parent(), SLOT(displayMessage(QString, bool)));
}

PreprocessDialog::~PreprocessDialog()
{
    delete ui;
}

QString PreprocessDialog::prepareArguments()
{
    QString args;
    QTextStream out(&args);

    out << " -mesh_file \"" << ui->meshFileNameEdit->text() + "\"";
    if (!ui->outputFileEdit->text().trimmed().isEmpty())
        out << " -output_file \"" << ui->outputFileEdit->text() + "\"";
    out << " -write_vtk ";
    if (ui->writeVTKCheck->isChecked())
        out << "1 ";
    else
        out << "0 ";

    out << " -margin_x " << ui->marginXSpin->value();
    out << " -margin_y " << ui->marginYSpin->value();
    out << " -margin_z " << ui->marginZSpin->value();
    out << " -mesh_size " << ui->meshSizeSpin->value();

    out << " -refine_mesh ";
    if (ui->refineInitMeshCheck->isChecked())
    {
        out << "1 ";
        out << "-mshmet_error " << ui->refineErrorSpin->value();
        out << " -mesh_size_min " << ui->refineMinSpin->value();
        out << " -mesh_size_max " << ui->refineMaxSpin->value();\
        out << " -mesh_hausd " << ui->hausdSpin->value();
        out << " -mesh_hgrad " << ui->hgradSpin->value();
        out << " -iterations " << ui->iterationSpin->value();
        out << " -threshold " << ui->thresholdSpin->value();
    }
    else
        out << "0 ";

    out << " -parallel ";
    if (ui->parallelCheck->isChecked())
        out << "1 ";
    else
        out << "0 ";

    return args;
}

void PreprocessDialog::readOutput()
{
    QString output = meshProcess->readAllStandardOutput();
    QStringList list = output.split("\n");
    for (QString line : list)
        if (!line.isEmpty())
        {
            emit sendMessage(line, false);
        }
}

void PreprocessDialog::readError()
{
    emit sendMessage(meshProcess->readAllStandardError(), true);
}

void PreprocessDialog::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    QMessageBox qmb;
    if (exitStatus == QProcess::ExitStatus::NormalExit)
        if (exitCode == 0)
            qmb.setText("Mesh generation process finished successfully!");
        else
            qmb.setText("Mesh generation process finished with error!");
    else
        qmb.setText("Mesh generation process interrupted!");
    qmb.exec();
    ui->runButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    ui->runButton->setText("Generate mesh");
}


void PreprocessDialog::on_meshFileBrowseButton_clicked()
{
    QFileDialog* qfd = new QFileDialog(this, "Select Mesh File", "", "*.mesh");
    QString s = qfd->getOpenFileName(this, "Select Mesh File", "", "*.mesh");
    delete qfd;

    if (s.toStdString().empty())
        return;

    ui->meshFileNameEdit->setText(s);
}

void PreprocessDialog::on_outputFileBrowseButton_clicked()
{
    QFileDialog* qfd = new QFileDialog(this, "Choose Output File", "", "*.mesh");
    QString s = qfd->getSaveFileName(this, "Choose Output File", "", "*.mesh");
    delete qfd;

    if (s.toStdString().empty())
        return;

    ui->outputFileEdit->setText(s);
}

void PreprocessDialog::on_refineInitMeshCheck_toggled(bool checked)
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

void PreprocessDialog::on_parallelCheck_toggled(bool checked)
{
    ui->parallelLabel->setEnabled(checked);
    ui->mpiSpin->setEnabled(checked);
}

void PreprocessDialog::on_runButton_clicked()
{
    QString args = prepareArguments();
    int n = 1;
    if (ui->parallelCheck->isChecked())
        n = ui->mpiSpin->value();

    meshProcess = new QProcess(this);
    QString cmdOld = "mpiexec -n " + QString::number(n) + " FreeFem++-mpi ../preprocess/main.edp -v 0 " + args;
    QString program = "mpiexec";
    QStringList arguments;
    arguments << "-n" << QString::number(n);
    arguments << "FreeFem++-mpi" << "../preprocess/main.edp";
    arguments << "-v" << "0";
    arguments << QProcess::splitCommand(args); // requires Qt 5.15 and above

    connect(meshProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readOutput()));
    connect(meshProcess, SIGNAL(readyReadStandardError()), this, SLOT(readError()));
    connect(meshProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
    emit sendMessage("Executing: " + cmdOld, false);
    meshProcess->start(program, arguments);

    ui->runButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    ui->runButton->setText("Running...");
}

void PreprocessDialog::on_stopButton_clicked()
{
    if (meshProcess->state() == QProcess::ProcessState::Running)
        meshProcess->kill();
}
