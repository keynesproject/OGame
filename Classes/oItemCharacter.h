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
    eSTATE_SHOW_UP,   //�}��_�X;//
    eSTATE_MOVE_OUT,  //�������~����;//
    eSTATE_BLOW_UP,   //�}�l�_�ʫ_�j;//
    eSTATE_BOMB,      //�z���ĪG,����ܽK�����}��;//

    eSTATE_ERROR,
};

class oCharacter : public Sprite
{
public:

    static oCharacter* create();
    static oCharacter* create(const std::string& filename);

    oCharacter();
    
    ~oCharacter();

    //�]�w�}����;//
    virtual void Setup(CharacterData *Data);

    //�]�w���檬�A;//
    virtual bool SetState(CharacterState State);

    //�����w�ѼƥN�����U�@�Ӫ��A;//
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
