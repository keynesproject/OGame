#ifndef __O_ITEM_CHARACTER_H__
#define __O_ITEM_CHARACTER_H__

#include "cocos2d.h"

USING_NS_CC;

class oSprite;

enum CharacterState
{
    eSTATE_STOP = 0,

    eSTATE_ERROR,
};

class oCharacter
{
public:
    oCharacter( Layer* Scene );
    
    ~oCharacter();
    
    void SetState( int State );

    virtual bool Action() = 0;

private:
    Layer* m_Scene;

    int m_nState;

    oSprite *m_sBody;
    oSprite *m_sBodyFall;
    oSprite *m_sBomb;    
};


#endif
