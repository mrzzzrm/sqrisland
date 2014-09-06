#ifndef MAINMENU_HPP
#define MAINMENU_HPP

  #include "framework.hpp"

  enum MAINMENU_STATE
  {
    MAINMENU_STATE_IN,
    MAINMENU_STATE_MAINSELECTION,
    MAINMENU_STATE_NEWSELECTION,
    MAINMENU_STATE_OUT
  };

  enum MAINMENU_OPTION
  {
    MAINMENU_NEW,
    MAINMENU_LOAD,
    MAINMENU_QUIT
  };


  class Mainmenu
  {
    public:

      static void loadStaticData();
      static void clearStaticData();

      Mainmenu();
      ~Mainmenu();

      void setLoadAvailable(bool is);
      bool isSelectionDone() const;
      MAINMENU_OPTION getSelection() const;

      void update();
      void draw();



    private:

      /* Static data */
      struct static_data
      {
        static gfx::Animation background;
        static gfx::Animation new_selected, new_unselected;
        static gfx::Animation load_selected, load_unselected, load_unavailable;
        static gfx::Animation quit_selected, quit_unselected;

        static gfx::Animation new_header;
        static gfx::Animation yes_selected, yes_unselected;
        static gfx::Animation no_selected, no_unselected;

        static audio::Sound click;
      };


      /* Data */
      util::Timer timer;
      gfx::ColorFading fading;
      MAINMENU_STATE state;
      MAINMENU_STATE last_state;
      bool load_available;
      MAINMENU_OPTION mainselection;
      bool start_new;
  };


#endif // MAINMENU_HPP
