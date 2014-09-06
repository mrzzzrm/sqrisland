#include "./Mainmenu.hpp"

using namespace audio;
using namespace std;
using namespace gfx;
using namespace error;
using namespace util;
using namespace input;
using namespace geo;

Animation Mainmenu::static_data::background;
Animation Mainmenu::static_data::new_selected;
Animation Mainmenu::static_data::new_unselected;
Animation Mainmenu::static_data::load_selected;
Animation Mainmenu::static_data::load_unselected;
Animation Mainmenu::static_data::load_unavailable;
Animation Mainmenu::static_data::quit_selected;
Animation Mainmenu::static_data::quit_unselected;
Animation Mainmenu::static_data::new_header;
Animation Mainmenu::static_data::yes_selected;
Animation Mainmenu::static_data::yes_unselected;
Animation Mainmenu::static_data::no_selected;
Animation Mainmenu::static_data::no_unselected;
Sound Mainmenu::static_data::click;

void Mainmenu::loadStaticData()
{
  try
  {
    static_data::background.load("dat/menu/main/background.ani");
    static_data::new_selected.load("dat/menu/main/new_selected.ani");
    static_data::new_unselected.load("dat/menu/main/new_unselected.ani");
    static_data::load_selected.load("dat/menu/main/load_selected.ani");
    static_data::load_unselected.load("dat/menu/main/load_unselected.ani");
    static_data::load_unavailable.load("dat/menu/main/load_unavailable.ani");
    static_data::quit_selected.load("dat/menu/main/quit_selected.ani");
    static_data::quit_unselected.load("dat/menu/main/quit_unselected.ani");
    static_data::new_header.load("dat/menu/main/new_header.ani");
    static_data::yes_selected.load("dat/menu/main/yes_selected.ani");
    static_data::yes_unselected.load("dat/menu/main/yes_unselected.ani");
    static_data::no_selected.load("dat/menu/main/no_selected.ani");
    static_data::no_unselected.load("dat/menu/main/no_unselected.ani");
    static_data::click.load("dat/audio/sounds/click.ogg");

  }
  catch(Exception &exception)
  {
    throw Exception() << "Error whilst loading static data for Mainmenu: " << exception;
  }
}

void Mainmenu::clearStaticData()
{
  static_data::background.clear();
  static_data::new_selected.clear();
  static_data::new_unselected.clear();
  static_data::load_selected.clear();
  static_data::load_unselected.clear();
  static_data::quit_selected.clear();
  static_data::quit_unselected.clear();
  static_data::new_header.clear();
  static_data::yes_selected.clear();
  static_data::yes_unselected.clear();
  static_data::no_selected.clear();
  static_data::no_unselected.clear();
  static_data::click.clear();
}

Mainmenu::Mainmenu()
{
  fading.setTicks(750);
  fading.setColor(BLACK);
  fading.setSize(IntSize(-1, -1));
  fading.forceStart(FADE_OUT);

  state = MAINMENU_STATE_IN;
  last_state = MAINMENU_STATE_IN;
  mainselection = MAINMENU_NEW;
  start_new = false;
  load_available = false;
}

Mainmenu::~Mainmenu()
{

}

void Mainmenu::setLoadAvailable(bool is)
{
  load_available = is;
}

bool Mainmenu::isSelectionDone() const
{
  return state == MAINMENU_STATE_OUT && fading.isDone();
}

MAINMENU_OPTION Mainmenu::getSelection() const
{
  return mainselection;
}

void Mainmenu::update()
{
  switch(state)
  {
    case MAINMENU_STATE_IN:
    {
      fading.update();

      if(fading.isDone())
        state = MAINMENU_STATE_MAINSELECTION;
    }

    case MAINMENU_STATE_MAINSELECTION:
    {
      if(Input::isButtonPressed(BUTTON_DOWN))
      {
        if((int)mainselection < (int)MAINMENU_QUIT)
        {
          if((int)mainselection == (int)MAINMENU_LOAD - 1 && !load_available)
            mainselection = (MAINMENU_OPTION)((int)mainselection + 2);
          else
            mainselection = (MAINMENU_OPTION)((int)mainselection + 1);
        }
      }
      if(Input::isButtonPressed(BUTTON_UP))
      {
        if((int)mainselection > 0)
        {
          if((int)mainselection == (int)MAINMENU_LOAD + 1 && !load_available)
            mainselection = (MAINMENU_OPTION)((int)mainselection - 2);
          else
            mainselection = (MAINMENU_OPTION)((int)mainselection - 1);
        }
      }
      if(Input::isButtonPressed(BUTTON_B))
      {
        SoundPlayer::play(static_data::click);

        if(mainselection == MAINMENU_NEW && load_available)
        {
          state = MAINMENU_STATE_NEWSELECTION;
        }
        else
        {
          fading.forceStart(FADE_IN);
          state = MAINMENU_STATE_OUT;
          last_state = MAINMENU_STATE_MAINSELECTION;
        }
      }
    }
    break;

    case MAINMENU_STATE_NEWSELECTION:
    {
      if(Input::isButtonPressed(BUTTON_DOWN) || Input::isButtonPressed(BUTTON_UP))
      {
        start_new = !start_new;
      }
      if(Input::isButtonPressed(BUTTON_B))
      {
        SoundPlayer::play(static_data::click);

        if(start_new)
        {
          fading.forceStart(FADE_IN);
          state = MAINMENU_STATE_OUT;
          last_state = MAINMENU_STATE_NEWSELECTION;
        }
        else
        {
          state = MAINMENU_STATE_MAINSELECTION;
        }
      }
      if(Input::isButtonPressed(BUTTON_X))
      {
        state = MAINMENU_STATE_MAINSELECTION;
      }
    }
    break;

    case MAINMENU_STATE_OUT:
    {
      fading.update();
    }
    break;
  }

}

void Mainmenu::draw()
{
  static_data::background.draw(IntPoint(0, 0));

  if(state == MAINMENU_STATE_MAINSELECTION || state == MAINMENU_STATE_IN || (state == MAINMENU_STATE_OUT && last_state == MAINMENU_STATE_MAINSELECTION))
  {
    if(mainselection == MAINMENU_NEW)
      static_data::new_selected.draw(IntPoint(450, 162));
    else
      static_data::new_unselected.draw(IntPoint(450, 162));

    if(load_available)
    {
      if(mainselection == MAINMENU_LOAD)
        static_data::load_selected.draw(IntPoint(440, 237));
      else
        static_data::load_unselected.draw(IntPoint(440, 237));
    }
    else
    {
      static_data::load_unavailable.draw(IntPoint(440, 237));
    }

    if(mainselection == MAINMENU_QUIT)
      static_data::quit_selected.draw(IntPoint(430, 322));
    else
      static_data::quit_unselected.draw(IntPoint(430, 322));
  }

  if(state == MAINMENU_STATE_NEWSELECTION || (state == MAINMENU_STATE_OUT && last_state == MAINMENU_STATE_NEWSELECTION))
  {
    static_data::new_header.draw(IntPoint(410, 162));

    if(start_new)
      static_data::yes_selected.draw(IntPoint(440, 237));
    else
      static_data::yes_unselected.draw(IntPoint(440, 237));

    if(!start_new)
      static_data::no_selected.draw(IntPoint(450, 322));
    else
      static_data::no_unselected.draw(IntPoint(450, 322));
  }

  if(state == MAINMENU_STATE_OUT || state == MAINMENU_STATE_IN)
    fading.draw();
}

