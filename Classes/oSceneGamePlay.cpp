#include "oSceneGameplay.h"
#include "oItemCharacter.h"
#include "oConfig.h"
#include "oUnit.h"

/////////////////////////////////////////////////////////////////////
//
//oLayerUiSetting
//
/////////////////////////////////////////////////////////////////////
Scene* oLayerUiSetting::createScene()
{
    // 'scene' is an autorelease object
    auto Scene = Scene::create();

    // 'layer' is an autorelease object
    auto Layer = oLayerUiSetting::create();

    // add layer as a child to scene
    Scene->addChild(Layer);

    // return the scene
    return Scene;
}

bool oLayerUiSetting::init()
{
    if (!Layer::init())
    {
        return false;
    }


    return true;
}

void oLayerUiSetting::update(float dt)
{
}

/////////////////////////////////////////////////////////////////////
//
//oLayerBackGround
//
/////////////////////////////////////////////////////////////////////
Scene* oLayerBackground::createScene()
{
    // 'scene' is an autorelease object
    auto Scene = Scene::create();

    // 'layer' is an autorelease object
    auto Layer = oLayerBackground::create();

    // add layer as a child to scene
    Scene->addChild(Layer);

    // return the scene
    return Scene;
}

bool oLayerBackground::init()
{
    if (!Layer::init())
    {
        return false;
    }

    //取得Scene設定資料;//
    oSceneConfig *SceneConfig = oConfig::getInstance()->GetSceneConfig("SceneGamePlay");
    if (!SceneConfig)
        return false;

    ImageData *pImgData = SceneConfig->GetImage("Cloud");
    std::size_t Found = pImgData->strFilePath.find_last_of(".");
    if (Found == 0)
        return false;

    string strFilePath = pImgData->strFilePath.substr(0, Found);
    string strFileExtension = pImgData->strFilePath.substr(Found);    

    //建立雲材質;//
    for(int i = 1; i <= pImgData->nAniNum; i++)
    {        
        char szName[128] = { 0 };
        sprintf(szName, "%s0%d%s", strFilePath.c_str(), i, strFileExtension.c_str());
        Texture2D *Texture = Director::getInstance()->getTextureCache()->addImage( szName );
        m_TextureCloud.push_back( Texture );
    }

    //建立雲Sprite;//
    for( int i = 0; i <= 20; i++ )
    {
        //建立一個雲Sprite,;//
        m_sClouds.push_back( Sprite::create() );   
        m_sClouds[i]->setVisible(false);
        this->addChild( m_sClouds[i], 5 );
    }  

    schedule( schedule_selector(oLayerBackground::ShowCloud), 0.8f, kRepeatForever, 0);


    //定時調度更新;//
    scheduleUpdate();    
    
    return true;
}

void oLayerBackground::ShowCloud( float dt )
{
    //Random 一個數值 MOD 10,若大於3則產生新的雲;//
    int Random = cocos2d::RandomHelper::random_int( 0, 10 );
    
    if ( Random < 5 )
        return;

    //取得未顯示的雲Sprite;//    
    for ( size_t i = 0; i < m_sClouds.size(); i++ )
    {
        if ( m_sClouds[i]->isVisible() == false )
        {
            //顯示雲;//
            m_sClouds[i]->setVisible( true );

            //隨機指定雲材質;//
            Random = cocos2d::RandomHelper::random_int( 0, 8 );
            m_sClouds[i]->setTexture( m_TextureCloud[Random] );
            Rect TextureRect = Rect::ZERO;
            TextureRect.size = m_TextureCloud[Random]->getContentSize();
            m_sClouds[i]->setTextureRect( TextureRect, false, TextureRect.size );
            
            //設定位置至畫面頂端;//
            auto ScreenSize = Director::getInstance()->getVisibleSize();
            Random = cocos2d::RandomHelper::random_int( 0, (int)(ScreenSize.width - m_sClouds[i]->getContentSize().width) );
            m_sClouds[i]->setPosition( Vec2( Random, ScreenSize.height + m_sClouds[i]->getContentSize().height ) );
            
            //設定移動路徑動作;//
            Random = cocos2d::RandomHelper::random_int(4, 7);
            auto MoveTo = MoveTo::create( Random, Vec2(m_sClouds[i]->getPositionX(), -10) );
            auto HideAction = Hide::create();
            auto CloudAction = Sequence::create(MoveTo, HideAction, NULL);
            m_sClouds[i]->runAction(CloudAction);
            
            //設定透明度;//
            Random = cocos2d::RandomHelper::random_int( 2, 6 );
            m_sClouds[i]->setCascadeOpacityEnabled(true);
            m_sClouds[i]->setOpacity( 255 * Random * 0.1f );            

            return;
        }
    }
}

void oLayerBackground::update(float dt)
{
}

/////////////////////////////////////////////////////////////////////
//
//oSceneGamePlay
//
/////////////////////////////////////////////////////////////////////
Scene* oSceneGamePlay::createScene()
{
    // 'scene' is an autorelease object
    auto Scene = Scene::create();

    //建立主遊戲場景
    auto LayerGamePlay = oSceneGamePlay::create();
    LayerGamePlay->setName("SceneGamePlay");
    
    // add layer as a child to scene
    Scene->addChild( LayerGamePlay, 0 );
    
    //兩種方式抓取 background Layer;//
    //cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(1)->pause();
    //cocos2d::Director::getInstance()->getRunningScene()->getChildByName("LayerBackground")->resume();

    // return the scene
    return Scene;
}

bool oSceneGamePlay::init()
{
    //初始化底圖為淡藍色;//
    if (!LayerColor::initWithColor(Color4B(150, 222, 221, 255)))
    {
        return false;
    }
    
    //取得Scene設定資料;//
    oSceneConfig *SceneConfig = oConfig::getInstance()->GetSceneConfig("SceneGamePlay");
    if (!SceneConfig)
        return false;

    //建立背景圖層-雲的移動;//
    m_LayerBg = oLayerBackground::create();
    this->addChild(m_LayerBg, 0);  
    
    //測試物件;//    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    //ImageData *ImgData = SceneConfig->GetImage("Cabbage");
    //m_Test = oSprite::create();
    //m_Test->SetAnimation(ImgData->strFilePath, ImgData->nAniNum, ImgData->fAniSpeed);
    //this->addChild(m_Test, 1);
    //m_Test->setScale(0.2);
    //m_Test->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 4));
    //m_Test->AniPlay(-1);

    m_TestActor = oCharacter::create();
    m_TestActor->Setup(SceneConfig->GetCharacter("Cabbage"));    
    m_TestActor->setPosition(visibleSize.width / 2, visibleSize.height / 4);
    m_TestActor->setScale(0.2f);
    this->addChild(m_TestActor, 1);

    //數字;//
    FontData *FontNum = SceneConfig->GetFont("FontNumDistance");    
    m_Distance = Label::createWithCharMap(FontNum->strFilePath, FontNum->nWidth, FontNum->nHigh, '0');
    m_Distance->setAnchorPoint(Vec2(1, 1));  
    m_Distance->setScale(3);
    m_Distance->setString("123456");
    m_Distance->setPosition(visibleSize.width-10, visibleSize.height-10);
    m_Distance->setHorizontalAlignment(TextHAlignment::RIGHT);
    this->addChild(m_Distance, 998);

    //建立滑鼠按鍵事件;//
    auto MouseDownListener = EventListenerMouse::create();
    MouseDownListener->onMouseDown = CC_CALLBACK_1(oSceneGamePlay::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(MouseDownListener, this);

    auto MouseUpListener = EventListenerMouse::create();
    MouseUpListener->onMouseUp = CC_CALLBACK_1(oSceneGamePlay::onMouseUp, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(MouseUpListener, this);
    
    //建立暫停按鈕;//    
    string strBtnN = SceneConfig->GetImage("BtnPauseN")->strFilePath;
    string strBtnS = SceneConfig->GetImage("BtnPauseS")->strFilePath;
    auto PauseItem = MenuItemImage::create(strBtnN.c_str(), strBtnS.c_str(), CC_CALLBACK_1(oSceneGamePlay::MenuPauseCallback, this));
    //PauseItem->setScale(2);
    m_BtnPause = Menu::create(PauseItem, NULL);
    m_BtnPause->setPosition(30, 30);
    this->addChild(m_BtnPause, 999);
    
    //建立繼續按鈕;//
    strBtnN = SceneConfig->GetImage("BtnResumeN")->strFilePath;
    strBtnS = SceneConfig->GetImage("BtnResumeS")->strFilePath;
    auto ResumeItem = MenuItemImage::create(strBtnN.c_str(), strBtnS.c_str(), CC_CALLBACK_1(oSceneGamePlay::MenuResumeCallback, this));
    m_BtnResume = Menu::create(ResumeItem, NULL);
    m_BtnResume->setPosition(30, 30);
    m_BtnResume->setVisible(false);
    this->addChild(m_BtnResume, 999);

    //建立暫定介面;//
    m_LayerUiSetting = oLayerUiSetting::create();
    m_LayerUiSetting->setVisible(false);
    m_LayerUiSetting->pause();
    this->addChild(m_LayerUiSetting, 999);
    
    //開始定時調度;//
    scheduleUpdate();    
    
    return true;
}

void oSceneGamePlay::onMouseDown(Event *event)
{    
    //Director::getInstance()->pause();
    int a = 5;
}

void oSceneGamePlay::onMouseUp(Event *event)
{    
    //Director::getInstance()->resume();
}

void oSceneGamePlay::update(float dt)
{

}

void oSceneGamePlay::MenuPauseCallback(cocos2d::Ref* pSender)
{    
    //關閉暫停按鍵，顯示Play按鍵;//
    m_BtnPause->setVisible(false);
    m_BtnResume->setVisible(true);

    //場景及背景場景暫停;//
    this->pause();
    m_LayerBg->pauseSchedulerAndActions();
    m_ActionNode = m_LayerBg->getActionManager()->pauseAllRunningActions();

    //顯示設定介面;//
    m_LayerUiSetting->setVisible(true);
    m_LayerUiSetting->resume();    
}

void oSceneGamePlay::MenuResumeCallback(cocos2d::Ref* pSender)
{
    //關閉Play按鍵，顯示暫停按鍵;//
    m_BtnPause->setVisible(true);
    m_BtnResume->setVisible(false);

    //場景及背景場景回復執行;//
    this->resume();
    m_LayerBg->resumeSchedulerAndActions();
    m_LayerBg->getActionManager()->resumeTargets(m_ActionNode);

    //關閉設定介面;//
    m_LayerUiSetting->setVisible(false);
    m_LayerUiSetting->pause();
}
