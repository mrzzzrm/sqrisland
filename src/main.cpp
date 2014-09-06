#include "framework.hpp"
#include "./Level.hpp"
#include "./Settings.hpp"
#include "./Savestate.hpp"
#include "./Mainmenu.hpp"
#include "./Intro.hpp"
#include "./scenes/scenes.hpp"

using namespace std;
using namespace etc;
using namespace util;
using namespace audio;
using namespace geo;
using namespace gfx;
using namespace input;
using namespace error;
using namespace font;

enum GAME_STATE
{
  GAME_STATE_MAINMENU,
  GAME_STATE_LEVEL,
  GAME_STATE_SCENE
};


void load_static_data()
{
  Player::loadStaticData();

  bullet::Player::loadStaticData();

  enemy::Egg::loadStaticData();
  enemy::Kite::loadStaticData();
  enemy::Punk::loadStaticData();
  enemy::Tri::loadStaticData();

  corpse::Egg::loadStaticData();
  corpse::Kite::loadStaticData();
  corpse::Punk::loadStaticData();
  corpse::Tri::loadStaticData();

  item::Coin::loadStaticData();
  item::Upgrader::loadStaticData();
  item::Life::loadStaticData();

  Level::loadStaticData();
  LevelIntro::loadStaticData();
  LevelInterface::loadStaticData();
  LevelMenu::loadStaticData();
  Mainmenu::loadStaticData();
  Textbox::loadStaticData();

  scene::GameOver::loadStaticData();
  scene::GameFinished::loadStaticData();
}

void clear_static_data()
{
  Player::clearStaticData();

  bullet::Player::clearStaticData();

  enemy::Egg::clearStaticData();
  enemy::Kite::clearStaticData();
  enemy::Punk::clearStaticData();
  enemy::Tri::clearStaticData();

  corpse::Egg::clearStaticData();
  corpse::Kite::clearStaticData();
  corpse::Punk::clearStaticData();
  corpse::Tri::clearStaticData();

  item::Coin::clearStaticData();
  item::Upgrader::clearStaticData();
  item::Life::clearStaticData();

  Level::loadStaticData();
  LevelIntro::clearStaticData();
  LevelInterface::clearStaticData();
  LevelMenu::clearStaticData();
  Mainmenu::clearStaticData();
  Textbox::clearStaticData();

  scene::GameOver::loadStaticData();
  scene::GameFinished::loadStaticData();
}

void init_playlist(Playlist &playlist)
{
  playlist.addTrack(Path("dat/audio/music/disco-very.ogg"));
  playlist.addTrack(Path("dat/audio/music/nivbible.ogg"));
  playlist.addTrack(Path("dat/audio/music/opera-pro.ogg"));
  playlist.addTrack(Path("dat/audio/music/proof.ogg"));
  playlist.addTrack(Path("dat/audio/music/t7es_msiegler.ogg"));
  playlist.addTrack(Path("dat/audio/music/tornado.ogg"));
}

void mainloop(const Settings &settings)
{
  TtfFont font("dat/fonts/ALEAWBB_.TTF",30);
  TtfLabel label(&font);
  FpsCounter fps;

  srand(SDL_GetTicks());

  Playlist playlist;
  GAME_STATE gamestate = GAME_STATE_MAINMENU;
  GAME_STATE postscene_gamestate = GAME_STATE_MAINMENU;
  Mainmenu *mainmenu = NULL;
  Level *level = NULL;
  Scene *scene = NULL;

  init_playlist(playlist);
  playlist.shuffle();
  playlist.play();
  SoundPlayer::setVolume(1);
  MusicPlayer::setVolume(0.5);

  while(true)
  {
    Input::update();
    playlist.update();

    switch(gamestate)
    {
      case GAME_STATE_MAINMENU:
      {
        if(mainmenu == NULL)
        {
          mainmenu = new Mainmenu();
          mainmenu->setLoadAvailable(Savestate::exists());
        }

        mainmenu->update();
        mainmenu->draw();

        if(mainmenu->isSelectionDone())
        {
          switch(mainmenu->getSelection())
          {
            case MAINMENU_NEW:
              level = new Level();
              level->startFromBeginning(settings);
              gamestate = GAME_STATE_LEVEL;
            break;

            case MAINMENU_LOAD:
              level = new Level();
              level->startFromSavestate(settings);
              gamestate = GAME_STATE_LEVEL;
            break;

            case MAINMENU_QUIT:
              return;
          }

          delete mainmenu;
          mainmenu = NULL;

          level->start();
        }
      }
      break;

      case GAME_STATE_LEVEL:
      {
        if(level == NULL)
        {
          level = new Level();
          level->startFromSavestate(settings);
          level->start();
        }

        level->update();
        level->draw();

        if(level->isOver())
        {
          switch(level->getState())
          {
            case LEVEL_STATE_NEXT_LEVEL:
              if(!Level::nextLevelExists())
              {
                Savestate::remove();
                gamestate = GAME_STATE_SCENE;
                scene = new scene::GameFinished();
                postscene_gamestate = GAME_STATE_MAINMENU;
              }
            break;

            case LEVEL_STATE_QUITTING:
              gamestate = GAME_STATE_MAINMENU;
            break;

            case LEVEL_STATE_GAMEOVER:
              gamestate = GAME_STATE_SCENE;
              scene = new scene::GameOver();
              postscene_gamestate = GAME_STATE_MAINMENU;
            break;

            default:break;
          }

          delete level;
          level = NULL;
        }
      }
      break;

      case GAME_STATE_SCENE:
      {
        scene->update();
        scene->draw();

        if(scene->isOver())
        {
          delete scene;
          scene = NULL;

          gamestate = postscene_gamestate;
        }
      }
      break;
    }
//
//    label.setText(float_to_string(fps.getFps()));
//    label.draw(IntPoint(400, 0), Alignment(0.5, 0));

    gfx::updateScreen(false);
    fps.frameFinished();
  }

  playlist.clear();
}


void run()
{
  Intro intro;
  Settings settings;


  settings.load();
  intro.load();
  intro.in();
  load_static_data();
  intro.out();
  mainloop(settings);
  clear_static_data();
}

void terminate()
{
  clear_static_data();
  env2d::close();
}

int main(int argc, char **argv)
{
  try
  {
    #ifdef PLATFORM_PANDORA
    env2d::open(800, 480, 16, true);
    #else
    env2d::open(800, 480, 16, false);
    #endif
    gfx::setTargetFps(58);
    atexit(::terminate);
    run();
  }
  catch(const Exception &exception)
  {
    cerr << exception << endl;
    return 1;
  }
  catch(...)
  {
    cerr << "Unknown exception" << endl;
    return 1;
  }


  return 0;
}
