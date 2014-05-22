#ifndef IRRLICHTWIDGET_H
#define IRRLICHTWIDGET_H

#include <qwidget.h>
#include <irrlicht/irrlicht.h>
#include <qfont.h>

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


class QPaintEvent;

class MEventReceiver : public IEventReceiver
{
public:
    ICameraSceneNode* cam;
    float degX;
    float degY;
    float distance;
    float speed;

    int lastMX, lastMY;

    bool moveCam;

   MEventReceiver()
   {
      speed= 0.03f;
      degX= 0;
      degY=0;
      distance= 50;

      lastMX= 0;
      lastMY= 0;

      moveCam= false;
   }

   virtual bool OnEvent(const SEvent &event)
   {
        if (event.EventType == irr::EET_MOUSE_INPUT_EVENT){
            switch(event.MouseInput.Event)
            {

            case EMIE_LMOUSE_PRESSED_DOWN:
                moveCam= true;
                lastMX= event.MouseInput.X;
                lastMY= event.MouseInput.Y;
                break;

            case EMIE_LMOUSE_LEFT_UP:
                moveCam= false;
                break;

            case EMIE_MOUSE_MOVED:
                if(moveCam){
                    int mX = event.MouseInput.X;
                    int mY = event.MouseInput.Y;

                    int distX= mX - lastMX;
                    int distY= mY - lastMY;

                    lastMX= mX;
                    lastMY= mY;

                    degX -= distX * speed;
                    degY += distY * speed;

                    //
                    vector3df cpos(0,0,0);
                    cpos.X= cos(degY) * cos(degX) * distance;
                    cpos.Y= sin(degY) * distance;
                    cpos.Z= cos(degY) * sin(degX) * distance;

                    cam->setPosition(cpos);
                    cam->setTarget(vector3df(0,0,0));
                }
            break;
            }
        }
        return false;
   }
};


class IrrlichtWidget {
public:
    IrrlichtWidget( void );
    bool isDeviceRun();
    void drawScene(float firstDegree, float secondDegree);
    void dropDevice();

private:
    IrrlichtDevice *device;//device object it is root
    IVideoDriver *driver;//for change scene setting
    ISceneManager *smgr;//for adding new node to scene
    IGUIEnvironment *guienv;//for adding text to scene
    ISceneNode *firstEmpty;//ilk link
    ISceneNode *secondEmpty;//ikinci link
    IGUIStaticText *firstDegreeText;//ilk kolun derecesinin yazdığı text
    IGUIStaticText *secondDegreeText;//ikinci kolun derecesinin yazdığı text
    //private method
    void neckRotateX(ISceneNode *neck, float degree);


};

#endif
