#include "oSceneGameplay.h"

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

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //�إ߶�����;//
    for(int i = 1; i <= 9; i++)
    {
        char szName[16] = { 0 };
        sprintf(szName, "Cloud0%d.png", i);
        Texture2D *Texture = Director::getInstance()->getTextureCache()->addImage( szName );
        m_TextureCloud.push_back( Texture );
    }

    //�إ߶�Sprite;//
    for( int i = 0; i <= 9; i++ )
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

    // 'layer' is an autorelease object
    auto LayerGamePlay = oSceneGamePlay::create();
    LayerGamePlay->setName("SceneGamePlay");
    auto LayerBackground = oLayerBackground::create();    
    LayerBackground->setName("LayerBackground");

    // add layer as a child to scene
    Scene->addChild( LayerGamePlay, 0 );
    Scene->addChild( LayerBackground, 1, 1 );
    
    //��ؤ覡��� background Layer;//
    //cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(1)->pause();
    //cocos2d::Director::getInstance()->getRunningScene()->getChildByName("LayerBackground")->resume();

    // return the scene
    return Scene;
}

bool oSceneGamePlay::init()
{
    if (!LayerColor::initWithColor(Color4B(150, 222, 221, 255)))
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //�إ߹C������;//
    m_sGameItem = Sprite::create("Cabbage.png");
    m_sGameItem->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 4 ) );
    m_sGameItem->setScale(2);
    this->addChild(m_sGameItem, 5 );
    
    //�إ߷ƹ�����ƥ�;//
    auto MouseDownListener = EventListenerMouse::create();
    MouseDownListener->onMouseDown = CC_CALLBACK_1(oSceneGamePlay::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(MouseDownListener, this);

    auto MouseUpListener = EventListenerMouse::create();
    MouseUpListener->onMouseUp = CC_CALLBACK_1(oSceneGamePlay::onMouseUp, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(MouseUpListener, this);

    //�}�l�w�ɽի�;//
    scheduleUpdate();    
    
    return true;
}

void oSceneGamePlay::onMouseDown(Event *event)
{
    Director::getInstance()->pause();
}

void oSceneGamePlay::onMouseUp(Event *event)
{
    Director::getInstance()->resume();
}

void oSceneGamePlay::update(float dt)
{

}


