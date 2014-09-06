#ifndef EVENTS_OBJECTIVEREACHED_HPP
#define EVENTS_OBJECTIVEREACHED_HPP

  #include "framework.hpp"

  #include "../Event.hpp"


  namespace event
  {
    class ObjectiveReached: public Event
    {
      public:

        ObjectiveReached(EVENT_STARTTYPE starttype);
        ~ObjectiveReached();


      private:

        /* Data */
    };
  }

#endif // OBJECTIVEREACHEDEVENT_HPP
