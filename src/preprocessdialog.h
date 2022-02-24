#ifndef PREPROCESSDIALOG_H
#define PREPROCESSDIALOG_H

#include <QDialog>
#include <QProcess>

namespace Ui {
class PreprocessDialog;
}

class PreprocessDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreprocessDialog(QWidget *parent = nullptr);
    ~PreprocessDialog();

private slots:
    void on_meshFileBrowseButton_clicked();
    void on_outputFileBrowseButton_clicked();
    void on_refineInitMeshCheck_toggled(bool);
    void on_parallelCheck_toggled(bool);
    void on_runButton_clicked();
    void on_stopButton_clicked();
    void processFinished(int, QProcess::ExitStatus);
    void readOutput();
    void readError();

signals:
    void sendMessage(QString, bool);

private:
    Ui::PreprocessDialog *ui;
    QString prepareArguments();
};

#endif // PREPROCESSDIALOG_H
