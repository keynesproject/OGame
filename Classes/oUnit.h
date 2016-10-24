#ifndef __O_UNIT_H__
#define __O_UNIT_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

/////////////////////////////////////////////////////////////////////
//
//oAudio
//
/////////////////////////////////////////////////////////////////////
class oAudio
{
public:
    oAudio();
    ~oAudio();

    bool Create(string File, bool isMusic, bool isLoop);

    void Play();
    void Stop();
    void Pause( bool isPause );
    
private:
    bool   m_IsCreated;      //是否建立;//
    string m_strFilePath;    //音樂檔案路徑;//
    bool   m_isLoop;         //是否循環撥放;//
    bool   m_isMusic;        //true代表為音樂,false代表為音效;//
    unsigned int m_nSoundId; //playEffect時回傳的ID;//
};

/////////////////////////////////////////////////////////////////////
//
//oSprite : 整合進音樂撥放功能
//
/////////////////////////////////////////////////////////////////////
class oSprite : public Sprite
{
public:
    oSprite();
    ~oSprite();
    
    void SetAnimation(string AniFile, int nAniNum );
    void AniPlay( int Loop );                //-1:持續回播, 0:停止播放, N:播放N次;//
    void AniPause( bool isResume );

    void SetMusic(string File, bool isMusic, bool isLoop);
    void MusicPlay();
    void MusicStop();
    void MusicPause( bool isResume);
    
private:
    oAudio m_Audio;          //音樂播放物件;//
    Animation *m_Animation;  //影格播放物件;//
};

#endif 
