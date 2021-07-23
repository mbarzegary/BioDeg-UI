#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QtMath>
#include <QProcess>

#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>

QProcess *process;

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

    ui->setupDock->setMinimumSize(ui->setupDock->minimumSize().width(), 200);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::prepareArguments()
{
    QString args;
    QTextStream out(&args);

    out << "-import_mesh ";
    if (ui->importMeshRadio->isChecked())
    {
        out << "1 ";
        out << "-mesh_file \"" << ui->meshFileNameEdit->text() + "\"";
        out << " -label_scaffold " << ui->scaffoldLabelSpin->value();
        out << " -label_medium " << ui->mediumLabelSpin->value();
    }
    else
    {
        out << "0 ";
        out << "-box_length " << ui->boxLengthSpin->value();
        out << " -cube_size_x " << ui->cuboidSizeXSpin->value();
        out << " -cube_size_y " << ui->cuboidSizeYSpin->value();
        out << " -cube_size_z " << ui->cuboidSizeZSpin->value();
        out << " -mesh_size " << ui->meshSizeSpin->value();
    }

    out << " -refine_initial_mesh ";
    if (ui->refineInitMeshCheck->isChecked())
    {
        out << "1 ";
        out << "-mshmet_error " << ui->refineErrorSpin->value();
        out << " -mesh_size_min " << ui->refineMinSpin->value();
        out << " -mesh_size_max " << ui->refineMaxSpin->value();
    }
    else
        out << "0 ";

    out << " -material_density " << ui->matDensitySpin->value();
    out << " -film_density " << ui->filmDensitySpin->value();
    out << " -material_satur " << ui->matSaturSpin->value();
    out << " -material_eps " << ui->filmPorositySpin->value();
    out << " -material_tau " << ui->filmTortuoSpin->value();
    out << " -k1 " << ui->k1ReactionSpin->value();
    out << " -k2 " << qPow(10, ui->k2ReactionSpin->value());
    out << " -d_mg " << ui->metalDiffCoefSpin->value();
    out << " -d_cl " << ui->clDiffCoefSpin->value();
    out << " -d_oh " << ui->ohDiffCoefSpin->value();

    out << " -initial_cl " << ui->initClConcenSpin->value();
    out << " -initial_oh " << qPow(10, -1*(14-ui->initpHSpin->value()));

    out << " -solve_mg " << int(ui->solveMetalCheck->isChecked());
    out << " -solve_film " << int(ui->solveFilmCheck->isChecked());
    out << " -solve_cl " << int(ui->solveClCheck->isChecked());
    out << " -solve_oh " << int(ui->solveOHCheck->isChecked());
    out << " -solve_ls " << int(ui->solveLSCheck->isChecked());

    out << " -time_step " << ui->dtSpin->value();
    out << " -final_time " << ui->tFinalSpin->value();

    out << " -do_redistance ";
    if (ui->doRedistCheck->isChecked())
    {
        out << "1 ";
        out << " -redistance_time " << ui->redistTimeSpin->value();
    }
    else
        out << "0 ";

    out << " -solve_fluid ";
    if (ui->solveFluidCheck->isChecked())
    {
        out << "1 ";
        out << "-solve_full_ns " << int(ui->solveFullNSCheck->isChecked());
        out << " -write_fluid_output " << int(ui->writeFluidOutputCheck->isChecked());
        out << " -solve_fluid_each " << ui->solveFluidEachSpin->value();
        out << " -fluid_in_x " << ui->inletVelocXSpin->value();
        out << " -fluid_in_y " << ui->inletVelocYSpin->value();
        out << " -fluid_in_z " << ui->inletVelocZSpin->value();
        out << " -fluid_nu " << ui->dynaViscositySpin->value();
        out << " -label_wall " << ui->wallLabelSpin->value();
        out << " -label_inlet " << ui->inletLabelSpin->value();
        out << " -label_outlet " << ui->outletLabelSpin->value();
    }
    else
    {
        out << "0 ";
        out << "-write_fluid_output 0";
    }

    out << " -text_output_file \"" << ui->outputDirEdit->text() + "/" + ui->outputNameEdit->text() + ".txt\"";

    out << " -write_vtk ";
    if (ui->writeVTKCheck->isChecked())
    {
        out << "1 ";
        out << "-vtk_output_name \"" << ui->outputDirEdit->text() + "/" + ui->outputNameEdit->text() + "\"";
    }
    else
        out << "0 ";

    out << " -save_last_state " << int(ui->saveLastCheck->isChecked());
    out << " -save_each " << ui->saveEachSpin->value();

    out << " -output_per_area " << int(ui->outputPerAreaCheck->isChecked());
    out << " -save_multiplier " << ui->saveMultiplierSpin->value();

    out << " -export_scaffold ";
    if (ui->exportScaffoldCheck->isChecked())
    {
        out << "1 ";
        out << " -export_scaffold_each " << ui->exportTimeSpin->value();
        out << " -export_scaffold_volume " << int(ui->exportScaffVolumeCheck->isChecked());
        out << " -export_scaffold_surface " << int(ui->exportScaffSurfaceCheck->isChecked());
    }
    else
        out << "0 ";

    out << " -save_initial_mesh " << int(ui->saveInitialMeshCheck->isChecked());
    out << " -save_initial_partitioned_mesh " << int(ui->saveInitialPartitionCheck->isChecked());

    return args;
}

void MainWindow::readOutput()
{
    displayMessage(process->readAllStandardOutput(), false);
}

void MainWindow::readError()
{
    displayMessage(process->readAllStandardError(), true);
}

void MainWindow::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    QMessageBox qmb;
//    qmb.setText("Process finished with exit code " + QString::number(exitCode));
    if (exitStatus == QProcess::ExitStatus::NormalExit)
        if (exitCode == 0)
            qmb.setText("Process finished successfully!");
        else
            qmb.setText("Process finished with error!");
    else
        qmb.setText("Process interrupted!");
    qmb.exec();
    ui->runButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    ui->runButton->setText("Run simulation");
}

void MainWindow::displayMessage(QString msg, bool isError)
{
    if (isError)
        ui->outputText->setTextColor(QColor("red"));
    else
        ui->outputText->setTextColor(QColor("black"));
    ui->outputText->append(msg);
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

void MainWindow::on_runButton_clicked()
{
    QString args = prepareArguments();
    process = new QProcess(this);
    QString program = "mpiexec -n 3 FreeFem++-mpi ../BioDeg-core/src/main.edp -v 0 " + args;
    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(readOutput()));
    connect(process, SIGNAL(readyReadStandardError()), this, SLOT(readError()));
    connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
    displayMessage("Executing: " + program, false);
    process->start(program);
    ui->runButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    ui->runButton->setText("Running...");
}

void MainWindow::on_stopButton_clicked()
{
    if (process->state() == QProcess::ProcessState::Running)
        process->kill();
}
