#include "preprocessdialog.h"
#include "ui_preprocessdialog.h"

preprocessdialog::preprocessdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::preprocessdialog)
{
    ui->setupUi(this);
}

preprocessdialog::~preprocessdialog()
{
    delete ui;
}
