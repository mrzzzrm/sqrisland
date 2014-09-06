#ifndef JUMP_HPP
#define JUMP_HPP

  #include "framework.hpp"


  enum JUMP_TYPE
  {
    JUMP_BY_KEY,
    JUMP_BY_SPRING
  };

  class Jump
  {
    public:

      Jump(const util::Timer &timer);
      ~Jump();


      void start(time_t dur);
      void end();

      bool is() const;

      bool hasLeftGround() const;
      void setLeftGround(bool is);

      bool isBreakable() const;
      void setBreakable(bool is);

      double getRelativeForce() const;

      void update();


    private:

      /* Data */
      const util::Timer &timer;
      bool _is;
      bool left_ground;
      bool breakable;
      time_t begin, dur;
  };

#endif // JUMP_HPP
