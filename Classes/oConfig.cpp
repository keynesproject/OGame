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

    //Ū��Json�ɮ�;//
    std::string strJson = cocos2d::FileUtils::getInstance()->getStringFromFile("Config.json");
    if (strJson.empty())
        return false;

    //�ѪRJSON�ɮ�;//
    rapidjson::Document JsonDoc;
    JsonDoc.Parse<rapidjson::kParseDefaultFlags>(strJson.c_str());

    //�P�_�O�_�ѪR���~;//
    if (JsonDoc.HasParseError())
        return false;

    //�P�_�O�_��Object;//
    if (!JsonDoc.IsObject())
        return false;

    //���o��Ƨ��������|;//
    ParseBaseDir(JsonDoc);

    //���o�r�����|;//
    ParseFont(JsonDoc);

    //Ū���æUScene���;//
    rapidjson::Value::ConstMemberIterator it = JsonDoc.MemberBegin();
    for (; it != JsonDoc.MemberEnd(); it++ )
    {
        //�P�_�O�_��Object�榡;//
        string Name = it->name.GetString();
        if (JsonDoc[Name.c_str()].IsObject())
        {
            //�ѪRScene���;//
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

    //���o�r�����|;//
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

    //�ѪR���ָ��;//
    if (ObjScene.HasMember("Music"))
    {
        if (!ParseObjMusic(Scene, ObjScene["Music"]))
        {
            delete Scene;
            return NULL;
        }
    }

    //�ѪR�ϧθ��;//
    if (ObjScene.HasMember("Image"))
    {
        if (!ParseObjImage(Scene, ObjScene["Image"]))
        {
            delete Scene;
            return NULL;
        }
    }

    //�ѪR�}����;//
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
        //���o����W��;//
        string Name = it->name.GetString();

        //�Y����W�٬�__HELP��ܬO�����C,�����ѪR���;//
        if (Name.compare(ccHelp) == 0 )
            continue;

        //�ˬd�榡�O�_��Array;//
        if (!Obj[Name.c_str()].IsArray())
            return false;

        //�ѪRMusic���;//
        MusicData* Data = ParseArrayMusic(Obj[Name.c_str()]);
        if (Data == NULL)
            return false;

        //�x�s���ָ��;//
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
        //���o����W��;//
        string Name = it->name.GetString();

        //�Y����W�٬�__HELP��ܬO�����C,�����ѪR���;//
        if (Name.compare(ccHelp) == 0)
            continue;

        //�ˬd�榡�O�_��Array;//
        if (!Obj[Name.c_str()].IsArray())
            return false;
           
        //�ѪRImage���;//
        ImageData* Data = ParseArrayImage(Obj[Name.c_str()]);
        if (Data == NULL)
            return false;

        //�x�s���ָ��;//
        Data->strJsonTag = Name;
        Scene->SetImage(*Data);
        delete Data;
        Data = NULL;
    }

    return true;
}

bool oConfig::ParseObjCharacter(oSceneConfig* Scene, rapidjson::Value& Obj)
{
    //���o�U�}����;//
    rapidjson::Value::ConstMemberIterator itCharacter = Obj.MemberBegin();
    for (; itCharacter != Obj.MemberEnd(); itCharacter++)
    {
        //�榡Sample;//
        //"Cabbage" :
        //{
        //    "ImgBody" : ["Cabbage.png", 2, ["CabbageTouch.mp3", 1, true, false]],
        //    "ImgFall" : ["CabbageFall.png", 1, ["CabbageFly.mp3", 1, false, false]],
        //    "ImgBomb" : ["Bomb.png", 1, ["CabbageBomb.mp3", 1, false, false]],
        //    "Satellite" : 0,
        //    "Speed" : 10
        //},

        //���o��@�}����;//
        rapidjson::Value& Item = Obj[itCharacter->name.GetString()];
        if (!Item.IsObject())
            return false;

        if (Item.MemberCount() != 5)
            return false;

        CharacterData Data;
        ImageData *ptrImgData;

        //�ѪRBody�ϸ�;//
        if (!Item.HasMember("ImgBody"))
            return false;
        ptrImgData = ParseArrayImage( Item["ImgBody"], false );
        if (ptrImgData == NULL)
            return false;
        Data.Body = *ptrImgData;
        delete ptrImgData;

        //�ѪRFall�ϸ�;//
        if (!Item.HasMember("ImgFall"))
            return false;
        ptrImgData = ParseArrayImage(Item["ImgFall"], false);
        if (ptrImgData == NULL)
            return false;
        Data.Fall = *ptrImgData;
        delete ptrImgData;

        //�ѪRBomb�ϸ�;//
        if (!Item.HasMember("ImgFall"))
            return false;
        ptrImgData = ParseArrayImage(Item["ImgFall"], false);
        if (ptrImgData == NULL)
            return false;
        Data.Bomb = *ptrImgData;
        delete ptrImgData;

        //�ѪR�ìP�ƶq;//
        if (!Item.HasMember("Satellite"))
            return false;
        Data.nSatellite = Item["Satellite"].GetInt();

        //�ѪR���ʳt��;//
        if (!Item.HasMember("Speed"))
            return false;
        Data.nSpeed = Item["Speed"].GetInt();

        //�ק��ɮ׸��|;//
        string CurrentDir = itCharacter->name.GetString();
        CurrentDir.append("/");
        Data.Body.strFilePath.insert(0, CurrentDir);
        Data.Fall.strFilePath.insert(0, CurrentDir);
        Data.Bomb.strFilePath.insert(0, CurrentDir);
        Data.Body.Music.strFilePath.insert(0, CurrentDir);
        Data.Fall.Music.strFilePath.insert(0, CurrentDir);
        Data.Bomb.Music.strFilePath.insert(0, CurrentDir);

        //�x�s�}���T;//
        Data.strJsonTag.append(itCharacter->name.GetString());
        Scene->SetCharacter(Data);
    }

    return true;
}

MusicData* oConfig::ParseArrayMusic( rapidjson::Value& Array, bool isUseDefaultDir)
{
    //�榡Sample: "BgLogo":["", 1, false] ;//
    if (Array.Size() != 4)
        return NULL;

    if (!( Array[0].IsString()
        && Array[1].IsInt()
        && Array[2].IsBool()
        && Array[3].IsBool()))
    {
        //��ܸ�Ʈ榡���~;//
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
    //�榡Sample: "Logo":["LOGO.png", 1, "0", 0, false]

    if (Array.Size() != 3)
        return NULL;

    if (!(Array[0].IsString()
        && Array[1].IsInt()
        && Array[2].IsArray() ) )
        return NULL;

    //���ѪR���ָ��;//
    MusicData* ptrMusicData = ParseArrayMusic(Array[2], isUseDefaultDir );
    if (ptrMusicData == NULL)
        return NULL;
        
    //���o��Ƹ��|�μv��ƥ�;//
    ImageData *ptrImageData = new ImageData;
    if (isUseDefaultDir)
        ptrImageData->strFilePath.append(m_strDirImage);
    ptrImageData->strFilePath.append(Array[0].GetString());
    ptrImageData->nAniNum = Array[1].GetInt();
    ptrImageData->Music = *ptrMusicData;

    delete ptrMusicData;
    ptrMusicData = NULL;

    //�榡 Sample: "Logo":["LOGO.png", 1, "0", 0, false]
    return ptrImageData;
}

