#ifndef INTRO_HPP
#define INTRO_HPP

  #include "framework.hpp"

  class Intro
  {
    public:

      Intro();
      ~Intro();

      void load();
      void in();
      void out();

    private:

      /* Data */
      util::Timer timer;
      gfx::Image img;
      gfx::ImageFading fading;
      time_t in_complete;
  };


#endif // INTRO_HPP
