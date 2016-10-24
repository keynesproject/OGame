#include "oItemCharacter.h"
#include "oUnit.h"

/////////////////////////////////////////////////////////////////////
//
//oCharacter
//
/////////////////////////////////////////////////////////////////////
oCharacter::oCharacter(Layer* Scene)
{
    m_Scene = Scene;
    m_nState = eSTATE_STOP;
}

oCharacter::~oCharacter()
{

}

void oCharacter::SetState(int State)
{
    if (State >= eSTATE_ERROR
        || State < eSTATE_STOP)
        return;

    m_nState = State;
}

