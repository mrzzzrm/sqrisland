#ifndef LEVELMENU_HPP
#define LEVELMENU_HPP

  #include "framework.hpp"

  enum LEVELMENU_STATE
  {
    LEVELMENU_STATE_INACTIVE,
    LEVELMENU_STATE_IN,
    LEVELMENU_STATE_MAIN,
    LEVELMENU_STATE_OUT
  };

  enum LEVELMENU_SELECTION
  {
    LEVELMENU_RESUME,
    LEVELMENU_QUIT
  };

  class LevelMenu
  {
    public:

      static void loadStaticData();
      static void clearStaticData();

      LevelMenu();
      ~LevelMenu();

      void activate();
      bool isSelectionDone() const;
      LEVELMENU_SELECTION getSelection() const;
      LEVELMENU_STATE getState() const;

      void update();
      void draw();


    private:

      /* Static data */
      struct static_data
      {
        static gfx::Animation background;
        static gfx::Animation resume_selected, resume_unselected;
        static gfx::Animation quit_selected, quit_unselected;
        static audio::Sound click;
      };

      /* Data */
      util::Timer timer;
      double x;
      LEVELMENU_SELECTION selection;
      LEVELMENU_STATE state;
  };


#endif // LEVELMENU_HPP
