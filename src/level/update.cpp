#include "../Level.hpp"

using namespace std;
using namespace gfx;
using namespace etc;
using namespace geo;
using namespace util;
using namespace input;
using namespace collision;
using namespace error;



void Level::UpdateStateIntro()
{
  if(!intro.isActive())
  {
    state = LEVEL_STATE_MAIN;

    list<Object*> eventareas;
    eventareas = ActiveObjects(OBJECT_EVENTAREA);
    for(list<Object*>::iterator i = eventareas.begin(); i != eventareas.end(); i++)
    {
      PlayerEventAreaCollision(*dynamic_cast<EventArea*>(*i), false);
    }
  }

  intro.update();
  UpdateMainTasks();
}

void Level::UpdateStateMain()
{
  UpdateMainTasks();

  if(objective_reached)
  {
    fading.forceStart(FADE_IN);
    state = LEVEL_STATE_NEXT_LEVEL;
    Save();
  }
}

void Level::UpdateStateMenu()
{
  menu.update();

  if(menu.isSelectionDone())
  {
    if(menu.getSelection() == LEVELMENU_RESUME && menu.getState() == LEVELMENU_STATE_INACTIVE)
    {
      state = LEVEL_STATE_MAIN;
    }
    if(menu.getSelection() == LEVELMENU_QUIT)
    {
      fading.forceStart(FADE_IN);
      state = LEVEL_STATE_QUITTING;
    }
  }
}

void Level::UpdateStateDying()
{
  fading.update();

  UpdateTouchDetection();
  UpdateGravity();
  UpdateObjects();
  UpdateActivity();

  if(fading.getType() == FADE_IN && fading.isDone()) // Screen is completly black
  {
    player.setPosition(reappearposition);
    player.sigRespawn();

    camera.warp(reappearposition);
    fading.forceStart(FADE_OUT);
    interface.sigKilled();

    state = LEVEL_STATE_RESPAWNING;
  }
}

void Level::UpdateStateRespawning()
{
  fading.update();

  UpdateTouchDetection();
  UpdateGravity();
  UpdateObjects();
  UpdateActivity();
  UpdateCamera();

  if(fading.getType() == FADE_OUT && fading.isDone())
  {
    state = LEVEL_STATE_MAIN;
    input_focus = INPUT_FOCUS_PLAYER;
  }
}

void Level::UpdateStateNextLevel()
{
  fading.update();
  UpdateMainTasks();
}

void Level::UpdateStateQuitting()
{
  fading.update();
}

void Level::UpdateStateGameOver()
{
  fading.update();

  UpdateTouchDetection();
  UpdateGravity();
  UpdateObjects();
  UpdateActivity();
}

void Level::UpdateMainTasks()
{
  textbox.update();

  UpdateMainPlayerInput();
  UpdateTouchDetection();
  UpdateGravity();
  UpdateObjects();
  UpdateCamera();
  UpdateSpecialTiles();
  UpdateSingleRunEffects();
  UpdateActivity();

  BlacklistCleanup();
  InputFocus();
  EventCheck();
}

void Level::UpdateTouchDetection()
{
  list<Object*> objects;

  objects = ActiveObjects(OBJECT_CLASS_TOUCH_DETECTION);
  for(list<Object*>::iterator i = objects.begin(); i != objects.end(); i++)
  {
    Object *object = *i;
    TouchDetection(*object);
  }
}

void Level::UpdateGravity()
{
  list<Object*> objects;

  objects = ActiveObjects(OBJECT_ALL);
  for(list<Object*>::iterator i = objects.begin(); i != objects.end(); i++)
  {
    Object *object = *i;
    if(object->getGravity().isActive())
    {
      object->getGravity().accelerate(settings.getGravityAcc());
      object->getGravity().limit(settings.getGravityMax());
    }
  }
}

void Level::UpdateObjects()
{
  list<Object*> objects;

  /* Objects updates */
  objects = ActiveObjects(OBJECT_ALL);
  for(list<Object*>::iterator i = objects.begin(); i != objects.end(); i++)
  {
    Object *object = *i;
    object->update();
  }

  /* Move objects except platforms */
  objects = ActiveObjects(OBJECT_ALL ^ OBJECT_PLATFORM);
  for(list<Object*>::iterator i = objects.begin(); i != objects.end(); i++)
  {
    if(blacklist[*i] == BLACKLIST_NOT)
      MoveObject(*(*i));
  }

  /* Move platforms */
  objects = ActiveObjects(OBJECT_PLATFORM);
  for(list<Object*>::iterator i = objects.begin(); i != objects.end(); i++)
  {
    if(blacklist[*i] == BLACKLIST_NOT)
      MovePlatform(*dynamic_cast<Platform*>(*i));
  }
}

void Level::UpdateCamera()
{
  camera.move(player.getPosition());
}

void Level::UpdateSpecialTiles()
{
  special_tiles.update();
}

void Level::UpdateSingleRunEffects()
{
  for(int l = 0; l < (int)active.effectlayers.size(); l++)
  {
    for(list<Effect*>::iterator i = active.effectlayers[l].begin(); i != active.effectlayers[l].end(); i++)
    {
      Effect *effect = *i;

      if(effect->getType() == EFFECT_SINGLE_RUN)
      {
        SingleRunEffect *sreffect = dynamic_cast<SingleRunEffect*>(effect);

        if(sreffect->hasRan())
          blacklist[effect] = BLACKLIST_REMOVE;
      }
    }
  }
}

void Level::UpdateActivity()
{
  if(Timer::getTicks() - last_activity_check >= settings.getActivityCheckInterval())
  {
    CheckActivity();
    RemoveInactiveBullets();
    RemoveInactiveCorpses();
    RemoveInactiveLifeItems();
    last_activity_check = Timer::getTicks();
  }
}

void Level::UpdateMainPlayerInput()
{
  switch(input_focus)
  {
    case INPUT_FOCUS_PLAYER:
    {
      if(Input::isButtonHold(BUTTON_LEFT)) player.sigMove(-1);
      if(Input::isButtonHold(BUTTON_RIGHT)) player.sigMove(1);
      if(Input::isButtonReleased(BUTTON_LEFT) && (player.getState() & PLAYER_LEFT)) player.sigStopMoving();
      if(Input::isButtonReleased(BUTTON_RIGHT) && (player.getState() & PLAYER_RIGHT)) player.sigStopMoving();

      if(Input::isButtonPressed(BUTTON_B)) player.sigJump(JUMP_BY_KEY, settings.getPlayerJumpDur());
      if(Input::isButtonReleased(BUTTON_B)) player.sigBreakJump();

      if(Input::isButtonPressed(BUTTON_UP))
      {
        list<Object*> objects = ActiveObjects(OBJECT_EVENTAREA);
        for(list<Object*>::iterator i = objects.begin(); i != objects.end(); i++)
        {
          PlayerEventAreaCollision(*dynamic_cast<EventArea*>(*i), true);
        }
      }

      if(Input::isButtonPressed(BUTTON_X) && player.getLevel() >= 1 && active.bullets.size() <= 2)
      {
        bullet::Player *tb = new bullet::Player(*this, player.getLevel());

        if(player.getState() & PLAYER_RIGHT)
        {
          tb->setPosition(player.getPosition() + IntVector(25, 0));
          tb->setMovement(FloatVector(550, -200));
        }
        else
        {
          tb->setPosition(player.getPosition() + IntVector(-25, 0));
          tb->setMovement(FloatVector(-550, -200));
        }
        active.bullets.push_back(tb);
      }

      if(Input::isButtonPressed(BUTTON_START) || Input::isKeyPressed(KEY_ESCAPE))
      {
        state = LEVEL_STATE_MENU;
        menu.activate();
      }
    }
    break;

    case INPUT_FOCUS_TEXTBOX:

      if(Input::isButtonPressed(BUTTON_B)) textbox.sigContinue();
      if(Input::isButtonPressed(BUTTON_START)) textbox.sigAbort();
        textbox.sigSetFast(Input::isButtonHold(BUTTON_B));
    break;

    default: break;
  }
}

