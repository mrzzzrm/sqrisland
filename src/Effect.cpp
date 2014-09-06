#include "./Effect.hpp"

using namespace std;
using namespace geo;
using namespace etc;


Effect::Effect(const util::Timer &timer, EFFECT_TYPE type):
  Object(timer, OBJECT_EFFECT)
{
  this->type = type;
}


Effect::~Effect()
{

}


EFFECT_TYPE Effect::getType() const
{
  return type;
}


