#include "./MovementBuffer.hpp"

using namespace std;
using namespace geo;



MovementBuffer::MovementBuffer(const geo::FloatVector &vector)
{
  load(vector);
}


MovementBuffer::MovementBuffer(const MovementBuffer &movementbuffer)
{
  load(movementbuffer);
}


MovementBuffer::~MovementBuffer()
{

}


void MovementBuffer::load(const geo::FloatVector &vector)
{
  FloatVector::load(vector);
}


void MovementBuffer::load(const MovementBuffer &movementbuffer)
{
  if(this == &movementbuffer)
    return;

  FloatVector::load(movementbuffer);
}


void MovementBuffer::clear()
{
  FloatVector::clear();
}


bool MovementBuffer::hasIntValue(AXIS axis) const
{
  if(axis == AXIS_X)
  {
    if(FloatVector::getX() >= 1 || FloatVector::getX() <= -1)
      return true;
    else
      return false;
  }
  else
  {
    if(FloatVector::getY() >= 1 || FloatVector::getY() <= -1)
      return true;
    else
      return false;
  }
}



void MovementBuffer::pushVector(const geo::FloatVector &vector)
{
  FloatVector::operator+=(vector);
}


s8 MovementBuffer::pop(AXIS axis)
{
  if(axis == AXIS_X)
  {
    if(FloatVector::getX() >= 1)
    {
      FloatVector::decX(1);
      return 1;
    }
    else if(FloatVector::getX() <= -1)
    {
      FloatVector::incX(1);
      return -1;
    }
    else
    {
      return 0;
    }
  }
  else
  {
    if(FloatVector::getY() >= 1)
    {
      FloatVector::decY(1);
      return 1;
    }
    else if(FloatVector::getY() <= -1)
    {
      FloatVector::incY(1);
      return -1;
    }
    else
    {
      return 0;
    }
  }

}


void MovementBuffer::push(AXIS axis, s8 dir)
{
  if(axis == AXIS_X)
    FloatVector::incX(dir);
  else
    FloatVector::incY(dir);
}


void MovementBuffer::clearIntValue(AXIS axis)
{
  if(axis == AXIS_X)
    FloatVector::decX((int)FloatVector::getX());
  else
    FloatVector::decY((int)FloatVector::getY());
}


