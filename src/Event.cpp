#include "./Event.hpp"

using namespace std;
using namespace error;


Event::Event(EVENT_TYPE type, EVENT_PLAYTYPE playtype, EVENT_STARTTYPE starttype)
{
  this->type = type;
  this->playtype = playtype;
  this->starttype = starttype;
}

Event::~Event()
{

}

EVENT_TYPE Event::getType() const
{
  return type;
}

EVENT_PLAYTYPE Event::getPlayType() const
{
  return playtype;
}

EVENT_STARTTYPE Event::getStartType() const
{
  return starttype;
}

EVENT_TYPE event_string_to_type(const std::string &string)
{
  if(string == "textbox")
    return EVENT_TEXTBOX;
  else if(string == "checkpoint")
    return EVENT_CHECKPOINT;
  else if(string == "objective_reached")
    return EVENT_OBJECTIVE_REACHED;
  else
    throw error::Exception() << "Unknown event-type '" << string << "'";
}
