#ifndef LEVELINTERFACE_HPP
#define LEVELINTERFACE_HPP

  #include <cassert>
  #include "framework.hpp"



  class LevelInterface
  {
    public:

      static void loadStaticData();
      static void clearStaticData();


      LevelInterface(const util::Timer &timer);
      ~LevelInterface();

      void setCoincount(int count);
      void setLifecount(int count);

      void sigCoin();
      void sigLife();
      void sigKilled();

      void update();
      void draw();


    private:

      /* Static data */
      struct static_data
      {
        static font::TtfFont font;
        static gfx::Animation life_icon;
        static gfx::Animation coin_icon;
      };

      /* Data */
      const util::Timer &timer;
      int coincount;
      int lifecount;
      gfx::Image *coincount_img;
      gfx::Image *lifecount_img;

      /* Internals */
      void renderCoincount();
      void renderLifecount();
  };

#endif
