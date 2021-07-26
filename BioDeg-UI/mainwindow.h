#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

private:
    Ui::MainWindow *ui;
    QString prepareArguments();
    void displayMessage(QString msg, bool isError);
    void updateDashboard(QString);
    void initializeDashboard();
};
#endif // MAINWINDOW_H
