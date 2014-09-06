#include "./LevelMenu.hpp"

using namespace std;
using namespace audio;
using namespace gfx;
using namespace util;
using namespace error;
using namespace geo;
using namespace input;

Animation LevelMenu::static_data::background;
Animation LevelMenu::static_data::resume_selected;
Animation LevelMenu::static_data::resume_unselected;
Animation LevelMenu::static_data::quit_selected;
Animation LevelMenu::static_data::quit_unselected;
Sound LevelMenu::static_data::click;

void LevelMenu::loadStaticData()
{
  try
  {
    static_data::background.load("dat/menu/level/background.ani");
    static_data::resume_selected.load("dat/menu/level/resume_selected.ani");
    static_data::resume_unselected.load("dat/menu/level/resume_unselected.ani");
    static_data::quit_selected.load("dat/menu/level/quit_selected.ani");
    static_data::quit_unselected.load("dat/menu/level/quit_unselected.ani");
    static_data::click.load("dat/audio/sounds/click.ogg");
  }
  catch(Exception &exception)
  {
    throw Exception() << "Error loading static data for the level menu: " << exception;
  }
}

void LevelMenu::clearStaticData()
{
  static_data::background.clear();
  static_data::resume_selected.clear();
  static_data::resume_unselected.clear();
  static_data::quit_selected.clear();
  static_data::quit_unselected.clear();
  static_data::click.clear();
}

LevelMenu::LevelMenu()
{
  x = 0;
  state = LEVELMENU_STATE_INACTIVE;
  selection = LEVELMENU_RESUME;
}

LevelMenu::~LevelMenu()
{

}

void LevelMenu::activate()
{
  x = -600;
  state = LEVELMENU_STATE_IN;
  selection = LEVELMENU_RESUME;
  timer.update();
}

bool LevelMenu::isSelectionDone() const
{
  return state == LEVELMENU_STATE_OUT || state == LEVELMENU_STATE_INACTIVE;
}

LEVELMENU_SELECTION LevelMenu::getSelection() const
{
  return selection;
}

LEVELMENU_STATE LevelMenu::getState() const
{
  return state;
}

void LevelMenu::update()
{
  timer.update();

  switch(state)
  {
    case LEVELMENU_STATE_IN:
    {
      x += timer.getPerSecond(1200);
      if(x >= 105)
      {
        x = 105;
        state = LEVELMENU_STATE_MAIN;
      }
    }
    break;

    case LEVELMENU_STATE_MAIN:
    {
      if(Input::isButtonPressed(BUTTON_UP) || Input::isButtonPressed(BUTTON_DOWN))
        selection = (LEVELMENU_SELECTION)!((bool)selection);

      if(Input::isButtonPressed(BUTTON_B) || Input::isButtonPressed(BUTTON_X))
      {
        state = LEVELMENU_STATE_OUT;
        SoundPlayer::play(static_data::click);
      }
    }
    break;

    case LEVELMENU_STATE_OUT:
    {
      x += timer.getPerSecond(1200);
      if(x >= 800)
      {
        state = LEVELMENU_STATE_INACTIVE;
      }
    }
    break;

    case LEVELMENU_STATE_INACTIVE:
    break;
  }

}

void LevelMenu::draw()
{
  IntVector rel(x, 40);

  static_data::background.draw(IntPoint(0, 0) + rel);

  if(selection == LEVELMENU_RESUME)
  {
    static_data::resume_selected.draw(IntPoint(130, 90) + rel);
    static_data::quit_unselected.draw(IntPoint(120, 200) + rel);
  }
  else
  {
    static_data::resume_unselected.draw(IntPoint(130, 90) + rel);
    static_data::quit_selected.draw(IntPoint(120, 200) + rel);
  }
}

