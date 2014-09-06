#ifndef EVENTS_TEXTBOX_HPP
#define EVENTS_TEXTBOX_HPP

  #include "framework.hpp"
  #include "../Event.hpp"



  namespace event
  {
    class Textbox: public Event
    {
      public:

        Textbox(EVENT_PLAYTYPE playtype, EVENT_STARTTYPE starttype, std::string text);
        ~Textbox();

        const std::string &getText() const;

      private:

        std::string text;
    };
  }

#endif
