#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "roboteq.h"
#include "ErrorCodes.h"
#include "Constants.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Roboteq device;

    void connectRoboteq();
    void disconnectRoboteq();

    void forward();
    void reverse();
    void left();
    void right();
    void stop();
};

#endif // MAINWINDOW_H
