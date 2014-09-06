#include "./Corpse.hpp"

using namespace std;
using namespace gfx;
using namespace geo;
using namespace util;


Corpse::Corpse(const util::Timer &timer, ENEMY_TYPE enemy_type, ENEMY_DEATH_TYPE death_type):
  Object(timer, OBJECT_CORPSE)
{
  this->enemy_type = enemy_type;
  this->death_type = death_type;

  to_be_removed = false;
}

Corpse::~Corpse()
{

}

bool Corpse::isToBeRemoved() const
{
  return to_be_removed;
}

ENEMY_TYPE Corpse::getEnemyType() const
{
  return enemy_type;
}

ENEMY_DEATH_TYPE Corpse::getDeathType() const
{
  return death_type;
}


void Corpse::update()
{
  Object::update();
}

