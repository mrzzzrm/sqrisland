#include "./Jump.hpp"


Jump::Jump(const util::Timer &timer):
  timer(timer)
{
  _is = false;
  left_ground = false;
  begin = 0;
  dur = 0;
  breakable = false;
}


Jump::~Jump()
{

}


void Jump::start(time_t dur)
{
  begin = timer.getTicks();
  this->dur = dur;
  left_ground = false;
  _is = true;
  breakable = true;
}


void Jump::end()
{
  _is = false;
}


bool Jump::is() const
{
  return _is;
}


bool Jump::hasLeftGround() const
{
  return left_ground;
}


void Jump::setLeftGround(bool is)
{
  left_ground = is;
}


bool Jump::isBreakable() const
{
  return breakable;
}


void Jump::setBreakable(bool is)
{
  breakable = is;
}


double Jump::getRelativeForce() const
{
  return 1.0 - ((double)(timer.getTicks() - begin))/dur;
}


void Jump::update()
{
  if(timer.getTicks() - begin > dur)
  {
    _is = false;
  }
}
