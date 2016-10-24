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
    //初始畫圖層，並設定底色為白色;//
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

    //取得Scene設定資料;//
    oSceneConfig *SceneConfig = oConfig::getInstance()->GetSceneConfig("SceneLogo");
    if (!SceneConfig)
        return false;

    //取得音樂檔;//
    MusicData *mData = SceneConfig->GetMusic("LogoStart");

    //取得圖檔;//


    //建立LOGO;//
    m_sLogo = Sprite::create("LOGO.png");
    m_sLogo->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    m_sLogo->setScale(5);
    this->addChild(m_sLogo, 0);


    //建立滑鼠按下事件;//
    auto MouseListener = EventListenerMouse::create();
    MouseListener->onMouseDown = CC_CALLBACK_1(oSceneLogo::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(MouseListener, this);

    scheduleOnce(schedule_selector(oSceneLogo::UpdateOnce), 5.0f);

    return true;
}

void oSceneLogo::onMouseDown(Event *event)
{
    //滑鼠按下後切換至遊戲場景;//
    auto SceneGamePlay = oSceneGamePlay::createScene();
    Director::getInstance()->replaceScene(SceneGamePlay);
}

void oSceneLogo::UpdateOnce(float DelayTime)
{
    //經過Delay時間後，切換至下個場景;//
    auto SceneGamePlay = oSceneGamePlay::createScene();
    Director::getInstance()->replaceScene(SceneGamePlay);
}


