#include "./TextManager.hpp"

using namespace std;
using namespace error;
using namespace util;
using namespace xml;


TextManager::TextManager()
{
  language = ENGLISH;
}

TextManager::~TextManager()
{

}

string TextManager::getText(const string &text)
{
  string file, key;
  size_t dotpos;

  dotpos = text.find(".");
  if(dotpos == text.npos)
    throw Exception() << "No '.' in textkey '" << text << ".";

  file = text.substr(0, dotpos);
  key = text.substr(dotpos + 1, text.npos);

  /* Check for file */
  try
  {
    if(texts.count(file) == 0)
      read_file(file);
  }
  catch(Exception ex)
  {
    throw Exception() << "TextManager couldn't load '" << file << "': " << ex.getDescription();
  }

  /* Check for key */
  if(texts[file].count(key) == 0)
    throw Exception() << "No such key '" << key << "' in file '" << file << "'";


  /* Check for language */
  if(texts[file][key].count(language) == 0)
    throw Exception() << "No such language '" << language_type_to_string(language) << "' for key '" << key << "' in file '" << file << "'";

  return texts[file][key][language];
}

void TextManager::setLanguage(LANGUAGE language)
{
  this->language = language;
}

void TextManager::read_file(const string &file)
{
  Xml xml;

  xml.load(Path(string("./dat/texts/") + file + string(".xml")));
  xml.toRootBlock("texts");

  texts[file] =  map<string, map<LANGUAGE, string> >();


  if(xml.toSubBlock("text"))
  {
    for(int t = 0; t > 0 ? xml.toNextBlock("text") : true; t++)
    {
      string key;

      key = xml.getString("key");
      texts[file][key] = map<LANGUAGE, string>();

      if(xml.toSubBlock())
      {
        for(int t = 0; t > 0 ? xml.toNextBlock() : true; t++)
        {
          LANGUAGE language;

          language = language_string_to_type(xml.getBlockValue());

          xml.toSubBlock();
          texts[file][key][language] = xml.getBlockValue();
          xml.toBaseBlock();
        }
      }

      xml.toBaseBlock();
    }
  }

}
