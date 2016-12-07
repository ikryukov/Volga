#include "configurationdialog.h"
#include "ui_configurationdialog.h"

Configurationdialog::Configurationdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Configurationdialog)
{
    ui->setupUi(this);
}

Configurationdialog::~Configurationdialog()
{
    delete ui;
}
