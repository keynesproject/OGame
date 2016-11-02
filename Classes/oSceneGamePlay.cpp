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

    //���oScene�]�w���;//
    oSceneConfig *SceneConfig = oConfig::getInstance()->GetSceneConfig("SceneGamePlay");
    if (!SceneConfig)
        return false;

    ImageData *pImgData = SceneConfig->GetImage("Cloud");
    std::size_t Found = pImgData->strFilePath.find_last_of(".");
    if (Found == 0)
        return false;

    string strFilePath = pImgData->strFilePath.substr(0, Found);
    string strFileExtension = pImgData->strFilePath.substr(Found);    

    //�إ߶�����;//
    for(int i = 1; i <= pImgData->nAniNum; i++)
    {        
        char szName[128] = { 0 };
        sprintf(szName, "%s0%d%s", strFilePath.c_str(), i, strFileExtension.c_str());
        Texture2D *Texture = Director::getInstance()->getTextureCache()->addImage( szName );
        m_TextureCloud.push_back( Texture );
    }

    //�إ߶�Sprite;//
    for( int i = 0; i <= 20; i++ )
    {
        //�إߤ@�Ӷ�Sprite,;//
        m_sClouds.push_back( Sprite::create() );   
        m_sClouds[i]->setVisible(false);
        this->addChild( m_sClouds[i], 5 );
    }  

    schedule( schedule_selector(oLayerBackground::ShowCloud), 0.8f, kRepeatForever, 0);


    //�w�ɽիק�s;//
    scheduleUpdate();    
    
    return true;
}

void oLayerBackground::ShowCloud( float dt )
{
    //Random �@�Ӽƭ� MOD 10,�Y�j��3�h���ͷs����;//
    int Random = cocos2d::RandomHelper::random_int( 0, 10 );
    
    if ( Random < 5 )
        return;

    //���o����ܪ���Sprite;//    
    for ( size_t i = 0; i < m_sClouds.size(); i++ )
    {
        if ( m_sClouds[i]->isVisible() == false )
        {
            //��ܶ�;//
            m_sClouds[i]->setVisible( true );

            //�H�����w������;//
            Random = cocos2d::RandomHelper::random_int( 0, 8 );
            m_sClouds[i]->setTexture( m_TextureCloud[Random] );
            Rect TextureRect = Rect::ZERO;
            TextureRect.size = m_TextureCloud[Random]->getContentSize();
            m_sClouds[i]->setTextureRect( TextureRect, false, TextureRect.size );
            
            //�]�w��m�ܵe������;//
            auto ScreenSize = Director::getInstance()->getVisibleSize();
            Random = cocos2d::RandomHelper::random_int( 0, (int)(ScreenSize.width - m_sClouds[i]->getContentSize().width) );
            m_sClouds[i]->setPosition( Vec2( Random, ScreenSize.height + m_sClouds[i]->getContentSize().height ) );
            
            //�]�w���ʸ��|�ʧ@;//
            Random = cocos2d::RandomHelper::random_int(4, 7);
            auto MoveTo = MoveTo::create( Random, Vec2(m_sClouds[i]->getPositionX(), -10) );
            auto HideAction = Hide::create();
            auto CloudAction = Sequence::create(MoveTo, HideAction, NULL);
            m_sClouds[i]->runAction(CloudAction);
            
            //�]�w�z����;//
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

    //�إߥD�C������
    auto LayerGamePlay = oSceneGamePlay::create();
    LayerGamePlay->setName("SceneGamePlay");
    
    // add layer as a child to scene
    Scene->addChild( LayerGamePlay, 0 );
    
    //��ؤ覡��� background Layer;//
    //cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(1)->pause();
    //cocos2d::Director::getInstance()->getRunningScene()->getChildByName("LayerBackground")->resume();

    // return the scene
    return Scene;
}

bool oSceneGamePlay::init()
{
    //��l�Ʃ��Ϭ��H�Ŧ�;//
    if (!LayerColor::initWithColor(Color4B(150, 222, 221, 255)))
    {
        return false;
    }
    
    //���oScene�]�w���;//
    oSceneConfig *SceneConfig = oConfig::getInstance()->GetSceneConfig("SceneGamePlay");
    if (!SceneConfig)
        return false;

    //�إ߭I���ϼh-��������;//
    m_LayerBg = oLayerBackground::create();
    this->addChild(m_LayerBg, 0);  
    
    //���ժ���;//    
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

    //�Ʀr;//
    FontData *FontNum = SceneConfig->GetFont("FontNumDistance");    
    m_Distance = Label::createWithCharMap(FontNum->strFilePath, FontNum->nWidth, FontNum->nHigh, '0');
    m_Distance->setAnchorPoint(Vec2(1, 1));  
    m_Distance->setScale(3);
    m_Distance->setString("123456");
    m_Distance->setPosition(visibleSize.width-10, visibleSize.height-10);
    m_Distance->setHorizontalAlignment(TextHAlignment::RIGHT);
    this->addChild(m_Distance, 998);

    //�إ߷ƹ�����ƥ�;//
    auto MouseDownListener = EventListenerMouse::create();
    MouseDownListener->onMouseDown = CC_CALLBACK_1(oSceneGamePlay::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(MouseDownListener, this);

    auto MouseUpListener = EventListenerMouse::create();
    MouseUpListener->onMouseUp = CC_CALLBACK_1(oSceneGamePlay::onMouseUp, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(MouseUpListener, this);
    
    //�إ߼Ȱ����s;//    
    string strBtnN = SceneConfig->GetImage("BtnPauseN")->strFilePath;
    string strBtnS = SceneConfig->GetImage("BtnPauseS")->strFilePath;
    auto PauseItem = MenuItemImage::create(strBtnN.c_str(), strBtnS.c_str(), CC_CALLBACK_1(oSceneGamePlay::MenuPauseCallback, this));
    //PauseItem->setScale(2);
    m_BtnPause = Menu::create(PauseItem, NULL);
    m_BtnPause->setPosition(30, 30);
    this->addChild(m_BtnPause, 999);
    
    //�إ��~����s;//
    strBtnN = SceneConfig->GetImage("BtnResumeN")->strFilePath;
    strBtnS = SceneConfig->GetImage("BtnResumeS")->strFilePath;
    auto ResumeItem = MenuItemImage::create(strBtnN.c_str(), strBtnS.c_str(), CC_CALLBACK_1(oSceneGamePlay::MenuResumeCallback, this));
    m_BtnResume = Menu::create(ResumeItem, NULL);
    m_BtnResume->setPosition(30, 30);
    m_BtnResume->setVisible(false);
    this->addChild(m_BtnResume, 999);

    //�إ߼ȩw����;//
    m_LayerUiSetting = oLayerUiSetting::create();
    m_LayerUiSetting->setVisible(false);
    m_LayerUiSetting->pause();
    this->addChild(m_LayerUiSetting, 999);
    
    //�}�l�w�ɽի�;//
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
    //�����Ȱ�����A���Play����;//
    m_BtnPause->setVisible(false);
    m_BtnResume->setVisible(true);

    //�����έI�������Ȱ�;//
    this->pause();
    m_LayerBg->pauseSchedulerAndActions();
    m_ActionNode = m_LayerBg->getActionManager()->pauseAllRunningActions();

    //��ܳ]�w����;//
    m_LayerUiSetting->setVisible(true);
    m_LayerUiSetting->resume();    
}

void oSceneGamePlay::MenuResumeCallback(cocos2d::Ref* pSender)
{
    //����Play����A��ܼȰ�����;//
    m_BtnPause->setVisible(true);
    m_BtnResume->setVisible(false);

    //�����έI�������^�_����;//
    this->resume();
    m_LayerBg->resumeSchedulerAndActions();
    m_LayerBg->getActionManager()->resumeTargets(m_ActionNode);

    //�����]�w����;//
    m_LayerUiSetting->setVisible(false);
    m_LayerUiSetting->pause();
}
