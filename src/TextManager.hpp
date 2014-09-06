#ifndef TEXTMANAGER_HPP
#define TEXTMANAGER_HPP

  #include "framework.hpp"
  #include "./locals.hpp"


  class TextManager
  {
    public:

      TextManager();
      ~TextManager();


      std::string getText(const std::string &text);

      void setLanguage(LANGUAGE language);


    private:

      LANGUAGE language;
      std::map<std::string, std::map<std::string, std::map<LANGUAGE, std::string> > > texts;

      void read_file(const std::string &file);
  };

#endif
