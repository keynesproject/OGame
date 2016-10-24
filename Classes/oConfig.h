#ifndef __O_CONFIG_H__
#define __O_CONFIG_H__

#include "cocos2d.h"
#include "json/rapidjson.h" 
#include "json/document.h"

using namespace std;

//音樂及音效資料;//
struct MusicData
{
    string strJsonTag;
    string strFilePath; //音樂檔案路徑;//    
    int    nPlayTime;   //播放次數;//
    bool   isLoop;      //是否循環撥放;//
    bool   isMusic;     //true代表為音樂,false代表為音效;//
};

//圖形資料;//
struct ImageData
{
    string    strJsonTag;
    string    strFilePath; //圖形檔案路徑;//
    int       nAniNum;     //動畫檔案張數;//
    MusicData Music;       //圖形需要音效資料;//
};

//腳色資料;//
struct CharacterData
{
    string    strJsonTag;
    ImageData Body;        //Body圖檔;//
    ImageData Fall;        //掉落圖檔;//
    ImageData Bomb;        //爆炸特效圖檔;//
    int       nSatellite;  //衛星數量;//
    int       nSpeed;      //移動速度;//
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

    bool m_IsParsed;  //紀錄是否以解析過Config.json檔;//

    //各檔案類型儲存的資料夾路徑;//
    string m_strDirImage;
    string m_strDirMusic;
    string m_strDirCharacter;

    //字型檔路徑;//
    vector< string > m_Fonts;

    //各Scene的音樂,圖檔,腳色資訊;//
    map< string, oSceneConfig* > m_SceneData;
};

#endif

