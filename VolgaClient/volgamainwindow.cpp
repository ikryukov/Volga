#include "volgamainwindow.h"
#include "ui_volgamainwindow.h"

VolgaMainWindow::VolgaMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VolgaMainWindow)
{
    ui->setupUi(this);
}

VolgaMainWindow::~VolgaMainWindow()
{
    delete ui;
}
