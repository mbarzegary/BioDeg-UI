#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "global.h"

#include <QMainWindow>
#include <QProcess>
#include <QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent (QCloseEvent*);

public slots:
    void displayMessage(QString, bool);

private slots:
    void readOutput();
    void readError();
    void processFinished(int, QProcess::ExitStatus);
    void toggleFullScreen(bool);
    void on_importMeshRadio_toggled(bool);
    void on_refineInitMeshCheck_toggled(bool);
    void on_solveFluidCheck_toggled(bool);
    void on_doRedistCheck_toggled(bool);
    void on_exportScaffoldCheck_toggled(bool);
    void on_meshFileBrowseButton_clicked();
    void on_outputDirBrowseButton_clicked();
    void on_runButton_clicked();
    void on_stopButton_clicked();
    void on_parallelCheck_toggled(bool);
    void on_actionOutput_toggled(bool);
    void on_outputDock_visibilityChanged(bool);
    void on_actionAbout_ASLI_triggered();
    void on_actionMeshgenerator_triggered();
    void on_viewResultsButton_clicked();
    void on_plotLossButton_clicked();
    void on_actionHelp_triggered();
    void on_actionTheory_guide_triggered();

private:
    Ui::MainWindow *ui;
    QString prepareArguments();
    void updateDashboard(QString);
    void initializeDashboard();
    void updateTaskList(bool, double);
};
#endif // MAINWINDOW_H
