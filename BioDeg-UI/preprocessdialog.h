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

private:
    Ui::preprocessdialog *ui;
};

#endif // PREPROCESSDIALOG_H
