#include "./locals.hpp"

using namespace std;
using namespace error;


LANGUAGE language_string_to_type(const string &string)
{
  if(string == "ger")
    return GERMAN;
  else if(string == "eng")
    return ENGLISH;
  else
    throw Exception() << "Unknown language type '" << string << "'";
}

std::string language_type_to_string(LANGUAGE language)
{
  if(language == GERMAN)
    return "ger";
  else if(language == ENGLISH)
    return "eng";
  else
    throw Exception() << "Unknown language type '" << language << "'";
}
