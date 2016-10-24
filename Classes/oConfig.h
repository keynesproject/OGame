#ifndef __O_CONFIG_H__
#define __O_CONFIG_H__

#include "cocos2d.h"
#include "json/rapidjson.h" 
#include "json/document.h"

using namespace std;

//���֤έ��ĸ��;//
struct MusicData
{
    string strJsonTag;
    string strFilePath; //�����ɮ׸��|;//    
    int    nPlayTime;   //���񦸼�;//
    bool   isLoop;      //�O�_�`������;//
    bool   isMusic;     //true�N������,false�N������;//
};

//�ϧθ��;//
struct ImageData
{
    string    strJsonTag;
    string    strFilePath; //�ϧ��ɮ׸��|;//
    int       nAniNum;     //�ʵe�ɮױi��;//
    MusicData Music;       //�ϧλݭn���ĸ��;//
};

//�}����;//
struct CharacterData
{
    string    strJsonTag;
    ImageData Body;        //Body����;//
    ImageData Fall;        //��������;//
    ImageData Bomb;        //�z���S�Ĺ���;//
    int       nSatellite;  //�ìP�ƶq;//
    int       nSpeed;      //���ʳt��;//
};

/////////////////////////////////////////////////////////////////////
//
//oSceneConfig
//
/////////////////////////////////////////////////////////////////////
class oSceneConfig
{
public:

    oSceneConfig();
    ~oSceneConfig();

    MusicData*     GetMusic(string Name);
    ImageData*     GetImage(string Name);
    CharacterData* CharacterMusic(string Name);

    vector< MusicData >&     GetALLMusic();
    vector< ImageData >&     GetALLImage();
    vector< CharacterData >& GetALLCharacter();

    void SetMusic(MusicData Data);
    void SetImage(ImageData Data);
    void SetCharacter(CharacterData Data);

private:
    vector< MusicData >   m_MusicData;
    vector< ImageData >   m_ImageData;
    vector< CharacterData > m_CharacterData;
};

/////////////////////////////////////////////////////////////////////
//
//oConfig
//
/////////////////////////////////////////////////////////////////////
class oConfig
{
public:
    static oConfig* getInstance();

    ~oConfig();

    bool LoadJsonConfig();

    oSceneConfig* GetSceneConfig( string SceneName );

private:

    oConfig();
    
    bool ParseBaseDir(rapidjson::Document& JsonDoc);
    bool ParseFont(rapidjson::Document& JsonDoc);
        
    oSceneConfig* ParseScene(rapidjson::Value& ObjScene);

    bool ParseObjMusic(oSceneConfig* Scene, rapidjson::Value& Obj );
    bool ParseObjImage(oSceneConfig* Scene, rapidjson::Value& Obj);
    bool ParseObjCharacter(oSceneConfig* Scene, rapidjson::Value& Obj);

    MusicData* ParseArrayMusic(rapidjson::Value& Array, bool isUseDefaultDir = true);
    ImageData* ParseArrayImage(rapidjson::Value& Array, bool isUseDefaultDir = true);

private:

    bool m_IsParsed;  //�����O�_�H�ѪR�LConfig.json��;//

    //�U�ɮ������x�s����Ƨ����|;//
    string m_strDirImage;
    string m_strDirMusic;
    string m_strDirCharacter;

    //�r���ɸ��|;//
    vector< string > m_Fonts;

    //�UScene������,����,�}���T;//
    map< string, oSceneConfig* > m_SceneData;
};

#endif

