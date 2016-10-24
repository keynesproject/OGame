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
    bool   m_IsCreated;      //�O�_�إ�;//
    string m_strFilePath;    //�����ɮ׸��|;//
    bool   m_isLoop;         //�O�_�`������;//
    bool   m_isMusic;        //true�N������,false�N������;//
    unsigned int m_nSoundId; //playEffect�ɦ^�Ǫ�ID;//
};

/////////////////////////////////////////////////////////////////////
//
//oSprite : ��X�i���ּ���\��
//
/////////////////////////////////////////////////////////////////////
class oSprite : public Sprite
{
public:
    oSprite();
    ~oSprite();
    
    void SetAnimation(string AniFile, int nAniNum );
    void AniPlay( int Loop );                //-1:����^��, 0:�����, N:����N��;//
    void AniPause( bool isResume );

    void SetMusic(string File, bool isMusic, bool isLoop);
    void MusicPlay();
    void MusicStop();
    void MusicPause( bool isResume);
    
private:
    oAudio m_Audio;          //���ּ��񪫥�;//
    Animation *m_Animation;  //�v�漽�񪫥�;//
};

#endif 
