#include "cse396_group1_interface.h"
#include "ui_cse396_group1_interface.h"
#include <QThread>
#include "serialport.h"


SerialPort *port;

CSE396_Group1_Interface::CSE396_Group1_Interface(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CSE396_Group1_Interface)
{
    ui->setupUi(this);

    _vis_timer=new QTimer(this);

/*
     port = new SerialPort("/dev/ttyACM0", SerialPort::BR_9600);     ///////////INITIALIZE

     if(!port->open())
         qDebug()<<"error opening serial port" ;
     else
         qDebug()<<"serial port opened";

     if(!port-> prepare())
         qDebug()<<"not preparedd";


     port->write('w');*/

    std::string s;


    /*if(!port->write('w'))
            qDebug()<<"cannot writing";
        else
            qDebug()<< "writed";

    port->readUntil(s,'a');*/

    //if(s == "fila") {
        connect(_vis_timer, SIGNAL (timeout ()), this, SLOT (system_initialization()));
        _vis_timer->start(5);
        setupRealtimeData();
    //}
}

CSE396_Group1_Interface::~CSE396_Group1_Interface()
{
    delete ui;
}

void CSE396_Group1_Interface::system_initialization(){


    this->hardwareConnected=false;
    this->deadmanConnected=false;
    this->deadmanPressed=false;
    ui->hardwareEdit->setText("False");
    ui->deadmanEdit->setText("False");
    ui->hardwareEdit->setStyleSheet("QLabel { color : red; }");
    ui->deadmanEdit->setStyleSheet("QLabel { color : red; }");
    ui->stateEdit->setStyleSheet("QLabel { color : red; }");
    //bikaç tane system initilization

}
bool CSE396_Group1_Interface::get_hardwareConnected(){
    return this->hardwareConnected;
}
bool CSE396_Group1_Interface::get_deadmanConnected(){
    return this->deadmanConnected;
}
bool CSE396_Group1_Interface::get_deadmanPressed(){
    return this->deadmanPressed;
}
void CSE396_Group1_Interface::set_hardwareConnected(bool newHardCon){
    this->hardwareConnected=newHardCon;
}
void CSE396_Group1_Interface::set_deadmanConnected(bool newDeadCon){
    this->deadmanConnected=newDeadCon;
}
void CSE396_Group1_Interface::set_deadmanPressed(bool newDeadPres){
    this->deadmanPressed=newDeadPres;
}
void CSE396_Group1_Interface::state_update(int status){

    switch(status){
        case 0:
        ui->hardwareEdit->setText("False");
        ui->deadmanEdit->setText("False");
        ui->stateEdit->setText("System is not ready");
        ui->hardwareEdit->setStyleSheet("QLabel { color : red; }");
        ui->deadmanEdit->setStyleSheet("QLabel { color : red; }");
        ui->stateEdit->setStyleSheet("QLabel { color : red; }");
        break;
        case 1:
        ui->hardwareEdit->setText("False");
        ui->deadmanEdit->setText("True");
        ui->stateEdit->setText("System is not ready");
        ui->hardwareEdit->setStyleSheet("QLabel { color : red; }");
        ui->deadmanEdit->setStyleSheet("QLabel { color : green; }");
        ui->stateEdit->setStyleSheet("QLabel { color : red; }");
        break;
        case 2:
        ui->hardwareEdit->setText("True");
        ui->deadmanEdit->setText("False");
        ui->stateEdit->setText("System is not ready");
        ui->hardwareEdit->setStyleSheet("QLabel { color : green; }");
        ui->deadmanEdit->setStyleSheet("QLabel { color : red; }");
        ui->stateEdit->setStyleSheet("QLabel { color : red; }");
        break;
        case 3:
        ui->hardwareEdit->setText("True");
        ui->deadmanEdit->setText("True");
        ui->stateEdit->setText("System is ready.You can press deadman switch");
        ui->hardwareEdit->setStyleSheet("QLabel { color : green; }");
        ui->deadmanEdit->setStyleSheet("QLabel { color : green; }");
        ui->stateEdit->setStyleSheet("QLabel { color : green; }");
        break;
        case 4:
        ui->hardwareEdit->setText("True");
        ui->deadmanEdit->setText("True");
        ui->stateEdit->setText("Pendubot is running");
        ui->hardwareEdit->setStyleSheet("QLabel { color : green; }");
        ui->deadmanEdit->setStyleSheet("QLabel { color : green; }");
        ui->stateEdit->setStyleSheet("QLabel { color : green; }");
        break;
        default:
        exit(-1);
        break;
    }

}
void CSE396_Group1_Interface::setupRealtimeData()
{

  ui->myPlot->addGraph(); // blue line
  ui->myPlot->graph(0)->setPen(QPen(Qt::blue));
  ui->myPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
  ui->myPlot->graph(0)->setAntialiasedFill(false);
  ui->myPlot2->addGraph();
  ui->myPlot2->graph(0)->setPen(QPen(Qt::red));
  ui->myPlot2->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
  ui->myPlot2->graph(0)->setAntialiasedFill(false);

  ui->myPlot->addGraph(); // blue dot
  ui->myPlot->graph(1)->setPen(QPen(Qt::blue));
  ui->myPlot->graph(1)->setLineStyle(QCPGraph::lsNone);
  ui->myPlot->graph(1)->setScatterStyle(QCPScatterStyle::ssDisc);
  ui->myPlot2->addGraph(); // red dot
  ui->myPlot2->graph(1)->setPen(QPen(Qt::red));
  ui->myPlot2->graph(1)->setLineStyle(QCPGraph::lsNone);
  ui->myPlot2->graph(1)->setScatterStyle(QCPScatterStyle::ssDisc);

  ui->myPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  ui->myPlot->xAxis->setNumberFormat("g");
  ui->myPlot->xAxis->setAutoTickStep(false);
  ui->myPlot->xAxis->setTickStep(1);
  ui->myPlot->axisRect()->setupFullAxesBox();
  ui->myPlot2->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  ui->myPlot2->xAxis->setNumberFormat("g");
  ui->myPlot2->xAxis->setAutoTickStep(false);
  ui->myPlot2->xAxis->setTickStep(1);
  ui->myPlot2->axisRect()->setupFullAxesBox();
  // make left and bottom axes transfer their ranges to right and top axes:
  connect(ui->myPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->myPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(ui->myPlot2->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->myPlot2->xAxis2, SLOT(setRange(QCPRange)));

  // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
  dataTimer.start(0); // Interval 0 means to refresh as fast as possible
}
void CSE396_Group1_Interface::realtimeDataSlot()
{

  // calculate two new data points:
  double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
  static double lastPointKey = 0;

  if (key-lastPointKey >= 0.01) // at most add point every 10 ms
  {
        std::string s;
        char *temp,*temp2,*temp3;

        temp = new char[10];
        temp2= new char[5];
        //port->readUntil(s,'a');


        strcpy(temp,s.c_str());

        temp2=strtok(temp,"a-");
        int value0=((int)key)%360;
        temp3=strtok(NULL,"a-");
        int value1=((int)(key))%360;
        this->_3dWidget.drawScene(value0, value1);

        // add data to lines:
        ui->myPlot->graph(0)->addData(key, value0);
        ui->myPlot2->graph(0)->addData(key, value1);
        // set data of dots:
        ui->myPlot->graph(1)->clearData();
        ui->myPlot->graph(1)->addData(key, value0);
        ui->myPlot2->graph(1)->clearData();
        ui->myPlot2->graph(1)->addData(key, value1);
        // remove data of lines that's outside visible range:
        ui->myPlot->graph(0)->removeDataBefore(key-4);
        ui->myPlot2->graph(0)->removeDataBefore(key-4);
        // rescale value (vertical) axis to fit the current data:
        ui->myPlot->graph(0)->rescaleValueAxis();
        ui->myPlot2->graph(0)->rescaleValueAxis();
        lastPointKey = key;
  }
  // make key axis range scroll with the data (at a constant range size of 8):
  ui->myPlot->xAxis->setRange(key+1, 4, Qt::AlignRight);
  ui->myPlot->replot();
  ui->myPlot2->xAxis->setRange(key+1, 4, Qt::AlignRight);
  ui->myPlot2->replot();
  // calculate frames per second:
  static double lastFpsKey;
  static int frameCount;
  ++frameCount;
  if (key-lastFpsKey > 1) // average fps over 1 seconds
  {
    ui->statusBar->showMessage(
          QString("%1 FPS, Total Data points: %2")
          .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
          .arg(ui->myPlot->graph(0)->data()->count()+ui->myPlot->graph(1)->data()->count())
          , 0);
    lastFpsKey = key;
    frameCount = 0;
  }
}

/*
while(1){

  if(w.get_hardwareConnected() || w.get_deadmanConnected()){

        if(w.get_hardwareConnected() && !w.get_deadmanConnected()){
            w.state_update(DEADMANISNOTCONNECTED);
        }
        else if(!w.get_hardwareConnected() && w.get_deadmanConnected()){
            w.state_update(HARDWAREISNOTCONNECTED);
        }
        else if(w.get_hardwareConnected() && w.get_deadmanConnected()){

            while(!w.get_deadmanPressed()){
                w.state_update(DEADMANISNOTPRESSED);
            }
            w.state_update(SYSTEMISRUNNING);
            //system is run fonk çağırmak lazım
        }
  }
  w.state_update(SYSTEMISNOTREADY);
}
*/

