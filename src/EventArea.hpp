#ifndef EVENTAREA_HPP
#define EVENTAREA_HPP

  #include <cassert>
  #include "framework.hpp"
  #include "./Object.hpp"
  #include "./Event.hpp"


  class EventArea: public Object
  {
    public:

      EventArea(const util::Timer &timer, Event *event, const geo::IntRect &rect);
      ~EventArea();

      const Event &getEvent() const;
      const geo::IntRect &getRect() const;
      bool isEntered() const;

      void setEntered(bool is);

      void draw(const Camera &camera);


    private:

      Event *event;
      geo::IntRect rect;
      bool entered;
  };


#endif
