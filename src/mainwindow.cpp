#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "aboutdialog.h"
#include "preprocessdialog.h"

#include <QtMath>
#include <QProcess>
#include <QLocale>
#include <QStyle>

#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QDialog>

//QT_CHARTS_USE_NAMESPACE
QT_USE_NAMESPACE

QProcess *process;
int totalSteps;
double totalTime;
Tasks currentTask, previousTask;
Simulation currentSimulation;

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

    ui->actionOutput->setChecked(true);

    #ifdef Q_OS_WINDOWS
    ui->outputText->setFontFamily("Consolas");
    #endif

    displayMessage("Welcome to BioDeg UI, the graphical interface of BioDeg simulation code!", false);

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
    QString output = process->readAllStandardOutput();
    QStringList list = output.split("\n");
    for (QString line : list)
        if (!line.isEmpty())
        {
            displayMessage(line, false);
            updateDashboard(line);
        }
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

void MainWindow::toggleFullScreen(bool value)
{
    if (value)
        this->showFullScreen();
    else
        this->showNormal();
}

void MainWindow::initializeDashboard()
{
    ui->dofLabel->setText("0");
    ui->nElementsLabel->setText("0");
    ui->averDOFLabel->setText("0");
    ui->nMPILabel->setText(ui->parallelCheck->isChecked() ? QString::number(ui->mpiSpin->value()) : "1");

    totalTime = ui->tFinalSpin->value();
    totalSteps = int(totalTime / ui->dtSpin->value()) + 1;
    ui->currentStepLabel->setText(QString("%1 / %2").arg(0).arg(totalSteps));
    ui->currentTimeLabel->setText(QString("%1 / %2").arg(0).arg(totalTime));
    ui->stepProgressBar->setValue(0);

    ui->massLossLabel->setText("0 %");
    ui->massLossProgressBar->setValue(0);

    ui->lsTimeLabel->setText("");
    ui->metalTimeLabel->setText("");
    ui->clTimeLabel->setText("");
    ui->filmTimeLabel->setText("");
    ui->ohTimeLabel->setText("");
    ui->nsTimeLabel->setText("");

    ui->solveMetalCheck->isChecked() ? ui->metalIconLabel->clear() :
        ui->metalIconLabel->setPixmap(style()->standardIcon(QStyle::SP_BrowserStop).pixmap(QSize(16, 16)));
    ui->solveLSCheck->isChecked() ? ui->lsIconLabel->clear() :
        ui->lsIconLabel->setPixmap(style()->standardIcon(QStyle::SP_BrowserStop).pixmap(QSize(16, 16)));
    ui->solveClCheck->isChecked() ? ui->clIconLabel->clear() :
        ui->clIconLabel->setPixmap(style()->standardIcon(QStyle::SP_BrowserStop).pixmap(QSize(16, 16)));
    ui->solveFilmCheck->isChecked() ? ui->filmIconLabel->clear() :
        ui->filmIconLabel->setPixmap(style()->standardIcon(QStyle::SP_BrowserStop).pixmap(QSize(16, 16)));
    ui->solveOHCheck->isChecked() ? ui->ohIconLabel->clear() :
        ui->ohIconLabel->setPixmap(style()->standardIcon(QStyle::SP_BrowserStop).pixmap(QSize(16, 16)));
    ui->solveFluidCheck->isChecked() ? ui->nsIconLabel->clear() :
        ui->nsIconLabel->setPixmap(style()->standardIcon(QStyle::SP_BrowserStop).pixmap(QSize(16, 16)));

}

void MainWindow::updateDashboard(QString output)
{
    output = output.toLower();
    if (output.startsWith("finite element dof"))
        ui->dofLabel->setText(QLocale(QLocale::English).toString(output.split(" ").last().toInt()));
    else if (output.startsWith("number of elements"))
        ui->nElementsLabel->setText(QLocale(QLocale::English).toString(output.split(" ").last().toInt()));
    else if (output.startsWith("the average dof"))
        ui->averDOFLabel->setText(QLocale(QLocale::English).toString(output.split(" ").last().toInt()));
    else if (output.startsWith("time:"))
    {
        double time = output.split(" ", Qt::SkipEmptyParts).at(1).toDouble();
        int step = output.split(" ", Qt::SkipEmptyParts).at(3).toInt();
        ui->currentStepLabel->setText(QString("%1 / %2").arg(step).arg(totalSteps));
        ui->currentTimeLabel->setText(QString("%1 / %2").arg(time).arg(totalTime));
        ui->stepProgressBar->setValue(int(step*100/totalSteps));
    }
    else if (output.startsWith("initial size"))
    {
        double loss = output.split(" ", Qt::SkipEmptyParts).last().toDouble();
        if (loss < 0)
            loss = 0;
        ui->massLossLabel->setText(QString("%1 %").arg(loss, 0, 'g', 3));
        ui->massLossProgressBar->setValue(int(loss));
    }
    else if (output.startsWith("solving"))
    {
        QString equation = output.split(" ", Qt::SkipEmptyParts).at(1);
        if (equation == "level")
            currentTask = Tasks::solveLSEquation;
        else if (equation == "mg")
            currentTask = Tasks::solveMetalEquation;
        else if (equation == "cl")
            currentTask = Tasks::solveClEquation;
        else if (equation == "protective")
            currentTask = Tasks::solveFilmEquation;
        else if (equation == "ph")
            currentTask = Tasks::solveOHEquation;
        else if (equation == "navier-stokes")
            currentTask = Tasks::solveFluidEquation;
        updateTaskList(false, 0);
    }
    else if (output.startsWith("solved"))
    {
        double time = output.split(" ", Qt::SkipEmptyParts).at(2).toDouble();
        updateTaskList(true, time);
    }
}

void MainWindow::updateTaskList(bool done, double time)
{
    QPixmap pixmap;
    if (done)
        pixmap = style()->standardIcon(QStyle::SP_DialogApplyButton).pixmap(QSize(16, 16));
    else
        pixmap = style()->standardIcon(QStyle::SP_ArrowRight).pixmap(QSize(16, 16));
    switch(currentTask)
    {
        case Tasks::solveMetalEquation:
            ui->metalIconLabel->setPixmap(pixmap);
            if (done)
                ui->metalTimeLabel->setText(QString("%1 seconds").arg(time, 0, 'f', 2));
            break;
        case Tasks::solveFilmEquation:
            ui->filmIconLabel->setPixmap(pixmap);
            if (done)
                ui->filmTimeLabel->setText(QString("%1 seconds").arg(time, 0, 'f', 2));
            break;
        case Tasks::solveLSEquation:
            ui->lsIconLabel->setPixmap(pixmap);
            if (done)
                ui->lsTimeLabel->setText(QString("%1 seconds").arg(time, 0, 'f', 2));
            break;
        case Tasks::solveClEquation:
            ui->clIconLabel->setPixmap(pixmap);
            if (done)
                ui->clTimeLabel->setText(QString("%1 seconds").arg(time, 0, 'f', 2));
            break;
        case Tasks::solveOHEquation:
            ui->ohIconLabel->setPixmap(pixmap);
            if (done)
                ui->ohTimeLabel->setText(QString("%1 seconds").arg(time, 0, 'f', 2));
            break;
        case Tasks::solveFluidEquation:
            ui->nsIconLabel->setPixmap(pixmap);
            if (done)
                ui->nsTimeLabel->setText(QString("%1 seconds").arg(time, 0, 'f', 2));
            break;
    }
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

void MainWindow::on_parallelCheck_toggled(bool checked)
{
    ui->parallelLabel->setEnabled(checked);
    ui->mpiSpin->setEnabled(checked);
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
    int n = 1;
    if (ui->parallelCheck->isChecked())
        n = ui->mpiSpin->value();

    process = new QProcess(this);
    QString cmdOld = "mpiexec -n " + QString::number(n) + " FreeFem++-mpi ../core/src/main.edp -v 0 " + args;
    QString program = "mpiexec";
    QStringList arguments;
    arguments << "-n" << QString::number(n);
    arguments << "FreeFem++-mpi" << "../core/src/main.edp";
    arguments << "-v" << "0";
    arguments << QProcess::splitCommand(args); // requires Qt 5.15 and above

    QString outputFile = ui->outputDirEdit->text() + "/" + ui->outputNameEdit->text() + ".txt";
    if (QFileInfo::exists(outputFile))
        QFile::remove(outputFile);

    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(readOutput()));
    connect(process, SIGNAL(readyReadStandardError()), this, SLOT(readError()));
    connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
    displayMessage("Executing: " + cmdOld, false);
    process->start(program, arguments);

    ui->runButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    ui->runButton->setText("Running...");
    initializeDashboard();

    currentSimulation.outputDir = ui->outputDirEdit->text();
    currentSimulation.mpiSize = ui->parallelCheck->isChecked() ? ui->mpiSpin->value() : 1;
    currentSimulation.vtkOutput = ui->writeVTKCheck->isChecked();
}

void MainWindow::on_stopButton_clicked()
{
    if (process->state() == QProcess::ProcessState::Running)
        process->kill();
}

void MainWindow::on_actionOutput_toggled(bool value)
{
    if (value)
        ui->outputDock->show();
    else
        ui->outputDock->hide();
}

void MainWindow::on_outputDock_visibilityChanged(bool visible)
{
    if (!this->windowState().testFlag(Qt::WindowMinimized)) // if this is not triggered because of minimizing the window
        ui->actionOutput->setChecked(visible);
}

void MainWindow::on_actionAbout_ASLI_triggered()
{
    AboutDialog *about = new AboutDialog(this);
    about->setWindowFlags(about->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    about->show();
}

void MainWindow::on_actionHelp_triggered()
{
    QString file = QFileInfo("../doc/manual/manual.pdf").absoluteFilePath();
    QUrl url = QUrl::fromLocalFile(file);
    QDesktopServices::openUrl(url);
}

void MainWindow::on_actionTheory_guide_triggered()
{
    QString file = QFileInfo("../doc/theory_guide/theory_guide.pdf").absoluteFilePath();
    QUrl url = QUrl::fromLocalFile(file);
    QDesktopServices::openUrl(url);
}

void MainWindow::on_actionMeshgenerator_triggered()
{
    PreprocessDialog *preprocessor = new PreprocessDialog(this);
    preprocessor->setWindowFlags(preprocessor->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    preprocessor->show();
}

void MainWindow::on_viewResultsButton_clicked()
{
    if (!currentSimulation.vtkOutput)
    {
        QMessageBox qmb;
        qmb.setText("Current simulation is not configured to write graphical output!");
        qmb.exec();
        return;
    }

    QString fileName = currentSimulation.outputDir + "/output_" + QString::number(currentSimulation.mpiSize) + ".pvd";
    QString arrayName;
    QString title;

    switch (ui->graphicOutputCombo->currentIndex())
    {
        case 0:
            arrayName = "Mg";
            title = "Metal ions concentration (g/mm^3)";
            break;
        case 1:
            arrayName = "F";
            title = "Protective film";
            break;
        case 2:
            arrayName = "Cl";
            title = "Cl- concentration (g/mm^3)";
            break;
        case 3:
            arrayName = "OH";
            title = "OH- concentration (g/mm^3)";
            break;
        case 4:
            arrayName = "pH";
            title = "pH";
            break;
    }

    QFile file("../postprocess/config.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << fileName << "\n";
    out << arrayName << "\n";
    out << title << "\n";

    QProcess *pythonProcess = new QProcess(this);
    QString program = "python";
    QStringList args;
    args << "run.py";
    pythonProcess->setWorkingDirectory("../postprocess/");
    pythonProcess->start(program, args);
}

void MainWindow::on_plotLossButton_clicked()
{
    QString fileName = currentSimulation.outputDir + "/output.txt";
    QLineSeries *series = new QLineSeries();

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        if (line.split("\t", Qt::SkipEmptyParts).length() > 3)
            continue;
        float time = line.split("\t", Qt::SkipEmptyParts).at(0).toFloat();
        float loss = line.split("\t", Qt::SkipEmptyParts).at(2).toFloat();
        series->append(time, loss);
    }

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Mass loss vs. time");

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QHBoxLayout *HLayout = new QHBoxLayout();
    HLayout->addWidget(chartView);

    QDialog *dialog = new QDialog(this);
    dialog->setWindowFlags(dialog->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    dialog->setLayout(HLayout);
    dialog->setWindowTitle("Mass loss plot");
    dialog->resize(400, 300);
    dialog->show();
}
