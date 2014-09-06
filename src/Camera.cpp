#include "./Camera.hpp"

using namespace std;
using namespace geo;
using namespace util;


Camera::Camera(const util::Timer &timer):
  timer(timer)
{

}


Camera::~Camera()
{

}


void Camera::clear()
{
  fixed_position.clear();
  offset.clear();
}


const FloatPoint Camera::getPosition() const
{
  return fixed_position + offset;
}


const FloatPoint &Camera::getFixedPosition() const
{
  return fixed_position;
}


void Camera::setFixedPosition(const FloatPoint &fixed_position)
{
  this->fixed_position = fixed_position;

  if(this->fixed_position.getX() < 0)
    this->fixed_position.setX(0);
  if(this->fixed_position.getY() < 0)
    this->fixed_position.setY(0);

  if(this->fixed_position.getX() > map_size.getWidth() - view_size.getWidth())
    this->fixed_position.setX(map_size.getWidth() - view_size.getWidth());
  if(this->fixed_position.getY() > map_size.getHeight() - view_size.getHeight())
    this->fixed_position.setY(map_size.getHeight() - view_size.getHeight());
}


const FloatVector &Camera::getOffset() const
{
  return offset;
}


void Camera::setOffset(const FloatVector &offset)
{
  this->offset = offset;
}


const IntSize &Camera::getViewSize() const
{
  return view_size;
}


void Camera::setViewSize(const IntSize &size)
{
  view_size = size;
}


const IntSize &Camera::getMapSize() const
{
  return map_size;
}


void Camera::setMapSize(const IntSize &size)
{
  map_size = size;
}


const double &Camera::getSpeed() const
{
  return speed;
}


void Camera::setSpeed(const double &speed)
{
  this->speed = speed;
}


void Camera::move(const IntPoint &target)
{
  FloatPoint target_pos;
  FloatVector vector;
  double len, step;


  target_pos = target - IntVector(view_size.getWidth()/2, view_size.getHeight()/2);

  if(target_pos.getX() < 0)
    target_pos.setX(0);
  if(target_pos.getY() < 0)
    target_pos.setY(0);

  if(target_pos.getX() > map_size.getWidth() - view_size.getWidth())
    target_pos.setX(map_size.getWidth() - view_size.getWidth());
  if(target_pos.getY() > map_size.getHeight() - view_size.getHeight())
    target_pos.setY(map_size.getHeight() - view_size.getHeight());



  vector = target_pos - fixed_position;
  len = vector.getLength();
  step = timer.getPerSecond(speed);

  if(len < step)
    step = len;
  if(len == 0)
    return;


  vector.setX(vector.getX() * (step/len));
  vector.setY(vector.getY() * (step/len));

  fixed_position += vector;
}


void Camera::warp(const IntPoint &target)
{
  FloatPoint target_pos;

  target_pos = target - IntVector(view_size.getWidth()/2, view_size.getHeight()/2);

  if(target_pos.getX() < 0)
    target_pos.setX(0);
  if(target_pos.getY() < 0)
    target_pos.setY(0);

  if(target_pos.getX() > map_size.getWidth() - view_size.getWidth())
    target_pos.setX(map_size.getWidth() - view_size.getWidth());
  if(target_pos.getY() > map_size.getHeight() - view_size.getHeight())
    target_pos.setY(map_size.getHeight() - view_size.getHeight());

  fixed_position = target_pos;
}


