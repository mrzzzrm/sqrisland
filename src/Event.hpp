#ifndef EVENT_HPP
#define EVENT_HPP

  #include "framework.hpp"


  enum EVENT_TYPE
  {
    EVENT_TEXTBOX,
    EVENT_CHECKPOINT,
    EVENT_OBJECTIVE_REACHED
  };

  enum EVENT_PLAYTYPE
  {
    EVENT_PLAY_ONCE,
    EVENT_PLAY_ALWAYS
  };

  enum EVENT_STARTTYPE
  {
    EVENT_START_MANUAL,
    EVENT_START_AUTOMATIC
  };


  class Event
  {
    public:

      Event(EVENT_TYPE type, EVENT_PLAYTYPE playtype, EVENT_STARTTYPE starttype);
      ~Event();

      virtual EVENT_TYPE getType() const;
      EVENT_PLAYTYPE getPlayType() const;
      EVENT_STARTTYPE getStartType() const;


    private:

      EVENT_TYPE type;
      EVENT_PLAYTYPE playtype;
      EVENT_STARTTYPE starttype;
  };


  EVENT_TYPE event_string_to_type(const std::string &string);

#endif
