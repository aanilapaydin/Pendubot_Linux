#include <irrlicht.h>
#include <iostream>
#include <QDebug>
#include "irrlichtWidget.h"


/*using namespaces*/
using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


IrrlichtWidget::IrrlichtWidget()
{
    //input capture objesi
    MEventReceiver receiver;
    //Root objeyi oluştur
    device = createDevice(EDT_OPENGL/*device type OPEN_GL de olabilir*/,
                            dimension2d<u32>(1024, 768)/*windows size*/,
                            16/*bits 16 or 32*/,
                            false/*is fullscreen?*/,
                            false/*is stencil buffer for drawing shadows*/,
                            false/*vsync, useful in fullscreen*/,
                            0/*&receiver*//*object receive events*/);
    if(!device){
        qDebug()<<"ROOT Obje olusturulamadi !!!!\n";
    }
    device->setWindowCaption(L"CSE396_Group1_Interface");
    //kullanılacak pointerları ROOTtan al
    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    guienv = device->getGUIEnvironment();


    //objeleri yerleştiriyoruz
    //masayı oluşturuyoruz
    ISceneNode *table = smgr->addMeshSceneNode(smgr->getMesh("/home/aanilapaydin/Cse396_Group1_Interface_Linux/CSE396_Group1_Interface_Linux/media/masa.obj"));
    table->setScale(vector3df(10.0f, 20.0f, 40.0f));
    table->setMaterialFlag(EMF_LIGHTING, false);
    table->setMaterialTexture(0,
            driver->getTexture("/home/aanilapaydin/Cse396_Group1_Interface_Linux/CSE396_Group1_Interface_Linux/media/wooden.bmp"));
    table->setPosition(vector3df(-6.0f, -2.3f, 0.0f));

//yeri oluşturuyoruz
    scene::ISceneNode *floor = smgr->addSphereSceneNode();
    if(floor){
    floor->setScale(vector3df(8.0f, 0.1f, 15.0f));
    floor->setMaterialFlag(video::EMF_LIGHTING, false);
    floor->setMaterialTexture(0,
             driver->getTexture("/home/aanilapaydin/Cse396_Group1_Interface_Linux/CSE396_Group1_Interface_Linux/media/rockwall.jpg"));
    floor->setPosition(core::vector3df(-6.0f, -22.0f, 0.0f));
    }//rootNode diğer objeler buna takılı olucak
    ISceneNode *rootNode = smgr->addCubeSceneNode();
    rootNode->setScale(vector3df(1.5f, 0.5f, 1.0f));
    rootNode->setMaterialFlag(EMF_LIGHTING, false);
    rootNode->setMaterialTexture(0,
              driver->getTexture("/home/aanilapaydin/Cse396_Group1_Interface_Linux/CSE396_Group1_Interface_Linux/media/engine.png"));
    rootNode->setPosition(vector3df(0.0f, 0.0f, 0.0f));
    //ilk boş parent
    firstEmpty = smgr->addEmptySceneNode();
    firstEmpty->setScale(vector3df(1.0f, 1.0f, 1.0f));
    firstEmpty->setPosition(vector3df(8.0f, 0.0f, 0.0f));
    //ilk eklem
    ISceneNode *firstNeck = smgr->addSphereSceneNode();
    firstNeck->setScale(vector3df(0.5f, 0.1f, 0.1f));
    firstNeck->setMaterialFlag(EMF_LIGHTING, false);
    firstNeck->setMaterialTexture(0,
           driver->getTexture("/home/aanilapaydin/Cse396_Group1_Interface_Linux/CSE396_Group1_Interface_Linux/media/firstneck.bmp"));
    firstNeck->setPosition(vector3df(0.0f, 0.0f, 0.0f));
    firstNeck->setParent(firstEmpty);
    //ilk kol
    ISceneNode *firstArm = smgr->addCubeSceneNode();
    firstArm->setScale(vector3df(0.1f, 1.0f, 0.2f));
    firstArm->setMaterialFlag(EMF_LIGHTING, false);
    firstArm->setMaterialTexture(0,
           driver->getTexture("/home/aanilapaydin/Cse396_Group1_Interface_Linux/CSE396_Group1_Interface_Linux/media/firstarm.bmp"));
    firstArm->setPosition(vector3df(1.0f, -2.0f, 0.0f));
    firstArm->setParent(firstEmpty);
    //ikinci boş parent
    secondEmpty = smgr->addEmptySceneNode();
    secondEmpty->setScale(vector3df(1.0f, 1.0f, 1.0f));
    secondEmpty->setParent(firstEmpty);
    secondEmpty->setPosition(vector3df(2.0f, -6.0f, 0.0f));
    //ikinci eklem
    ISceneNode *secondNeck = smgr->addSphereSceneNode();
    secondNeck->setScale(vector3df(0.5f, 0.1f, 0.1f));
    secondNeck->setMaterialFlag(EMF_LIGHTING, false);
    secondNeck->setMaterialTexture(0,
         driver->getTexture("/home/aanilapaydin/Cse396_Group1_Interface_Linux/CSE396_Group1_Interface_Linux/media/secondneck.bmp"));
    secondNeck->setPosition(vector3df(0.0f, 0.0f, 0.0f));
    secondNeck->setParent(secondEmpty);
    //ikinci kol
    ISceneNode *secondArm = smgr->addCubeSceneNode();
    secondArm->setScale(vector3df(0.1f, 1.0f, 0.2f));
    secondArm->setMaterialFlag(EMF_LIGHTING, false);
    secondArm->setMaterialTexture(0,
         driver->getTexture("/home/aanilapaydin/Cse396_Group1_Interface_Linux/CSE396_Group1_Interface_Linux/media/white.png"));
    secondArm->setPosition(vector3df(1.5f, -2.0f, 0.0f));
    secondArm->setParent(secondEmpty);
    //Objeyi görebilmek için kamera ekliyoruz
    ICameraSceneNode* cam = smgr->addCameraSceneNode(0, vector3df(50,0,0), vector3df(0,0,0));
    //receiver.cam = cam;
    //gui ekranında derecelerin gözükmesi
    /*firstDegreeText = guienv->addStaticText(L"First Arm Degree = ", rect<u32>(10,10,300,40), true);
    firstDegreeText->setOverrideFont(guienv->getFont("media/fontcourier.bmp"));
    secondDegreeText = guienv->addStaticText(L"Second Arm Degree = ", rect<u32>(40,40,300,70), true);
    secondDegreeText->setOverrideFont(guienv->getFont("media/fontcourier.bmp"));*/
}

bool IrrlichtWidget::isDeviceRun(){
    return device->run();
}

void IrrlichtWidget::drawScene(float firstDegree, float secondDegree){
    //start scene
    driver->beginScene(true, true, SColor(205,100,135,140));
    //rotate objects
    neckRotateX(firstEmpty, firstDegree);
    neckRotateX(secondEmpty, secondDegree);
    //write degrees to text
    //wchar_t temp[255];
    //swprintf_s(temp, 255, L"First Arm Degree = %f",firstDegree);
    //firstDegreeText->setText(temp);
    //swprintf_s(temp, 255, L"Second Arm Degree = %f",secondDegree);
    //secondDegreeText->setText(temp);
    //draw objects
    smgr->drawAll();
    guienv->drawAll();
    //end scene
    driver->endScene();
}

void IrrlichtWidget::dropDevice(){
    device->drop();
}

/**kolun dönmesini sağlayan fonksiyon*/
void IrrlichtWidget::neckRotateX(ISceneNode *neck, float degree){
    vector3df neckRotation = neck->getRotation();
    neckRotation.X = degree;
    neck->setRotation(neckRotation);
}

