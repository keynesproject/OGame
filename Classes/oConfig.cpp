#include "oConfig.h"

const char *ccDirFont = "DirFont";
const char *ccDirImage = "DirImage";
const char *ccDirMusic = "DirMusic";
const char *ccDirCharacter = "DirCharacter";
const char *ccHelp = "__HELP";

/////////////////////////////////////////////////////////////////////
//
//oSceneConfig
//
/////////////////////////////////////////////////////////////////////
oSceneConfig::oSceneConfig()
{

}

oSceneConfig::~oSceneConfig()
{
    
}

MusicData* oSceneConfig::GetMusic(string Name)
{
    vector< MusicData >::iterator it = m_MusicData.begin();
    for (; it != m_MusicData.end(); it++)
    {
        if (Name.compare(it->strJsonTag) == 0)
        {
            return &(*it);
        }
    }

    return NULL;
}

ImageData* oSceneConfig::GetImage(string Name)
{
    vector< ImageData >::iterator it = m_ImageData.begin();
    for (; it != m_ImageData.end(); it++)
    {
        if (Name.compare(it->strJsonTag) == 0)
        {
            return &(*it);
        }
    }

    return NULL;
}

CharacterData* oSceneConfig::CharacterMusic(string Name)
{
    vector< CharacterData >::iterator it = m_CharacterData.begin();
    for (; it != m_CharacterData.end(); it++)
    {
        if (Name.compare(it->strJsonTag) == 0)
        {
            return &(*it);
        }
    }

    return NULL;
}

vector< MusicData >& oSceneConfig::GetALLMusic()
{
    return m_MusicData;
}

vector< ImageData >& oSceneConfig::GetALLImage()
{
    return m_ImageData;
}

vector< CharacterData >& oSceneConfig::GetALLCharacter()
{
    return m_CharacterData;
}

void oSceneConfig::SetMusic(MusicData Data)
{
    m_MusicData.push_back(Data);
}

void oSceneConfig::SetImage(ImageData Data)
{
    m_ImageData.push_back(Data);
}

void oSceneConfig::SetCharacter(CharacterData Data)
{
    m_CharacterData.push_back(Data);
}

/////////////////////////////////////////////////////////////////////
//
//oConfig
//
/////////////////////////////////////////////////////////////////////
oConfig *oConfig::getInstance()
{
    static oConfig m_Instance;

    return &m_Instance;
}

oConfig::oConfig()
{
    m_IsParsed = false;
}

oConfig::~oConfig()
{
    map< string, oSceneConfig*>::iterator it = m_SceneData.begin();
    for (; it != m_SceneData.end(); it++)
    {
        if (it->second != NULL)
        {
            delete it->second;
            it->second = NULL;
        }
    }
    m_SceneData.clear();
}

oSceneConfig* oConfig::GetSceneConfig(string SceneName)
{
    map< string, oSceneConfig* >::iterator it = m_SceneData.find(SceneName);
    if (it != m_SceneData.end())
        return it->second;

    return NULL;
}

bool oConfig::LoadJsonConfig()
{
    if (m_IsParsed)
        return true;

    //讀取Json檔案;//
    std::string strJson = cocos2d::FileUtils::getInstance()->getStringFromFile("Config.json");
    if (strJson.empty())
        return false;

    //解析JSON檔案;//
    rapidjson::Document JsonDoc;
    JsonDoc.Parse<rapidjson::kParseDefaultFlags>(strJson.c_str());

    //判斷是否解析錯誤;//
    if (JsonDoc.HasParseError())
        return false;

    //判斷是否為Object;//
    if (!JsonDoc.IsObject())
        return false;

    //取得資料夾分類路徑;//
    ParseBaseDir(JsonDoc);

    //取得字型路徑;//
    ParseFont(JsonDoc);

    //讀取並各Scene資料;//
    rapidjson::Value::ConstMemberIterator it = JsonDoc.MemberBegin();
    for (; it != JsonDoc.MemberEnd(); it++ )
    {
        //判斷是否為Object格式;//
        string Name = it->name.GetString();
        if (JsonDoc[Name.c_str()].IsObject())
        {
            //解析Scene資料;//
            oSceneConfig *Scene = ParseScene(JsonDoc[Name.c_str()]);
            if (Scene == NULL)
                return false;

            m_SceneData.insert(pair< string, oSceneConfig*>(Name, Scene));
        }
    }
    
    m_IsParsed = true;

    return true;
}

bool oConfig::ParseBaseDir(rapidjson::Document& JsonDoc)
{
    if ( !(JsonDoc.HasMember(ccDirImage)
        && JsonDoc.HasMember(ccDirMusic)
        && JsonDoc.HasMember(ccDirCharacter) ) )
        return false;

    m_strDirImage = JsonDoc[ccDirImage].GetString();
    m_strDirMusic = JsonDoc[ccDirMusic].GetString();
    m_strDirCharacter = JsonDoc[ccDirCharacter].GetString();

    return true;
}

bool oConfig::ParseFont(rapidjson::Document& JsonDoc)
{
    char *chObjFont = "Fonts";

    string strDirFont;

    //取得字型路徑;//
    if (JsonDoc.HasMember(ccDirFont)
        && JsonDoc.HasMember(chObjFont))
    {
        strDirFont = JsonDoc[ccDirFont].GetString();

        rapidjson::Value& ArrayFonts = JsonDoc[chObjFont];
        for (size_t i = 0; i < ArrayFonts.Size(); i++)
        {
            string strFontFile = strDirFont;
            strFontFile.append(ArrayFonts[i].GetString());
            m_Fonts.push_back(strFontFile);
        }

        return true;
    }

    return false;
}

oSceneConfig* oConfig::ParseScene(rapidjson::Value& ObjScene)
{
    oSceneConfig *Scene = new oSceneConfig;    

    //解析音樂資料;//
    if (ObjScene.HasMember("Music"))
    {
        if (!ParseObjMusic(Scene, ObjScene["Music"]))
        {
            delete Scene;
            return NULL;
        }
    }

    //解析圖形資料;//
    if (ObjScene.HasMember("Image"))
    {
        if (!ParseObjImage(Scene, ObjScene["Image"]))
        {
            delete Scene;
            return NULL;
        }
    }

    //解析腳色資料;//
    if (ObjScene.HasMember("Character"))
    {
        if (!ParseObjCharacter(Scene, ObjScene["Character"]))
        {
            delete Scene;
            return NULL;
        }
    }

    return Scene;
}

bool oConfig::ParseObjMusic(oSceneConfig* Scene, rapidjson::Value& Obj)
{
    rapidjson::Value::ConstMemberIterator it = Obj.MemberBegin();
    for (; it != Obj.MemberEnd(); it++ )
    {
        //取得物件名稱;//
        string Name = it->name.GetString();

        //若物件名稱為__HELP表示是說明列,不予解析資料;//
        if (Name.compare(ccHelp) == 0 )
            continue;

        //檢查格式是否為Array;//
        if (!Obj[Name.c_str()].IsArray())
            return false;

        //解析Music資料;//
        MusicData* Data = ParseArrayMusic(Obj[Name.c_str()]);
        if (Data == NULL)
            return false;

        //儲存音樂資料;//
        Data->strJsonTag = Name;
        Scene->SetMusic(*Data);
        delete Data;
        Data = NULL;
    }
    return true;
}

bool oConfig::ParseObjImage(oSceneConfig* Scene, rapidjson::Value& Obj)
{    
    rapidjson::Value::ConstMemberIterator it = Obj.MemberBegin();
    for (; it != Obj.MemberEnd(); it++)
    {
        //取得物件名稱;//
        string Name = it->name.GetString();

        //若物件名稱為__HELP表示是說明列,不予解析資料;//
        if (Name.compare(ccHelp) == 0)
            continue;

        //檢查格式是否為Array;//
        if (!Obj[Name.c_str()].IsArray())
            return false;
           
        //解析Image資料;//
        ImageData* Data = ParseArrayImage(Obj[Name.c_str()]);
        if (Data == NULL)
            return false;

        //儲存音樂資料;//
        Data->strJsonTag = Name;
        Scene->SetImage(*Data);
        delete Data;
        Data = NULL;
    }

    return true;
}

bool oConfig::ParseObjCharacter(oSceneConfig* Scene, rapidjson::Value& Obj)
{
    //取得各腳色資料;//
    rapidjson::Value::ConstMemberIterator itCharacter = Obj.MemberBegin();
    for (; itCharacter != Obj.MemberEnd(); itCharacter++)
    {
        //格式Sample;//
        //"Cabbage" :
        //{
        //    "ImgBody" : ["Cabbage.png", 2, ["CabbageTouch.mp3", 1, true, false]],
        //    "ImgFall" : ["CabbageFall.png", 1, ["CabbageFly.mp3", 1, false, false]],
        //    "ImgBomb" : ["Bomb.png", 1, ["CabbageBomb.mp3", 1, false, false]],
        //    "Satellite" : 0,
        //    "Speed" : 10
        //},

        //取得單一腳色資料;//
        rapidjson::Value& Item = Obj[itCharacter->name.GetString()];
        if (!Item.IsObject())
            return false;

        if (Item.MemberCount() != 5)
            return false;

        CharacterData Data;
        ImageData *ptrImgData;

        //解析Body圖資;//
        if (!Item.HasMember("ImgBody"))
            return false;
        ptrImgData = ParseArrayImage( Item["ImgBody"], false );
        if (ptrImgData == NULL)
            return false;
        Data.Body = *ptrImgData;
        delete ptrImgData;

        //解析Fall圖資;//
        if (!Item.HasMember("ImgFall"))
            return false;
        ptrImgData = ParseArrayImage(Item["ImgFall"], false);
        if (ptrImgData == NULL)
            return false;
        Data.Fall = *ptrImgData;
        delete ptrImgData;

        //解析Bomb圖資;//
        if (!Item.HasMember("ImgFall"))
            return false;
        ptrImgData = ParseArrayImage(Item["ImgFall"], false);
        if (ptrImgData == NULL)
            return false;
        Data.Bomb = *ptrImgData;
        delete ptrImgData;

        //解析衛星數量;//
        if (!Item.HasMember("Satellite"))
            return false;
        Data.nSatellite = Item["Satellite"].GetInt();

        //解析移動速度;//
        if (!Item.HasMember("Speed"))
            return false;
        Data.nSpeed = Item["Speed"].GetInt();

        //修改檔案路徑;//
        string CurrentDir = itCharacter->name.GetString();
        CurrentDir.append("/");
        Data.Body.strFilePath.insert(0, CurrentDir);
        Data.Fall.strFilePath.insert(0, CurrentDir);
        Data.Bomb.strFilePath.insert(0, CurrentDir);
        Data.Body.Music.strFilePath.insert(0, CurrentDir);
        Data.Fall.Music.strFilePath.insert(0, CurrentDir);
        Data.Bomb.Music.strFilePath.insert(0, CurrentDir);

        //儲存腳色資訊;//
        Data.strJsonTag.append(itCharacter->name.GetString());
        Scene->SetCharacter(Data);
    }

    return true;
}

MusicData* oConfig::ParseArrayMusic( rapidjson::Value& Array, bool isUseDefaultDir)
{
    //格式Sample: "BgLogo":["", 1, false] ;//
    if (Array.Size() != 4)
        return NULL;

    if (!( Array[0].IsString()
        && Array[1].IsInt()
        && Array[2].IsBool()
        && Array[3].IsBool()))
    {
        //表示資料格式有誤;//
        return NULL;
    }

    MusicData* Data = new MusicData;

    if (isUseDefaultDir)
        Data->strFilePath.append(m_strDirMusic);
    Data->strFilePath.append(Array[0].GetString());
    Data->nPlayTime = Array[1].GetInt();
    Data->isLoop = Array[2].GetBool();
    Data->isMusic = Array[3].GetBool();
    
    return Data;
}

ImageData* oConfig::ParseArrayImage( rapidjson::Value& Array, bool isUseDefaultDir)
{
    //格式Sample: "Logo":["LOGO.png", 1, "0", 0, false]

    if (Array.Size() != 3)
        return NULL;

    if (!(Array[0].IsString()
        && Array[1].IsInt()
        && Array[2].IsArray() ) )
        return NULL;

    //先解析音樂資料;//
    MusicData* ptrMusicData = ParseArrayMusic(Array[2], isUseDefaultDir );
    if (ptrMusicData == NULL)
        return NULL;
        
    //取得資料路徑及影格數目;//
    ImageData *ptrImageData = new ImageData;
    if (isUseDefaultDir)
        ptrImageData->strFilePath.append(m_strDirImage);
    ptrImageData->strFilePath.append(Array[0].GetString());
    ptrImageData->nAniNum = Array[1].GetInt();
    ptrImageData->Music = *ptrMusicData;

    delete ptrMusicData;
    ptrMusicData = NULL;

    //格式 Sample: "Logo":["LOGO.png", 1, "0", 0, false]
    return ptrImageData;
}

