#ifndef __O_SCENE_LOGO_H__
#define __O_SCENE_LOGO_H__

#include "cocos2d.h"

USING_NS_CC;

class oAudio;

class oSceneLogo : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(oSceneLogo);

    void onMouseDown(Event *event);

    void UpdateOnce(float DelayTime);

private:
    oAudio *m_SoundStart;
    Sprite *m_sLogo;
};


#endif
