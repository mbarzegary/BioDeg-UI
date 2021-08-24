#ifndef PREPROCESSDIALOG_H
#define PREPROCESSDIALOG_H

#include <QDialog>

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

private:
    Ui::preprocessdialog *ui;
};

#endif // PREPROCESSDIALOG_H
