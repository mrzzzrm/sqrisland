#include "../Level.hpp"

using namespace std;
using namespace gfx;
using namespace etc;
using namespace geo;
using namespace util;
using namespace input;
using namespace collision;
using namespace error;


void Level::MoveObject(Object &object)
{
  switch(object.getType())
  {
    case OBJECT_PLAYER: // Some player-specific code recognizing topcollision (e.g. used for boxes)
    {
      int posy, bufy;

      posy = object.getPosition().getY();
      bufy = (int)object.getMovementBuffer().getY();

      player.setTopcoll(false);

      MovePlayer();

      if(posy + bufy != player.getPosition().getY())
      {
        if(bufy < 0)
          player.setTopcoll(true);
      }
    }
    break;

    case OBJECT_ENEMY:  MoveEnemy(dynamic_cast<Enemy&>(object));    break;
    case OBJECT_BULLET: MoveBullet(dynamic_cast<Bullet&>(object));  break;
    case OBJECT_ITEM:   MoveItem(dynamic_cast<Item&>(object));      break;
    case OBJECT_CORPSE: MoveCorpse(dynamic_cast<Corpse&>(object));  break;

    default:
    break;
  }
}


void Level::MovePlayer()
{
  bool ir1, ir2 = false; // Interrupts for nested loops

  for(int a = 0; a < 2; a++)
  {
    ir1 = false;
    AXIS axis = (AXIS)a;


    while(player.getMovementBuffer().hasIntValue(axis))
    {
      s8 sgn;
      list<Object*> enemies, items, eventareas;
      list<SpecialTile*> special_tiles;

      sgn = player.getMovementBuffer().pop(axis);
      player.move(axis, sgn);

      if(state == LEVEL_STATE_DYING || state == LEVEL_STATE_GAMEOVER)
          continue;

      special_tiles = this->special_tiles.getCollisionWithPlayer(player);
      PlayerSpecialTilesInteraction(special_tiles, axis, sgn);

      if(PlayerIllegalOverlap())
      {
        player.move(axis, -sgn);
        break;
      }

      enemies = ActiveObjects(OBJECT_ENEMY);
      for(list<Object*>::iterator i = enemies.begin(); i != enemies.end(); i++)
      {
        if(PlayerEnemyCollision(*dynamic_cast<Enemy*>(*i)) & OBJECT_PLAYER)
        {
          player.move(axis, -sgn);
          ir1 = true;
          break;
        }
      }

      items = ActiveObjects(OBJECT_ITEM);
      for(list<Object*>::iterator i = items.begin(); i != items.end(); i++)
      {
        if(PlayerItemCollision(*dynamic_cast<Item*>(*i)) & OBJECT_PLAYER)
        {
          player.move(axis, -sgn);
          ir1 = true;
          break;
        }
      }

      eventareas = ActiveObjects(OBJECT_EVENTAREA);
      for(list<Object*>::iterator i = eventareas.begin(); i != eventareas.end(); i++)
      {
        if(state != LEVEL_STATE_INTRO)
        {
          if(PlayerEventAreaCollision(*dynamic_cast<EventArea*>(*i), false) & OBJECT_PLAYER)
          {
            player.move(axis, -sgn);
            ir1 = true;
            break;
          }
        }
      }

      if(PlayerBottomBorderCollision())
      {
        player.move(axis, -sgn);
        ir2 = true;
        break;
      }

      if(ir1)
        break;
    }

    player.getMovementBuffer().clearIntValue(axis);

    if(ir2)
      break;
  }
}


void Level::MovePlatform(Platform &platform)
{

  for(int a = 0; a < 2; a++)
  {
    AXIS axis = (AXIS)a;

    while(platform.getMovementBuffer().hasIntValue(axis))
    {
      s8 sgn;
      list<Object*> objects;
      list<pair<Object*, bool> > travelers;


      sgn = platform.getMovementBuffer().pop(axis);

      /**
        Get travelers
      **/
      objects = ActiveObjects(OBJECT_CLASS_PLATFORM_TRAVELER);
      for(list<Object*>::iterator i = objects.begin(); i != objects.end(); i++)
      {
        Object *object = *i;

        object->move(AXIS_Y, 1);
        if(object->isCollision(platform))
          travelers.push_back(pair<Object*, bool>(object, true));
        object->move(AXIS_Y, -1);
      }

      if(travelers.size() > 0)
      {
        /**
          Move platform and travelers
        **/
        platform.move(axis, sgn);
        for(list<pair<Object*, bool> >::iterator i = travelers.begin(); i != travelers.end(); i++)
        {
          Object *object = i->first;
          object->move(axis, sgn);
        }

        /**
          If platform is stopped move all back
        **/
        if(PlatformIllegalOverlap(platform))
        {
          platform.move(axis, -sgn);
          for(list<pair<Object*, bool> >::iterator i = travelers.begin(); i != travelers.end(); i++)
          {
            Object *object = i->first;
            object->move(axis, -sgn);
          }

          break;
        }
        else
        {
          bool stopped_traveler;

          /**
            Check travelers for stopping collision
          **/
          stopped_traveler = false;
          for(list<pair<Object*, bool> >::iterator i = travelers.begin(); i != travelers.end();)
          {
            Object *object = i->first;
            if(i->second && ObjectIllegalOverlap(*object))
            {
              object->move(axis, -sgn);
              i->second = false;
              i = travelers.begin();
              stopped_traveler = true;
            }
            else
            {
              i++;
            }
          }

          /**
            If platform is stopped by stopped travelermove all back
          **/
          if(stopped_traveler)
          {
            if(PlatformIllegalOverlap(platform))
            {
              platform.move(axis, -sgn);
              for(list<pair<Object*, bool> >::iterator i = travelers.begin(); i != travelers.end(); i++)
              {
                Object *object = i->first;

                if(i->second)
                  object->move(axis, -sgn);
              }

              break;
            }
          }


          /**
            Move all back to perform effect - movement;
          **/
          platform.move(axis, -sgn);
          for(list<pair<Object*, bool> >::iterator i = travelers.begin(); i != travelers.end(); i++)
          {
            Object *object = i->first;

            if(i->second)
              object->move(axis, -sgn);
          }

          /**
            Put moving travelers and platforms into blacklist
            to prevent interaction when moving after each other
          **/
          blacklist[(Object*)&platform] = BLACKLIST_DISABLED;
          for(list<pair<Object*, bool> >::iterator i = travelers.begin(); i != travelers.end(); i++)
          {
            Object *object = i->first;

            if(i->second)
              blacklist[object] = BLACKLIST_DISABLED;
          }

          /**
            Move all with effects
          **/
          platform.move(axis, sgn);
          for(list<pair<Object*, bool> >::iterator i = travelers.begin(); i != travelers.end(); i++)
          {
            if(i->second)
            {
              FloatVector buffer = i->first->getMovementBuffer();

              i->first->getMovementBuffer().clear();

              if(axis == AXIS_X)
                i->first->getMovementBuffer().pushVector(IntVector(sgn, 0));
              else
                i->first->getMovementBuffer().pushVector(IntVector(0, sgn));

              MoveObject(*(i->first));

              i->first->getMovementBuffer().load(buffer);
            }
          }

          /**
            Remove moving travelers and platforms from blacklist
          **/
          blacklist[&platform] = BLACKLIST_NOT;
          for(list<pair<Object*, bool> >::iterator i = travelers.begin(); i != travelers.end(); i++)
          {
            Object *object = i->first;

            if(i->second)
              blacklist[object] = BLACKLIST_NOT;
          }
        }


      }
      else
      {
        platform.move(axis, sgn);
        if(PlatformIllegalOverlap(platform))
        {
          platform.move(axis, -sgn);
          break;
        }
      }

    }

    platform.getMovementBuffer().clearIntValue(axis);
  }
}


void Level::MoveEnemy(Enemy &enemy)
{
  bool ir = false;

  for(int a = 0; a < 2; a++)
  {
    AXIS axis = (AXIS)a;

    while(enemy.getMovementBuffer().hasIntValue(axis))
    {
      s8 sgn;

      sgn = enemy.getMovementBuffer().pop(axis);
      enemy.move(axis, sgn);

      if(PlayerEnemyCollision(enemy) & OBJECT_ENEMY)
      {
        enemy.move(axis, -sgn);
        ir = true;
        break;
      }
      if(EnemyIllegalOverlap(enemy))
      {
        enemy.move(axis, -sgn);
        break;
      }

      if(ir)
        break;
    }

    enemy.getMovementBuffer().clearIntValue(axis);

    if(ir)
      break;
  }
}


void Level::MoveBullet(Bullet &bullet)
{
  bool ir = false; // Interrupt for nested loop

  for(int a = 0; a < 2; a++)
  {
    AXIS axis = (AXIS)a;

    while(bullet.getMovementBuffer().hasIntValue(axis))
    {
      s8 sgn;
      list<Object*> enemies;

      sgn = bullet.getMovementBuffer().pop(axis);
      bullet.move(axis, sgn);

      if(BulletIllegalOverlap(bullet))
      {
        bullet.move(axis, -sgn);
        if(bullet.getType() == BULLET_PLAYER && axis == AXIS_Y) // Playerbullets bounce...
        {
          bullet::Player *pbullet = dynamic_cast<bullet::Player*>(&bullet);

          if(pbullet->getLevel() < 2) // ...if they are level 2, so thez aren't deleted
          {
            RemoveBullet(bullet);
            ir = true;
            break;
          }
        }
        else // Non-Playerbullets(no such currently) or Playerbullets that got stopped on the X axis are removed.
        {
          RemoveBullet(bullet);
          ir = true;
          break;
        }
      }

      enemies = ActiveObjects(OBJECT_ENEMY);
      for(list<Object*>::iterator i = enemies.begin(); i != enemies.end(); i++)
      {
        if(EnemyBulletCollision(*dynamic_cast<Enemy*>(*i), bullet) & OBJECT_BULLET)
        {
          bullet.move(axis, -sgn);
          RemoveBullet(bullet);
          ir = true;
          break;
        }
      }
    }

    bullet.getMovementBuffer().clearIntValue(axis);

    if(ir)
      break;
  }
}


void Level::MoveItem(Item &item)
{
  bool ir = false; // Interrupt for nested loop

  for(int a = 0; a < 2; a++)
  {
    AXIS axis = (AXIS)a;

    while(item.getMovementBuffer().hasIntValue(axis))
    {
      s8 sgn;

      sgn = item.getMovementBuffer().pop(axis);
      item.move(axis, sgn);

      if(ItemIllegalOverlap(item))
      {
        item.move(axis, -sgn);
        break;
      }

      if(PlayerItemCollision(item))
      {
        item.move(axis, -sgn);
        RemoveItem(item);
        ir = true;
        break;
      }
    }

    item.getMovementBuffer().clearIntValue(axis);

    if(ir)
      break;
  }
}

void Level::MoveCorpse(Corpse &corpse)
{
  for(int a = 0; a < 2; a++)
  {
    AXIS axis = (AXIS)a;

    while(corpse.getMovementBuffer().hasIntValue(axis))
    {
      s8 sgn;

      sgn = corpse.getMovementBuffer().pop(axis);
      corpse.move(axis, sgn);

      if(CorpseIllegalOverlap(corpse))
      {
        corpse.move(axis, -sgn);
        break;
      }
    }

    corpse.getMovementBuffer().clearIntValue(axis);
  }
}

