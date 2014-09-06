#include "./ObjectiveReached.hpp"


namespace event
{

  ObjectiveReached::ObjectiveReached(EVENT_STARTTYPE starttype):
    Event(EVENT_OBJECTIVE_REACHED, EVENT_PLAY_ALWAYS, starttype)
  {

  }

  ObjectiveReached::~ObjectiveReached()
  {

  }

}
