#include "./GameFinished.hpp"

using namespace std;
using namespace util;
using namespace error;
using namespace geo;
using namespace gfx;
using namespace input;


namespace scene
{
  Animation GameFinished::static_data::background;

  void GameFinished::loadStaticData()
  {
    try
    {
      static_data::background.load("dat/scenes/gamefinished.ani");
    }
    catch(Exception &exception)
    {
      throw Exception() << "Error loading static data for scene::GameFinished: " << exception;
    }
  }

  void GameFinished::clearStaticData()
  {
    static_data::background.clear();
  }

  GameFinished::GameFinished()
  {
    fading.setTicks(500);
    fading.setSize(IntSize(-1, -1));
    fading.setColor(BLACK);
    fading.forceStart(FADE_OUT);

    timer.update();
    start = timer.getTicks();
  }

  GameFinished::~GameFinished()
  {

  }

  bool GameFinished::isOver()
  {
    return getState() == SCENE_STATE_OUT && fading.isDone();
  }

  void GameFinished::update()
  {
    timer.update();

    switch(getState())
    {
      case SCENE_STATE_IN:
        fading.update();
        if(fading.isDone())
          GameFinished::setState(SCENE_STATE_MAIN);
      break;

      case SCENE_STATE_MAIN:
        if(timer.getTicks() - start > 2000 || Input::isButtonPressed(BUTTON_B))
        {
          setState(SCENE_STATE_OUT);
          fading.forceStart(FADE_IN);
        }
      break;

      case SCENE_STATE_OUT:
        fading.update();
      break;

    }
  }

  void GameFinished::draw()
  {
    static_data::background.draw(IntPoint(0, 0));

    if(getState() != SCENE_STATE_MAIN)
      fading.draw();
  }

}
