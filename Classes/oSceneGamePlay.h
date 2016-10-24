#ifndef __O_SCENE_GAME_PLAY_H__
#define __O_SCENE_GAME_PLAY_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

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

    void update(float dt) override;

private:
    Sprite *m_sGameItem;

};

#endif

