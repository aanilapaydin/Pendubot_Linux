#include "cse396_group1_interface.h"
#include <QApplication>

#ifdef Q_WS_X11
#include <X11/Xlib.h>
#endif



int main(int argc, char *argv[])
{

    #ifdef Q_WS_X11
        XInitThreads();
    #endif

    QApplication a(argc, argv);


    CSE396_Group1_Interface w;


    w.show();

    a.exec();
    return 0;
}
