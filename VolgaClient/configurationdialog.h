#ifndef CONFIGURATIONDIALOG_H
#define CONFIGURATIONDIALOG_H

#include <QDialog>

namespace Ui {
class Configurationdialog;
}

class Configurationdialog : public QDialog
{
    Q_OBJECT

public:
    explicit Configurationdialog(QWidget *parent = 0);
    ~Configurationdialog();

private:
    Ui::Configurationdialog *ui;
};

#endif // CONFIGURATIONDIALOG_H
