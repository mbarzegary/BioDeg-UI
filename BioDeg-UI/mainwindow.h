#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_importMeshRadio_toggled(bool checked);
    void on_refineInitMeshCheck_toggled(bool checked);
    void on_solveFluidCheck_toggled(bool checked);
    void on_doRedistCheck_toggled(bool checked);
    void on_exportScaffoldCheck_toggled(bool checked);
    void on_meshFileBrowseButton_clicked();
    void on_outputDirBrowseButton_clicked();

private:
    Ui::MainWindow *ui;
    QString prepareArguments();
};
#endif // MAINWINDOW_H
