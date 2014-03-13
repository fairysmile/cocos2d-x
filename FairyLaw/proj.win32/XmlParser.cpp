#include "XMLParser.h"
 
using namespace std;
using namespace cocos2d;
 
// 空格
const static int SPACE = 32;
// 换行
const static int NEXTLINE = 10;
// tab 横向制表符
const static int TAB = 9;
 
XMLParser* XMLParser::parseWithFile(const char *xmlFileName)
{
    XMLParser *pXMLParser = new XMLParser();
    if( pXMLParser->initWithFile(xmlFileName) )
    {
        pXMLParser->autorelease();   
        return pXMLParser;
    }
    CC_SAFE_DELETE(pXMLParser);
    return NULL;
}
 
bool XMLParser::initWithFile(const char *xmlFileName)
{
    m_pDictionary = new CCDictionary();
    CCSAXParser _parser;
    _parser.setDelegator(this);
    const char *fullPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(xmlFileName);
    return _parser.parse(fullPath);
}
 
XMLParser* XMLParser::parseWithString(const char *content)
{
    XMLParser *pXMLParser = new XMLParser();
    if( pXMLParser->initWithString(content) )
    {
        pXMLParser->autorelease();   
        return pXMLParser;
    }
    CC_SAFE_DELETE(pXMLParser);
    return NULL;
}
 
bool XMLParser::initWithString(const char *content)
{
    m_pDictionary = new CCDictionary();
    CCSAXParser _parse;
    _parse.setDelegator(this);
    return _parse.parse(content, strlen(content) );
}
 
void XMLParser::startElement(void *ctx, const char *name, const char **atts)
{
    this->startXMLElement = (char *)name;
    if(this->startXMLElement == "string")
    {
        while(atts && *atts)
        {
            const char *attsKey = *atts;    
            if(0 == strcmp(attsKey, "name"))
            {
                ++ atts;
                const char *attsValue = *atts;
                m_key = attsValue;
                break;
            }
            ++ atts;
        }
 
    }
 
}
 
void XMLParser::endElement(void *ctx, const char *name)
{
    this->endXMLElement = (char *)name;
}
 
void XMLParser::textHandler(void *ctx, const char *s, int len)
{
    string value((char *)s, 0, len);
    //是否全是非正常字符
    bool noValue = true;
    for(int i = 0; i < len; ++i)
    {
        if(s[i] != SPACE && s[i] != NEXTLINE && s[i] != TAB)
        {
            noValue = false;    
            break;
        }
    }
    if(noValue) return;
    CCString *pString = CCString::create(value);
    this->m_pDictionary->setObject(pString, this->m_key);
}
 
CCString* XMLParser::getString(const char *key)
{
    string strKey(key);
    return (CCString *)this->m_pDictionary->objectForKey(strKey);
}
 
XMLParser::XMLParser()
{
}
 
XMLParser::~XMLParser()
{
    CC_SAFE_DELETE(this->m_pDictionary);
}