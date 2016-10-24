#include "oUnit.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

#define AUDIO_INS CocosDenshion::SimpleAudioEngine::getInstance()

/////////////////////////////////////////////////////////////////////
//
//oAudio
//
/////////////////////////////////////////////////////////////////////
oAudio::oAudio()
{
    m_IsCreated = false;
}

oAudio::~oAudio()
{
    if (!m_strFilePath.empty())
    {
        if(!m_isMusic)
            AUDIO_INS->unloadEffect(m_strFilePath.c_str());
    }
}

bool oAudio::Create(string File, bool isMusic, bool isLoop)
{
    if (!cocos2d::FileUtils::getInstance()->isFileExist(File))
        return false;

    //預先載入音樂或音效;//
    if (m_isMusic)
        AUDIO_INS->preloadBackgroundMusic(m_strFilePath.c_str());
    else
        AUDIO_INS->preloadEffect(m_strFilePath.c_str());
    
    m_strFilePath = File;
    m_isLoop = isLoop;
    m_isMusic = isMusic;
    m_IsCreated = true;
    return true;
}

void oAudio::Play()
{
    if (!m_IsCreated)
        return;

    if (m_isMusic)
        AUDIO_INS->playBackgroundMusic(m_strFilePath.c_str(), m_isLoop);
    else
        m_nSoundId = AUDIO_INS->playEffect(m_strFilePath.c_str(), m_isLoop);
}

void oAudio::Stop()
{
    if (!m_IsCreated)
        return;

    if (m_isMusic)
        AUDIO_INS->stopBackgroundMusic();
    else
        AUDIO_INS->stopEffect(m_nSoundId);
}

void oAudio::Pause(bool isPause)
{
    if (!m_IsCreated)
        return;

    if (isPause)
    {
        if (m_isMusic)
            AUDIO_INS->pauseBackgroundMusic();
        else
            AUDIO_INS->pauseEffect(m_nSoundId);
    }
    else
    {
        if (m_isMusic)
            AUDIO_INS->resumeBackgroundMusic();
        else
            AUDIO_INS->resumeEffect(m_nSoundId);
    }
}

/////////////////////////////////////////////////////////////////////
//
//oSprite
//
/////////////////////////////////////////////////////////////////////
oSprite::oSprite()
{
    m_Animation = NULL;
}

oSprite::~oSprite()
{

}

void oSprite::SetAnimation(string AniFile, int nAniNum)
{
    m_Animation = Animation::create();
    for (int i = 1; i<nAniNum; i++)
    {
        char szName[100] = { 0 };
        sprintf(szName, "%s%02d.png", AniFile.c_str(), i);
        m_Animation->addSpriteFrameWithFile(szName);
    }

    // should last 2.8 seconds. And there are 14 frames.
    m_Animation->setDelayPerUnit(2.8f / 14.0f);
    m_Animation->setRestoreOriginalFrame(true);
}

void oSprite::AniPlay(int Loop)
{
    if (!m_Animation)
        return;

    m_Animation->setLoops(Loop);
    Animate* Ani = Animate::create(m_Animation);    
    this->runAction(Ani);
}

void oSprite::AniPause(bool isResume)
{
    if (!m_Animation)
        return;


}

void oSprite::SetMusic(string File, bool isMusic, bool isLoop)
{
    m_Audio.Create(File, isMusic, isLoop);
}

void oSprite::MusicPlay()
{
    m_Audio.Play();
}

void oSprite::MusicStop()
{
    m_Audio.Stop();
}

void oSprite::MusicPause(bool isPause)
{
    m_Audio.Pause(isPause);
}
