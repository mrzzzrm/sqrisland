#include "./BouncingEnemy.hpp"

using namespace std;
using namespace util;
using namespace error;


BouncingEnemy::BouncingEnemy(const Timer &timer, ENEMY_TYPE type, const tmx::base::Object &object):
  Enemy(timer, type, object)
{
  if(!object.getPropertiesConst().hasValue("bounce_force"))
    throw Exception() << "No property 'bounce_force' set in object " << object;

  bounce_force = string_to_float(object.getPropertiesConst().getValue("bounce_force"));
}

BouncingEnemy::~BouncingEnemy()
{

}

const double &BouncingEnemy::getBounceForce() const
{
  return bounce_force;
}

void BouncingEnemy::setBounceForce(const double &bounce_force)
{
  this->bounce_force = bounce_force;
}

void BouncingEnemy::update()
{
  Enemy::update();

  if(Object::isGroundtouch())
    Object::getGravity().setValue(-bounce_force);
}

