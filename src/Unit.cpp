#include "./Unit.hpp"

using namespace std;
using namespace geo;
using namespace util;
using namespace error;


Unit::Unit(const util::Timer &timer, OBJECT_TYPE type, const tmx::base::Object &object):
  Object(timer, type)
{
  int tw, th;

  tw = object.getObjectgroup().getMap().getTileWidth();
  th = object.getObjectgroup().getMap().getTileHeight();

  move_direction = UNIT_MOVE_NOT;


  /* Speed */
  if(object.getPropertiesConst().hasValue("speed"))
    speed = string_to_float(object.getPropertiesConst().getValue("speed")) * sqrt(tw*tw + th*th);
  else
    throw Exception() << "No speed defined in Unit " << object;


  /* Movement */
  if(object.getPropertiesConst().hasValue("axis_patrol"))
  {
    vector<string> segs;
    AXIS axis;
    int m1, m2;


    segs = string_split_to_vector(object.getPropertiesConst().getValue("axis_patrol"), "/");
    if(segs.size() != 3)
      throw Exception() << "'axis_patrol' must have 3 segments";

    if(segs[0] == "x")
      axis = AXIS_X;
    else if(segs[0] == "y")
      axis = AXIS_Y;
    else
      throw Exception() << "Unknown axis '" << segs[0] << "'";

    m1 = string_to_integer(segs[1]);
    m2 = string_to_integer(segs[2]);

    if(axis == AXIS_X)
    {
      m1 *= tw;
      m2 *= tw;
        m2 += tw - 1;
    }
    else
    {
      m1 *= th;
      m2 *= th;
        m2 += th - 1;
    }

    setAxisPatrol(axis, m1, m2, 0);
  }
  else if(object.getPropertiesConst().hasValue("axis_const"))
  {
    vector<string> segs;
    AXIS axis;
    s8 dir;

    segs = string_split_to_vector(object.getPropertiesConst().getValue("axis_const"), "/");
    if(segs.size() != 2)
      throw Exception() << "'axis_const' must have 2 segments";

    if(segs[0] == "x")
      axis = AXIS_X;
    else if(segs[0] == "y")
      axis = AXIS_Y;
    else
      throw Exception() << "Unknown axis '" << segs[0] << "'";

    dir = string_to_integer(segs[1]);

    setAxisConst(axis, dir);
  }
  else if(object.getPropertiesConst().hasValue("border_patrol"))
  {
    string axis_str;
    vector<string> segs;
    AXIS axis;
    s8 dir;

    segs = string_split_to_vector(object.getPropertiesConst().getValue("border_patrol"), "/");


    if(segs.size() == 2)
    {
      int _dir = string_to_integer(segs[1]);
      dir = _dir < 0 ? -1 : _dir > 0 ? 1 : 0;
    }
    else if(segs.size() != 1)
    {
      throw Exception() << "Too many segments in 'border_patrol' of object " << object;
    }
    else
    {
      dir = -1;
    }

    axis_str = segs[0];

    if(axis_str == "x")
      axis = AXIS_X;
    else if(axis_str == "y")
      axis = AXIS_Y;
    else
      throw Exception() << "Unknown axis '" << axis_str << "' in object " << object;


    setBorderPatrol(axis, dir);
  }
  else
  {
    throw Exception() << "No movement specified for object " << object;
  }



  /* Position */
  Object::setPosition(IntPoint(object.getX(), object.getY()));
}


Unit::~Unit()
{

}


bool Unit::isActiveInRect(const geo::IntRect &rect) const
{

  switch(movement_type)
  {
    case UNIT_MOVEMENT_NONE: break;

    case UNIT_MOVEMENT_AXIS_PATROL:
    {
      if(Object::isActiveInRect(rect))
        return true;

      IntPoint p1(getPosition()), p2(getPosition());

      if(axis_patrol.axis == AXIS_X)
      {
        p1.setX(axis_patrol.m1);
        p2.setX(axis_patrol.m2);
      }
      else
      {
        p1.setY(axis_patrol.m1);
        p2.setY(axis_patrol.m2);
      }

      if((p1.getX() >= rect.getX() && p1.getX() < rect.getX() + rect.getWidth()) &&
         (p1.getY() >= rect.getY() && p1.getY() < rect.getY() + rect.getHeight()))
        return true;
      if((p2.getX() >= rect.getX() && p2.getX() < rect.getX() + rect.getWidth()) &&
         (p2.getY() >= rect.getY() && p2.getY() < rect.getY() + rect.getHeight()))
        return true;
    }
    break;

    case UNIT_MOVEMENT_AXIS_CONST:
    {
      IntRect trect(rect);

      if(axis_const.axis == AXIS_X)
      {
        trect.setWidth(trect.getWidth() * 3);
        trect.setX(trect.getX() - rect.getWidth());
      }
      else
      {
        trect.setHeight(trect.getHeight() * 3);
        trect.setY(trect.getY() - rect.getHeight());
      }

      if(Object::isActiveInRect(trect))
        return true;
    }
    break;

    case UNIT_MOVEMENT_BORDER_PATROL:
      if(Object::isActiveInRect(rect))
        return true;
    break;
  }

  return false;
}


const double &Unit::getSpeed() const
{
  return speed;
}


void Unit::setSpeed(const double &speed)
{
  this->speed = speed;
}


UNIT_MOVEMENT_TYPE Unit::getMovementType() const
{
  return movement_type;
}


UNIT_MOVE_DIRECTION Unit::getMoveDirection() const
{
  return move_direction;
}


s8 Unit::getXMoveDirection() const
{
  if(move_direction == UNIT_MOVE_LEFT)
    return -1;
  else if(move_direction == UNIT_MOVE_RIGHT)
    return 1;
  else
    return 0;
}


s8 Unit::getYMoveDirection() const
{
  if(move_direction == UNIT_MOVE_UP)
    return -1;
  else if(move_direction == UNIT_MOVE_DOWN)
    return 1;
  else
    return 0;
}


void Unit::setAxisPatrol(AXIS axis, int m1, int m2, int target)
{
  movement_type = UNIT_MOVEMENT_AXIS_PATROL;

  axis_patrol.axis = axis;
  axis_patrol.m1 = m1;
  axis_patrol.m2 = m2;
  axis_patrol.target = target;
}


void Unit::setAxisConst(AXIS axis, s8 dir)
{
  movement_type = UNIT_MOVEMENT_AXIS_CONST;

  axis_const.axis = axis;
  axis_const.dir = dir;
}

void Unit::setBorderPatrol(AXIS axis, s8 dir)
{
  movement_type = UNIT_MOVEMENT_BORDER_PATROL;

  border_patrol.axis = axis;
  border_patrol.dir = dir;
}


const IntRect &Unit::getBodyrect() const
{
  return bodyrect;
}


void Unit::setBodyrect(const IntRect &bodyrect)
{
  this->bodyrect = bodyrect;
}


void Unit::update()
{
  Object::update();

  switch(movement_type)
  {
    case UNIT_MOVEMENT_AXIS_PATROL:    AxisPatrol(); break;
    case UNIT_MOVEMENT_AXIS_CONST:     AxisConst(); break;
    case UNIT_MOVEMENT_BORDER_PATROL:  BorderPatrol(); break;

    default: break;
  }

}


void Unit::AxisPatrol()
{
  int target;
  int len;
  double step;


  if(axis_patrol.axis == AXIS_X)
  {
    if(axis_patrol.target == 0 && Object::isLefttouch())
      axis_patrol.target = 1;
    if(axis_patrol.target == 1 && Object::isRighttouch())
      axis_patrol.target = 0;


    if(axis_patrol.target == 0)
      target = axis_patrol.m1 - bodyrect.getX();
    else
      target = axis_patrol.m2 - (bodyrect.getX() + bodyrect.getWidth());

    len = target - Object::getPosition().getX();
    step = Object::getTimer().getPerSecond(speed) * sgn(len);

    if(len < 0)
      move_direction = UNIT_MOVE_LEFT;
    else if(len > 0)
      move_direction = UNIT_MOVE_RIGHT;
    else
      move_direction = UNIT_MOVE_NOT;


    if(abs(len) <= fabs(step))
    {
      Object::getMovementBuffer().pushVector(FloatVector(len, 0));
      axis_patrol.target ^= 1;
    }
    else
    {
      Object::getMovementBuffer().pushVector(FloatVector(step, 0));
    }
  }
  else
  {
    if(axis_patrol.target == 0 && Object::isToptouch())
      axis_patrol.target = 1;
    if(axis_patrol.target == 1 && Object::isGroundtouch())
      axis_patrol.target = 0;

    if(axis_patrol.target == 0)
      target = axis_patrol.m1 - bodyrect.getY();
    else
      target = axis_patrol.m2 - (bodyrect.getY() + bodyrect.getHeight());

    len = target - Object::getPosition().getY();
    step = Object::getTimer().getPerSecond(speed) * sgn(len);

    if(len * speed < 0)
      move_direction = UNIT_MOVE_UP;
    else if(len * speed > 0)
      move_direction = UNIT_MOVE_LEFT;
    else
      move_direction = UNIT_MOVE_NOT;

    if(abs(len) <= fabs(step))
    {
      Object::getMovementBuffer().pushVector(FloatVector(0, len));
      axis_patrol.target ^= 1;
    }
    else
    {
      Object::getMovementBuffer().pushVector(FloatVector(0, step));
    }
  }
}


void Unit::AxisConst()
{
  if(axis_const.axis == AXIS_X)
  {
    Object::getMovementBuffer().pushVector(FloatVector(Object::getTimer().getPerSecond(speed) * axis_const.dir, 0));

    if(speed * axis_const.dir < 0)
      move_direction = UNIT_MOVE_LEFT;
    else if(speed * axis_const.dir > 0)
      move_direction = UNIT_MOVE_RIGHT;
    else
      move_direction = UNIT_MOVE_NOT;
  }
  else
  {
    Object::getMovementBuffer().pushVector(FloatVector(0, Object::getTimer().getPerSecond(speed) * axis_const.dir));

    if(speed * axis_const.dir < 0)
      move_direction = UNIT_MOVE_UP;
    else if(speed * axis_const.dir > 0)
      move_direction = UNIT_MOVE_DOWN;
    else
      move_direction = UNIT_MOVE_NOT;
  }
}


void Unit::BorderPatrol()
{
  if(border_patrol.axis == AXIS_X)
  {
    if((border_patrol.dir == -1 && Object::isLefttouch()) ||
       (border_patrol.dir == 1 && Object::isRighttouch()))
        border_patrol.dir *= -1;

    Object::getMovementBuffer().pushVector(FloatVector(border_patrol.dir * Object::getTimer().getPerSecond(speed), 0));

    if(border_patrol.dir < 0)
      move_direction = UNIT_MOVE_LEFT;
    else if(border_patrol.dir > 0)
      move_direction = UNIT_MOVE_RIGHT;
    else
      move_direction = UNIT_MOVE_NOT;
  }
  else
  {
    if((border_patrol.dir == -1 && Object::isToptouch()) ||
       (border_patrol.dir == 1 && Object::isGroundtouch()))
        border_patrol.dir *= -1;

    Object::getMovementBuffer().pushVector(FloatVector(0, border_patrol.dir * Object::getTimer().getPerSecond(speed)));

    if(border_patrol.dir < 0)
      move_direction = UNIT_MOVE_UP;
    else if(border_patrol.dir > 0)
      move_direction = UNIT_MOVE_DOWN;
    else
      move_direction = UNIT_MOVE_NOT;
  }
}
