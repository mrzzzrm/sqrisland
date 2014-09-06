#include "./Gravity.hpp"



Gravity::Gravity(const util::Timer &timer):
  timer(timer)
{
  active = false;
  value = 0;
}


Gravity::~Gravity()
{

}


bool Gravity::isActive() const
{
  return active;
}


void Gravity::setActive(bool is)
{
  active = is;
  if(!is)
    value = 0;
}


const double &Gravity::getValue() const
{
  return value;
}


void Gravity::setValue(const double &value)
{
  this->value = value;
}


void Gravity::limit(const double &max)
{
  if(max >= 0)
  {
    if(value > max)
      value = max;
  }
}


void Gravity::flush()
{
  value = 0;
}


void Gravity::accelerate(const double &acc)
{
  value += timer.getPerSecond(acc);
}

