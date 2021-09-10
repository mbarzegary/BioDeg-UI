#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "global.h"

#include <QMainWindow>
#include <QProcess>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void displayMessage(QString msg, bool isError);

private slots:
    void readOutput();
    void readError();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void toggleFullScreen(bool value);
    void on_importMeshRadio_toggled(bool checked);
    void on_refineInitMeshCheck_toggled(bool checked);
    void on_solveFluidCheck_toggled(bool checked);
    void on_doRedistCheck_toggled(bool checked);
    void on_exportScaffoldCheck_toggled(bool checked);
    void on_meshFileBrowseButton_clicked();
    void on_outputDirBrowseButton_clicked();
    void on_runButton_clicked();
    void on_stopButton_clicked();
    void on_parallelCheck_toggled(bool checked);
    void on_actionOutput_toggled(bool arg1);
    void on_outputDock_visibilityChanged(bool visible);
    void on_actionAbout_ASLI_triggered();
    void on_actionMeshgenerator_triggered();
    void on_viewResultsButton_clicked();
    void on_plotLossButton_clicked();
    void on_actionHelp_triggered();

private:
    Ui::MainWindow *ui;
    QString prepareArguments();
    void updateDashboard(QString);
    void initializeDashboard();
    void updateTaskList(bool, double);
};
#endif // MAINWINDOW_H
