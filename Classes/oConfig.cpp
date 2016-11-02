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

FontData* oSceneConfig::GetFont(string strName)
{
    vector< FontData >::iterator it = m_FontData.begin();
    for (; it != m_FontData.end(); it++)
    {
        if (strName.compare(it->strJsonTag) == 0)
        {
            return &(*it);
        }
    }

    return NULL;
}

MusicData* oSceneConfig::GetMusic(string strName)
{
    vector< MusicData >::iterator it = m_MusicData.begin();
    for (; it != m_MusicData.end(); it++)
    {
        if (strName.compare(it->strJsonTag) == 0)
        {
            return &(*it);
        }
    }

    return NULL;
}

ImageData* oSceneConfig::GetImage(string strName)
{
    vector< ImageData >::iterator it = m_ImageData.begin();
    for (; it != m_ImageData.end(); it++)
    {
        if (strName.compare(it->strJsonTag) == 0)
        {
            return &(*it);
        }
    }

    return NULL;
}

CharacterData* oSceneConfig::GetCharacter(string strName)
{
    vector< CharacterData >::iterator it = m_CharacterData.begin();
    for (; it != m_CharacterData.end(); it++)
    {
        if (strName.compare(it->strJsonTag) == 0)
        {
            return &(*it);
        }
    }

    return NULL;
}

vector< FontData >& oSceneConfig::GetALLFont()
{
    return m_FontData;
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

void oSceneConfig::SetFont(FontData Data)
{
    m_FontData.push_back(Data);
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

oSceneConfig* oConfig::GetSceneConfig(string strSceneName)
{
    //先判斷是否解析Config完畢;//
    if (!LoadJsonConfig())
        return NULL;

    map< string, oSceneConfig* >::iterator it = m_SceneData.find(strSceneName);
    if (it != m_SceneData.end())
        return it->second;

    return NULL;
}

string oConfig::GetPathImage()
{
    return m_strDirImage;
}

string oConfig::GetPathMusic()
{
    return m_strDirMusic;
}

string oConfig::GetPathCharacter()
{
    return m_strDirCharacter;
}

bool oConfig::LoadJsonConfig()
{
    if (m_IsParsed)
        return true;

    //讀取Json檔案;//
    std::string strJson = cocos2d::FileUtils::getInstance()->getStringFromFile("Config.json");
    if (strJson.empty())
    {
        SetErrorInform("Config.json file not found!");
        return false;
    }

    //解析JSON檔案;//
    rapidjson::Document JsonDoc;
    JsonDoc.Parse<rapidjson::kParseDefaultFlags>(strJson.c_str());

    //判斷是否解析錯誤;//
    if (JsonDoc.HasParseError())
    {
        SetErrorInform("Config.json format error!");
        return false;
    }

    //判斷是否為Object;//
    if (!JsonDoc.IsObject())
        return false;

    //取得資料夾分類路徑;//
    ParseBaseDir(JsonDoc);

    //取得字型路徑;//
    //ParseFont(JsonDoc);

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
            {
                SetErrorInform("Scene :", Name);
                return false;
            }

            m_SceneData.insert(pair< string, oSceneConfig*>(Name, Scene));
        }
    }
    
    m_IsParsed = true;

    return true;
}

bool oConfig::ParseBaseDir(rapidjson::Document& JsonDoc)
{    
    if (!JsonDoc.HasMember(ccDirFont))
    {
        SetErrorInform("Base dir not found : ", (string)ccDirFont);
        return false;
    }
    m_strDirFont = JsonDoc[ccDirFont].GetString();

    if (!JsonDoc.HasMember(ccDirImage))
    {
        SetErrorInform("Base dir not found : ", (string)ccDirImage);
        return false;
    }
    m_strDirImage = JsonDoc[ccDirImage].GetString();

    if (!JsonDoc.HasMember(ccDirMusic))
    {
        SetErrorInform("Base dir not found : ", (string)ccDirMusic);
        return false;
    }
    m_strDirMusic = JsonDoc[ccDirMusic].GetString();

    if (!JsonDoc.HasMember(ccDirCharacter))
    {
        SetErrorInform("Base dir not found : ", (string)ccDirCharacter);
        return false;
    }
    m_strDirCharacter = JsonDoc[ccDirCharacter].GetString();
    
    return true;
}

oSceneConfig* oConfig::ParseScene(rapidjson::Value& ObjScene)
{
    oSceneConfig *Scene = new oSceneConfig;    

    //解析字型資料;//
    if (ObjScene.HasMember("Font"))
    {
        if (!ParseObjFont(Scene, ObjScene["Font"]))
        {
            delete Scene;
            return NULL;
        }
    }

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

bool oConfig::ParseObjFont(oSceneConfig* Scene, rapidjson::Value& Obj)
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
        {
            SetErrorInform("Parse Obj Font not array: ", Name);
            return false;
        }

        //解析Font資料;//
        FontData* Data = ParseArrayFont(Obj[Name.c_str()]);
        if (Data == NULL)
        {
            SetErrorInform("Parse Obj Font : ", Name);
            return false;
        }

        //儲存字型資料;//
        Data->strJsonTag = Name;
        Scene->SetFont(*Data);
        delete Data;
        Data = NULL;
    }
    return true;
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
        {
            SetErrorInform("Parse Obj Music not array: ", Name);
            return false;
        }

        //解析Music資料;//
        MusicData* Data = ParseArrayMusic(Obj[Name.c_str()]);
        if (Data == NULL)
        {
            SetErrorInform("Parse Obj Music : ", Name);
            return false;
        }

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
        {
            SetErrorInform("Parse Obj Image not array: ", Name);
            return false;
        }
           
        //解析Image資料;//
        ImageData* Data = ParseArrayImage(Obj[Name.c_str()]);
        if (Data == NULL)
        {
            SetErrorInform("Parse Obj Image : ", Name);
            return false;
        }

        //儲存圖形資料;//
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
        {
            SetErrorInform("Parse Obj Character not object: ", (string)itCharacter->name.GetString());
            return false;
        }

        if (Item.MemberCount() != 6)
        {
            char Member[8];
            sprintf(Member, "%d", Item.MemberCount());
            SetErrorInform("Parse Obj Character Member not 5 : ", (string)Member);
            return false;
        }

        CharacterData Data;
        ImageData *ptrImgData;

        //解析Body圖資;//
        if (!Item.HasMember("ImgBody"))
        {
            SetErrorInform("Parse Obj Character not has member : ImgBody");
            return false;
        }
        ptrImgData = ParseArrayImage( Item["ImgBody"], false );
        if (ptrImgData == NULL)
            return false;
        Data.Body = *ptrImgData;
        delete ptrImgData;

        //解析Fall圖資;//
        if (!Item.HasMember("ImgFall"))
        {
            SetErrorInform("Parse Obj Character not has member : ImgFall");
            return false;
        }
        ptrImgData = ParseArrayImage(Item["ImgFall"], false);
        if (ptrImgData == NULL)
            return false;
        Data.Fall = *ptrImgData;
        delete ptrImgData;

        //解析Bomb圖資;//
        if (!Item.HasMember("ImgBomb"))
        {
            SetErrorInform("Parse Obj Character not has member : ImgBomb");
            return false;
        }
        ptrImgData = ParseArrayImage(Item["ImgBomb"], false);
        if (ptrImgData == NULL)
            return false;
        Data.Bomb = *ptrImgData;
        delete ptrImgData;

        //解析腳色種類;//
        if (!Item.HasMember("Type"))
        {
            SetErrorInform("Parse Obj Character not has member : Type");
            return false;
        }
        Data.nType = Item["Type"].GetInt();

        //解析衛星數量;//
        if (!Item.HasMember("Satellite"))
        {
            SetErrorInform("Parse Obj Character not has member : Satellite");
            return false;
        }
        Data.nSatellite = Item["Satellite"].GetInt();

        //解析移動速度;//
        if (!Item.HasMember("Speed"))
        {
            SetErrorInform("Parse Obj Character not has member : Speed");
            return false;
        }
        Data.nSpeed = Item["Speed"].GetInt();

        //修改檔案路徑;//
        string CurrentDir = m_strDirCharacter;
        CurrentDir.append(itCharacter->name.GetString());
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

FontData* oConfig::ParseArrayFont(rapidjson::Value& Array, bool isUseDefaultDir )
{
    //格式Sample: "FontTTF1" : [ "arial.ttf", true, 0, 0, 2],
    if (Array.Size() != 5)
    {
        SetErrorInform("Parse Array Font size not 5");
        return NULL;
    }

    if (!(Array[0].IsString()
        && Array[1].IsBool()
        && Array[2].IsInt()
        && Array[3].IsInt()
        && Array[4].IsInt()))
    {
        //表示資料格式有誤;//
        SetErrorInform("Parse Array Font format wrong");
        return NULL;
    }

    FontData* Data = new FontData;

    if (isUseDefaultDir)
        Data->strFilePath.append(m_strDirFont);
    Data->strFilePath.append(Array[0].GetString());
    Data->isTTf  = Array[1].GetBool();
    Data->nWidth = Array[2].GetInt();
    Data->nHigh  = Array[3].GetInt();
    Data->nAlignment = Array[4].GetInt();

    return Data;
}

MusicData* oConfig::ParseArrayMusic( rapidjson::Value& Array, bool isUseDefaultDir)
{
    //格式Sample: "BgLogo" : [ "", false, true ],
    if (Array.Size() != 3)
    {
        SetErrorInform("Parse Array Music size not 4");
        return NULL;
    }

    if (!( Array[0].IsString()
        && Array[1].IsBool()
        && Array[2].IsBool()))
    {
        //表示資料格式有誤;//
        SetErrorInform("Parse Array Music format wrong");
        return NULL;
    }

    MusicData* Data = new MusicData;

    if (isUseDefaultDir)
        Data->strFilePath.append(m_strDirMusic);
    Data->strFilePath.append(Array[0].GetString());
    Data->isLoop = Array[1].GetBool();
    Data->isMusic = Array[2].GetBool();
    
    return Data;
}

ImageData* oConfig::ParseArrayImage( rapidjson::Value& Array, bool isUseDefaultDir)
{
    //格式Sample: "Logo":[ "LOGO.png", 1, 1, ["", 0, false, false ] ]
    if (Array.Size() != 4)
    {
        SetErrorInform("Parse Array Image size not 4");
        return NULL;
    }

    if (!(Array[0].IsString()
        && Array[1].IsInt()
        && Array[2].IsDouble()
        && Array[3].IsArray()))
    {
        SetErrorInform("Parse Array Image format wrong");
        return NULL;
    }

    //先解析音樂資料;//
    MusicData* ptrMusicData = ParseArrayMusic(Array[3], isUseDefaultDir );
    if (ptrMusicData == NULL)
    {
        SetErrorInform("Parse Array Image music wrong");
        return NULL;
    }
        
    //取得資料路徑及影格數目;//
    ImageData *ptrImageData = new ImageData;
    if (isUseDefaultDir)
        ptrImageData->strFilePath.append(m_strDirImage);
    ptrImageData->strFilePath.append(Array[0].GetString());
    ptrImageData->nAniNum = Array[1].GetInt();
    ptrImageData->fAniSpeed = Array[2].GetDouble();
    ptrImageData->Music = *ptrMusicData;

    delete ptrMusicData;
    ptrMusicData = NULL;

    return ptrImageData;
}

string oConfig::GetErrorInfom()
{
    return m_strErrorInfor;
}

void oConfig::SetErrorInform(string strError, bool NewLine )
{
    if (NewLine)
        m_strErrorInfor.append("\\n");

    m_strErrorInfor.append("[ JSON ]");
    m_strErrorInfor.append(strError);
}

void oConfig::SetErrorInform(string strError, string strMember, bool NewLine )
{
    if (NewLine)
        m_strErrorInfor.append("\\n");

    m_strErrorInfor.append("[ JSON ]");
    m_strErrorInfor.append(strError);
    m_strErrorInfor.append(strMember);
}


