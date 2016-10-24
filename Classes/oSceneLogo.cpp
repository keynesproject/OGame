#include "oConfig.h"
#include "oSceneLogo.h"
#include "oSceneGamePlay.h"
#include "oUnit.h"

USING_NS_CC;

Scene* oSceneLogo::createScene()
{
    // 'scene' is an autorelease object
    auto Scene = Scene::create();

    // 'layer' is an autorelease object
    auto Layer = oSceneLogo::create();

    // add layer as a child to scene
    Scene->addChild(Layer);

    // return the scene
    return Scene;
}

bool oSceneLogo::init()
{
    //��l�e�ϼh�A�ó]�w���⬰�զ�;//
    if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255)))
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /*
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(oSceneLogo::menuCloseCallback, this));

    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
        origin.y + closeItem->getContentSize().height / 2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    */

    //���oScene�]�w���;//
    oSceneConfig *SceneConfig = oConfig::getInstance()->GetSceneConfig("SceneLogo");
    if (!SceneConfig)
        return false;

    //���o������;//
    MusicData *mData = SceneConfig->GetMusic("LogoStart");

    //���o����;//


    //�إ�LOGO;//
    m_sLogo = Sprite::create("LOGO.png");
    m_sLogo->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    m_sLogo->setScale(5);
    this->addChild(m_sLogo, 0);


    //�إ߷ƹ����U�ƥ�;//
    auto MouseListener = EventListenerMouse::create();
    MouseListener->onMouseDown = CC_CALLBACK_1(oSceneLogo::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(MouseListener, this);

    scheduleOnce(schedule_selector(oSceneLogo::UpdateOnce), 5.0f);

    return true;
}

void oSceneLogo::onMouseDown(Event *event)
{
    //�ƹ����U������ܹC������;//
    auto SceneGamePlay = oSceneGamePlay::createScene();
    Director::getInstance()->replaceScene(SceneGamePlay);
}

void oSceneLogo::UpdateOnce(float DelayTime)
{
    //�g�LDelay�ɶ���A�����ܤU�ӳ���;//
    auto SceneGamePlay = oSceneGamePlay::createScene();
    Director::getInstance()->replaceScene(SceneGamePlay);
}


