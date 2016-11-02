#include "oItemCharacter.h"
#include "oUnit.h"
#include "oConfig.h"

/////////////////////////////////////////////////////////////////////
//
//oCharacter
//
/////////////////////////////////////////////////////////////////////
oCharacter* oCharacter::create()
{
    oCharacter *sprite = new (std::nothrow) oCharacter();
    if (sprite && sprite->init())
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

oCharacter* oCharacter::create(const std::string& filename)
{
    oCharacter *sprite = new (std::nothrow) oCharacter();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

oCharacter::oCharacter()
{    

}

oCharacter::~oCharacter()
{

}

void oCharacter::Setup(CharacterData *pData)
{
    for (size_t i = 0; i < eACTOR_MAX_NUM; i++)
    {
        m_Actor.push_back(oSprite::create());
        this->addChild(m_Actor[i]);
    }
    
    //設定圖形及音樂資訊;//
    m_Actor[eACTOR_BODY]->SetAnimation(&pData->Body);
    m_Actor[eACTOR_FALL]->SetAnimation(&pData->Fall);
    m_Actor[eACTOR_BOMB]->SetAnimation(&pData->Bomb);

    m_State = eSTATE_STOP;
}

bool oCharacter::SetState(CharacterState State)
{
    if (   State >= eSTATE_ERROR
        || State < eSTATE_STOP)
        return false;

    m_State = State;

    return true;
}

bool oCharacter::SetState()
{
    return true;
}

bool oCharacter::Action()
{
    return false;
}