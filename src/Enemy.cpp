#include "./Enemy.hpp"
#include "./Player.hpp"

using namespace std;
using namespace geo;
using namespace util;
using namespace error;


Enemy::Enemy(const util::Timer &timer, ENEMY_TYPE type, const tmx::base::Object &object):
  Unit(timer, OBJECT_ENEMY, object)
{
  this->type = type;

  hp = 0;
}


Enemy::~Enemy()
{

}


ENEMY_TYPE Enemy::getType() const
{
  return type;
}


int Enemy::getHP() const
{
  return hp;
}


void Enemy::setHP(int hp)
{
  this->hp = hp;
}


void Enemy::decHP(int hp)
{
  this->hp -= hp;
}

void Enemy::setRectWoundable(int i)
{
  woundable_rects.push_back(i);
  woundable_rects.sort();
  woundable_rects.unique();
}


ENEMY_PLAYER_COLLISION_TYPE Enemy::getCollisionWithPlayer(const Player &player) const
{
  ENEMY_PLAYER_COLLISION_TYPE res;
  list<int>::const_iterator woundable_rects_iterator;
  IntPoint enemy_corner;
  IntPoint player_corner;


  res = ENEMY_PLAYER_COLLISION_NONE;
  enemy_corner = Object::getPosition() - Object::getAlignment() * RectMap::getSize();
  player_corner = player.getPosition() - player.getAlignment() * player.getSize();

  for(int pr = 0; pr < player.getRectCount(); pr++)
  {
    IntRect player_rect(player.getRect(pr));

    player_rect.incX(player_corner.getX());
    player_rect.incY(player_corner.getY());

    woundable_rects_iterator = woundable_rects.begin();

    for(int er = 0; er < RectMap::getRectCount(); er++)
    {
      IntRect enemy_rect(RectMap::getRect(er));

      enemy_rect.incX(enemy_corner.getX());
      enemy_rect.incY(enemy_corner.getY());

      if(enemy_rect.isCollision(player_rect))
      {
        if(*woundable_rects_iterator == er)
        {
          res = ENEMY_PLAYER_COLLISION_ENEMY_DAMAGED;
          woundable_rects_iterator++;
        }
        else
        {
          return ENEMY_PLAYER_COLLISION_PLAYER_DAMAGED;
        }
      }
    }
  }

  return res;
}


void Enemy::update()
{
  Unit::update();
}

