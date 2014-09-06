#include "./Object.hpp"

using namespace std;
using namespace geo;
using namespace etc;
using namespace util;



Object::Object(const util::Timer &timer, OBJECT_TYPE type, const IntPoint &position):
  timer(timer),
  gravity(timer)
{
  this->type = type;
  this->position = position;

  lefttouch = false;
  righttouch = false;
  toptouch = false;
  groundtouch = false;
}


Object::~Object()
{

}


const Timer &Object::getTimer() const
{
  return timer;
}


const IntPoint &Object::getPosition() const
{
  return position;
}


void Object::setPosition(const IntPoint &position)
{
  this->position = position;
}


void Object::move(AXIS axis, s8 dir)
{
  if(axis == AXIS_X)
    position.incX(dir);
  else
    position.incY(dir);
}


MovementBuffer &Object::getMovementBuffer()
{
  return movementbuffer;
}


const Alignment &Object::getAlignment() const
{
  return alignment;
}


void Object::setAlignment(const Alignment &alignment)
{
  this->alignment = alignment;
  updateBounds();
}


OBJECT_TYPE Object::getType() const
{
  return type;
}


bool Object::isLefttouch() const
{
  return lefttouch;
}


void Object::setLefttouch(bool is)
{
  lefttouch = is;
}


bool Object::isRighttouch() const
{
  return righttouch;
}


void Object::setRighttouch(bool is)
{
  righttouch = is;
}

bool Object::isToptouch() const
{
  return toptouch;
}


void Object::setToptouch(bool is)
{
  toptouch = is;
}


bool Object::isGroundtouch() const
{
  return groundtouch;
}


void Object::setGroundtouch(bool is)
{
  groundtouch = is;
}


Gravity &Object::getGravity()
{
  return gravity;
}


void Object::setBoundsSize(const geo::IntSize &size)
{
  bounds.size = size;
  updateBounds();
}


bool Object::isBoundsCollision(const Object &object) const
{
  int x, y, r;

  x = (position.getX() + bounds.circle_offset.getX()) -  (object.position.getX() + object.bounds.circle_offset.getX());
  y = (position.getY() + bounds.circle_offset.getY()) -  (object.position.getY() + object.bounds.circle_offset.getY());
  r = bounds.circle_radius + object.bounds.circle_radius;

  if(x*x + y*y <= r*r)
    return true;
  else
    return false;
}


bool Object::isBoundsCollision(const geo::IntRect &rect) const
{
  return rect.isCollision(bounds.rect + point_to_vector(position));
}


bool Object::isActiveInRect(const geo::IntRect &rect) const
{
  return isBoundsCollision(rect);
}


void Object::update()
{
  if(groundtouch && gravity.getValue() > 0)
  {
    gravity.flush();
    movementbuffer.pushVector(IntVector(0, 1)); // Ugly hack to prevent landing on damaging tiles
  }
  if(toptouch && gravity.getValue() < 0)
  {
    gravity.flush();
    movementbuffer.pushVector(IntVector(0, -1)); // Ugly hack to prevent landing on damaging tiles
  }

  if(gravity.isActive())
    movementbuffer.pushVector(FloatVector(0, timer.getPerSecond(gravity.getValue())));
}


bool Object::isCollision(const Object &object) const
{
  return RectMap::isCollision(object, position, object.position, IntRect(0, 0, -1, -1), IntRect(0, 0, -1, -1), alignment, object.alignment);
}


void Object::updateBounds()
{
  bounds.rect.load
    (
      (int)(alignment.getX() * bounds.size.getWidth()),
      (int)(alignment.getY() * bounds.size.getHeight()),
      (int)(bounds.size.getWidth()),
      (int)(bounds.size.getHeight())
    );

  bounds.circle_radius = (int)sqrt((bounds.size.getWidth()*bounds.size.getWidth())/4 + (bounds.size.getHeight()*bounds.size.getHeight())/4) + 1;

  bounds.circle_offset.load
    (
      (int)((0.5 - alignment.getX()) * bounds.size.getWidth()),
      (int)((0.5 - alignment.getY()) * bounds.size.getHeight())
    );
}

