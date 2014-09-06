#ifndef LEVELINTRO_HPP
#define LEVELINTRO_HPP


  #include "framework.hpp"

  enum LEVELINTRO_STATE
  {
    LEVELINTRO_STATE_INACTIVE,
    LEVELINTRO_STATE_IN,
    LEVELINTRO_STATE_MAIN,
    LEVELINTRO_STATE_OUT,
  };

  class LevelIntro
  {
    public:

      static void loadStaticData();
      static void clearStaticData();


      LevelIntro(const util::Timer &timer);
      ~LevelIntro();


      void start();
      bool isActive() const;

      void setLine0(const std::string &text);
      void setLine1(const std::string &text);

      void update();
      void draw();

    private:

      /* Static Data */
      struct static_data
      {
        static font::TtfFont line_0_font;
        static font::TtfFont line_1_font;
      };

      /* Data */
      const util::Timer &timer;
      LEVELINTRO_STATE state;
      gfx::ColorFading fading;
      font::TtfLabel line_0;
      font::TtfLabel line_1;
      double line_0_x, line_1_x;
  };

#endif
