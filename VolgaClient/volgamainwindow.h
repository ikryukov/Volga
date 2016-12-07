#ifndef VOLGAMAINWINDOW_H
#define VOLGAMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class VolgaMainWindow;
}

class VolgaMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit VolgaMainWindow(QWidget *parent = 0);
    ~VolgaMainWindow();

public slots:
    void onOpen();

private:
    Ui::VolgaMainWindow *ui;
};

#endif // VOLGAMAINWINDOW_H
