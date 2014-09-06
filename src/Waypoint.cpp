#include "./Waypoint.hpp"


using namespace std;
using namespace geo;


Waypoint::Waypoint(const geo::IntPoint &point, int wait, double speed):
  IntPoint(point)
{
  this->wait = wait;
  this->speed = speed;
}


Waypoint::~Waypoint()
{

}


void Waypoint::clear()
{
  IntPoint::clear();
  speed = 0;
  wait = 0;
}


int Waypoint::getWait() const
{
  return wait;
}


void Waypoint::setWait(int wait)
{
  this->wait = wait;
}


const double &Waypoint::getSpeed() const
{
  return speed;
}


void Waypoint::setSpeed(const double &speed)
{
  this->speed = speed;
}

