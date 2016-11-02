#ifndef __O_SCENE_GAME_PLAY_H__
#define __O_SCENE_GAME_PLAY_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class oSprite;
class oCharacter;

/////////////////////////////////////////////////////////////////////
//
//oLayerUiSetting
//
/////////////////////////////////////////////////////////////////////
class oLayerUiSetting : public Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(oLayerUiSetting);

    void update(float dt) override;
};

/////////////////////////////////////////////////////////////////////
//
//oLayerBackGround
//
/////////////////////////////////////////////////////////////////////
class oLayerBackground : public Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(oLayerBackground);
    
    void ShowCloud( float dt );

    void update(float dt) override;    
    
private:
    vector< Texture2D* > m_TextureCloud;
    vector< Sprite* > m_sClouds;
};

/////////////////////////////////////////////////////////////////////
//
//oSceneGamePlay
//
/////////////////////////////////////////////////////////////////////
class oSceneGamePlay : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(oSceneGamePlay);

    void onMouseDown(Event *event);
    void onMouseUp(Event *event);

    void MenuPauseCallback(cocos2d::Ref* pSender);
    void MenuResumeCallback(cocos2d::Ref* pSender);

    void update(float dt) override;

private:
    //UI�]�w�ϼh;//
    Layer   *m_LayerUiSetting;

    //�Ȱ����s;//
    Menu    *m_BtnPause;
    Menu    *m_BtnResume;

    //�I�������ʹϼh;//
    Layer   *m_LayerBg;
    Vector<Node*> m_ActionNode;

    //�ثe���ʶZ��;//
    Label   *m_Distance;

    oCharacter *m_TestActor;
    oSprite    *m_Test;
};

#endif

