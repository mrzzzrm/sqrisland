#include "./Bullet.hpp"

using namespace std;
using namespace geo;
using namespace util;


Bullet::Bullet(const Timer &timer, BULLET_TYPE type):
  Object(timer, OBJECT_BULLET)
{
  this->type = type;
}


Bullet::~Bullet()
{

}


BULLET_TYPE Bullet::getType() const
{
  return type;
}


Movement &Bullet::getMovement()
{
  return movement;
}


void Bullet::setMovement(const util::Movement &movement)
{
  this->movement = movement;
}


void Bullet::update()
{
  Object::update();

  movement.update();
  Object::getMovementBuffer().pushVector(Object::getTimer().getPerSecond(movement));
}

