#include "../Level.hpp"

using namespace std;
using namespace gfx;
using namespace etc;
using namespace geo;
using namespace util;
using namespace input;
using namespace collision;
using namespace error;


bool Level::ObjectCollisionWithLevel(const Object &object)
{
  return RasterMap::isCollision(object, IntPoint(0, 0), object.getPosition(), IntRect(0, 0, -1, -1), IntRect(0, 0, -1, -1), ALIGN_BY_CORNER, object.getAlignment()) ||
         special_tiles.isCollision(object);
}

bool Level::ObjectCollisionWithPlayer(const Object &object)
{
  return player.isCollision(object);
}

Platform *Level::ObjectCollisionWithPlatform(const Object &object)
{
  for(list<Platform*>::iterator i = active.platforms.begin(); i != active.platforms.end(); i++)
  {
    Platform *platform = *i;

    if((Object*)platform == &object)
      continue;

    if(platform->isCollision(object))
      return platform;
  }

  return NULL;
}

Enemy *Level::ObjectCollisionWithEnemy(const Object &object)
{
  for(list<Enemy*>::iterator i = active.enemies.begin(); i != active.enemies.end(); i++)
  {
    Enemy *enemy = *i;

    if((Object*)enemy == &object)
      continue;

    if(enemy->isCollision(object))
      return enemy;
  }

  return NULL;
}

Bullet *Level::ObjectCollisionWithBullet(const Object &object)
{
  for(list<Bullet*>::iterator i = active.bullets.begin(); i != active.bullets.end(); i++)
  {
    Bullet *bullet = *i;

    if((Object*)bullet == &object)
      continue;

    if(bullet->isCollision(object))
      return bullet;
  }

  return NULL;
}

Item *Level::ObjectCollisionWithItem(const Object &object)
{
  for(list<Item*>::iterator i = active.items.begin(); i != active.items.end(); i++)
  {
    Item *item = *i;

    if((Object*)item == &object)
      continue;

    if(item->isCollision(object))
      return item;
  }

  return NULL;
}

bool Level::ObjectIllegalOverlap(const Object &object)
{
  switch(object.getType())
  {
    case OBJECT_PLAYER:   return PlayerIllegalOverlap();                                        break;
    case OBJECT_PLATFORM: return PlatformIllegalOverlap(dynamic_cast<const Platform&>(object)); break;
    case OBJECT_ENEMY:    return EnemyIllegalOverlap(dynamic_cast<const Enemy&>(object));       break;
    case OBJECT_BULLET:   return BulletIllegalOverlap(dynamic_cast<const Bullet&>(object));     break;
    case OBJECT_ITEM:     return ItemIllegalOverlap(dynamic_cast<const Item&>(object));         break;

    default: return false;
  }
}

bool Level::PlayerIllegalOverlap()
{
  RectMap side;


  if(state == LEVEL_STATE_DYING || state == LEVEL_STATE_GAMEOVER)
    return false;

  if(ObjectCollisionWithLevel(player)) return true;
  if(ObjectCollisionWithPlatform(player)) return true;

  /* Stop on left/right level sides */
  side.setSize(IntSize(1, 100));
  side.addRect(IntRect(0, 0, 1, 100));

  if(player.RectMap::isCollision(side, player.getPosition(), IntPoint(-1, player.getPosition().getY() - 50), IntRect(0, 0, -1, -1), IntRect(0, 0, -1, -1), player.getAlignment(), ALIGN_BY_CORNER))
    return true;
  if(player.RectMap::isCollision(side, player.getPosition(), IntPoint(Map::getWidth() * Map::getTileWidth(), player.getPosition().getY()), IntRect(0, 0, -1, -1), IntRect(0, 0, -1, -1), player.getAlignment(), ALIGN_BY_CORNER))
    return true;

  return false;
}

bool Level::PlatformIllegalOverlap(const Platform &platform)
{
  if(ObjectCollisionWithLevel(platform)) return true;
  if(ObjectCollisionWithPlayer(platform)) return true;
  if(ObjectCollisionWithPlatform(platform)) return true;
  if(ObjectCollisionWithEnemy(platform)) return true;

  return false;
}

bool Level::EnemyIllegalOverlap(const Enemy &enemy)
{
  if(ObjectCollisionWithLevel(enemy)) return true;
  if(ObjectCollisionWithPlatform(enemy)) return true;
  if(ObjectCollisionWithPlayer(enemy)) return true;

  return false;
}

bool Level::BulletIllegalOverlap(const Bullet &bullet)
{
  if(ObjectCollisionWithLevel(bullet)) return true;
  if(ObjectCollisionWithPlatform(bullet)) return true;

  return false;
}

bool Level::ItemIllegalOverlap(const Item &item)
{
  if(item.getType() == ITEM_LIFE) // The life-item has to have an collmap to detect a collision with the player
    return false;

  if(ObjectCollisionWithLevel(item)) return true;
  if(ObjectCollisionWithPlatform(item)) return true;

  return false;
}

bool Level::CorpseIllegalOverlap(const Corpse &corpse)
{
  if(ObjectCollisionWithLevel(corpse)) return true;
  if(ObjectCollisionWithPlatform(corpse)) return true;

  return false;
}

OBJECT_TYPE Level::PlayerEnemyCollision(Enemy &enemy)
{
  ENEMY_PLAYER_COLLISION_TYPE coll_type;
  u32 res;

  if(
     state == LEVEL_STATE_NEXT_LEVEL ||
     state == LEVEL_STATE_GAMEOVER ||
     state == LEVEL_STATE_QUITTING ||
     state == LEVEL_STATE_RESPAWNING ||
     state == LEVEL_STATE_DYING
    )
      return (OBJECT_TYPE)0;


  res = 0;
  coll_type = enemy.getCollisionWithPlayer(player);


  if(coll_type != ENEMY_PLAYER_COLLISION_NONE)
  {
    if(coll_type == ENEMY_PLAYER_COLLISION_PLAYER_DAMAGED)
    {
      if(!player.isFlickering())
        PlayerDamaged();

      res |= OBJECT_ENEMY;
      res |= OBJECT_PLAYER;
    }
    else // Implies ENEMY_PLAYER_COLLISION_ENEMY_DAMAGED
    {
      if(enemy.getType() == ENEMY_EGG)
      {
        res |= OBJECT_PLAYER;
        res |= OBJECT_ENEMY;
      }
      else
      {
        RemoveEnemy(enemy, ENEMY_DEATH_BY_JUMP);
        res |= OBJECT_ENEMY;
      }

      player.sigJump(JUMP_BY_SPRING, settings.getPlayerOnEnemyJumpDur());
    }
  }

  return (OBJECT_TYPE)res;
}

bool Level::PlayerBottomBorderCollision()
{
  RectMap rmap;
  rmap.setSize(IntSize(100, 1));
  rmap.addRect(IntRect(0, 0, 100, 1));

  if(player.RectMap::isCollision
      (
       rmap,
       player.getPosition(), IntPoint(player.getPosition().getX() - 50, Map::getHeight() * Map::getTileHeight() + 1),
       IntRect(0, 0, -1, -1), IntRect(0, 0, -1, -1),
       player.getAlignment(), ALIGN_BY_CORNER
      ))
      {
        PlayerKilled();
        return true;
      }
      else
      {
        return false;
      }
}

OBJECT_TYPE Level::EnemyBulletCollision(Enemy &enemy, Bullet &bullet)
{
  if(!enemy.isCollision(bullet))
    return (OBJECT_TYPE)0;

  RemoveBullet(bullet);

  if(enemy.getType() == ENEMY_EGG)
  {
    return OBJECT_BULLET;
  }
  else
  {
    RemoveEnemy(enemy, ENEMY_DEATH_BY_BULLET);
    return (OBJECT_TYPE)(OBJECT_ENEMY | OBJECT_BULLET);
  }
}

OBJECT_TYPE Level::PlayerItemCollision(Item &item)
{
  if(!player.isCollision(item))
    return (OBJECT_TYPE)0;

  PlayerCollectItem(item.getType());
  RemoveItem(item);

  return OBJECT_ITEM;
}

OBJECT_TYPE Level::PlayerEventAreaCollision(EventArea &eventarea, bool manual)
{
  const Event *event;


  if(!((RectMap)player).isCollision(eventarea.getRect(), player.getPosition(), IntRect(0, 0, -1, -1), ALIGN_BY_CENTER))
  {
    eventarea.setEntered(false);
    return (OBJECT_TYPE)0;
  }
  if(eventarea.isEntered() && !manual)
    return (OBJECT_TYPE)0;
  if(event_pending)
    return (OBJECT_TYPE)0;

  eventarea.setEntered(true);


  event = &eventarea.getEvent();
  if(event->getStartType() == EVENT_START_MANUAL && !manual)
    return (OBJECT_TYPE)0;

  switch(event->getType())
  {
    case EVENT_TEXTBOX:
    {
      const event::Textbox *textboxevent = dynamic_cast<const event::Textbox*>(event);
      textbox.setText(textboxevent->getText());
      textbox.start();
      event_pending = true;
    }
    break;

    case EVENT_CHECKPOINT:
    {
      const event::Checkpoint *checkpointevent = dynamic_cast<const event::Checkpoint*>(event);
      reappearposition = IntPoint(eventarea.getRect().getX(), eventarea.getRect().getY()) + point_to_vector(checkpointevent->getRelativeReappearPosition());
    }
    break;

    case EVENT_OBJECTIVE_REACHED:
    {
      event_pending = true;
      objective_reached = true;
    }
    break;
  }

  if(event->getPlayType() == EVENT_PLAY_ONCE)
    blacklist[&eventarea] = BLACKLIST_REMOVE;


  return (OBJECT_TYPE)0;
}
