#ifndef __O_ITEM_CHARACTER_H__
#define __O_ITEM_CHARACTER_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class oSprite;

struct CharacterData;

enum CharacterState
{
    eSTATE_STOP = 0,
    eSTATE_SHOW_UP,   //腳色冒出;//
    eSTATE_MOVE_OUT,  //往視窗外移動;//
    eSTATE_BLOW_UP,   //開始震動冒大;//
    eSTATE_BOMB,      //爆炸效果,並顯示皺掉的腳色;//

    eSTATE_ERROR,
};

class oCharacter : public Sprite
{
public:

    static oCharacter* create();
    static oCharacter* create(const std::string& filename);

    oCharacter();
    
    ~oCharacter();

    //設定腳色資料;//
    virtual void Setup(CharacterData *Data);

    //設定執行狀態;//
    virtual bool SetState(CharacterState State);

    //不指定參數代表執行下一個狀態;//
    bool SetState();

    virtual bool Action();

protected:
    enum eActorSprite
    {
        eACTOR_BODY=0,
        eACTOR_FALL,
        eACTOR_BOMB,
        eACTOR_MAX_NUM,
    };

    CharacterState m_State;

    vector<oSprite*> m_Actor;
};


#endif
