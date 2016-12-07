#include "volgamainwindow.h"
#include "ui_volgamainwindow.h"
#include <QFileDialog>
#include <QString>
#include "configurationdialog.h"

VolgaMainWindow::VolgaMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VolgaMainWindow)
{
    ui->setupUi(this);
    ui->openGLWidget->initCL();

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(onOpen()));
    connect(ui->actionConfiguration, SIGNAL(triggered()), this, SLOT(onConfiguration()));
}

VolgaMainWindow::~VolgaMainWindow()
{
    delete ui;
}

void VolgaMainWindow::onOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open scene"), tr("."), tr("Scene Files (*.obj *.objm)"));
    if(fileName.isEmpty())
    {
        // TODO: add error handling
        return;
    }
    ui->openGLWidget->InitData(fileName);
    ui->openGLWidget->StartRenderThreads();
    ui->openGLWidget->isLoaded = true;

}

void VolgaMainWindow::onConfiguration()
{
    Configurationdialog cfgDialog;
    if (cfgDialog.exec() == QDialog::Accepted)
    {
        //TODO: update render settings
    }
}
