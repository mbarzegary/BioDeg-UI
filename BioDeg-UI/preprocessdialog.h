#ifndef PREPROCESSDIALOG_H
#define PREPROCESSDIALOG_H

#include <QDialog>
#include <QProcess>

namespace Ui {
class preprocessdialog;
}

class preprocessdialog : public QDialog
{
    Q_OBJECT

public:
    explicit preprocessdialog(QWidget *parent = nullptr);
    ~preprocessdialog();

private slots:
    void on_meshFileBrowseButton_clicked();
    void on_outputFileBrowseButton_clicked();
    void on_refineInitMeshCheck_toggled(bool checked);
    void on_parallelCheck_toggled(bool checked);
    void on_runButton_clicked();
    void on_stopButton_clicked();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void readOutput();
    void readError();

signals:
    void sendMessage(QString msg, bool isError);

private:
    Ui::preprocessdialog *ui;
    QString prepareArguments();
};

#endif // PREPROCESSDIALOG_H
