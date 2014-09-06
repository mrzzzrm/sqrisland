#include "./GameOver.hpp"

using namespace std;
using namespace util;
using namespace error;
using namespace geo;
using namespace gfx;
using namespace input;


namespace scene
{
  Animation GameOver::static_data::background;

  void GameOver::loadStaticData()
  {
    try
    {
      static_data::background.load("dat/scenes/gameover.ani");
    }
    catch(Exception &exception)
    {
      throw Exception() << "Error loading static data for scene::GameOver: " << exception;
    }
  }

  void GameOver::clearStaticData()
  {
    static_data::background.clear();
  }

  GameOver::GameOver()
  {
    fading.setTicks(500);
    fading.setSize(IntSize(-1, -1));
    fading.setColor(BLACK);
    fading.forceStart(FADE_OUT);

    timer.update();
    start = timer.getTicks();
  }

  GameOver::~GameOver()
  {

  }

  bool GameOver::isOver()
  {
    return getState() == SCENE_STATE_OUT && fading.isDone();
  }

  void GameOver::update()
  {
    timer.update();

    switch(getState())
    {
      case SCENE_STATE_IN:
        fading.update();
        if(fading.isDone())
          GameOver::setState(SCENE_STATE_MAIN);
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

  void GameOver::draw()
  {
    static_data::background.draw(IntPoint(0, 0));

    if(getState() != SCENE_STATE_MAIN)
      fading.draw();
  }

}
