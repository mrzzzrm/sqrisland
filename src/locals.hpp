#ifndef LOCALS_HPP
#define LOCALS_HPP

  #include "framework.hpp"


  enum LANGUAGE
  {
    GERMAN,
    ENGLISH
  };

  LANGUAGE language_string_to_type(const std::string &string);
  std::string language_type_to_string(LANGUAGE language);

#endif
