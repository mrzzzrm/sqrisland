#ifndef EVENTS_CHECKPOINT_HPP
#define EVENTS_CHECKPOINT_HPP

  #include "framework.hpp"

  #include "../Event.hpp"

  namespace event
  {
    class Checkpoint: public Event
    {
      public:

        Checkpoint(const std::string &relative_reappearposition, const geo::IntSize &tilesize);
        ~Checkpoint();

        const geo::IntPoint &getRelativeReappearPosition() const;


      private:

        /* Data */
        geo::IntPoint relative_reappearposition;
    };
  }

#endif // CHECKPOINT_HPP
