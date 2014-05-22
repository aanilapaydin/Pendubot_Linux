#ifndef CSE396_GROUP1_INTERFACE_H
#define CSE396_GROUP1_INTERFACE_H

#include <QMainWindow>
#include <iostream>
#include <vector>
#include <QObject>
#include <QThread>
#include <QRunnable>
#include <QTimer>
#include <QString>
#include <QProgressDialog>
#include "qcustomplot.h"
#include <cstdio>
#include <ctime>
#include "serialport.h"
#include "irrlichtWidget.h"

namespace Ui {
class CSE396_Group1_Interface;
#define SYSTEMISNOTREADY          0
#define HARDWAREISNOTCONNECTED    2
#define DEADMANISNOTCONNECTED     1
#define DEADMANISNOTPRESSED       3
#define SYSTEMISRUNNING           4

}

class CSE396_Group1_Interface : public QMainWindow
{
    Q_OBJECT

public:
    explicit CSE396_Group1_Interface(QWidget *parent = 0);
    ~CSE396_Group1_Interface();
    void state_update(int status);
    bool get_hardwareConnected();
    bool get_deadmanConnected();
    bool get_deadmanPressed();
    void set_hardwareConnected(bool newHardCon);
    void set_deadmanConnected(bool newDeadCon);
    void set_deadmanPressed(bool newDeadPres);
    void sleep(unsigned int mseconds);
    void setupRealtimeData();



private:
    Ui::CSE396_Group1_Interface *ui;
    QVector<int> downX,downY,upX,upY;
    bool hardwareConnected;
    bool deadmanConnected;
    bool deadmanPressed;
    QTimer *_vis_timer;
    QTimer dataTimer;
    IrrlichtWidget _3dWidget;


private slots:

    void system_initialization();
    void realtimeDataSlot();
};

#endif // CSE396_GROUP1_INTERFACE_H
