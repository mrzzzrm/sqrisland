#include "./Item.hpp"

using namespace geo;


Item::Item(const util::Timer &timer, ITEM_TYPE type, const tmx::base::Object &object):
  Object(timer, OBJECT_ITEM)
{
  this->type = type;
  Object::setPosition(IntPoint(object.getX(), object.getY()));
}

Item::Item(const util::Timer &timer, ITEM_TYPE type):
  Object(timer, OBJECT_ITEM)
{
  this->type = type;
  Object::setPosition(IntPoint(0, 0));
}


Item::~Item()
{

}


ITEM_TYPE Item::getType() const
{
  return type;
}


void Item::update()
{
  Object::update();
}


ITEM_TYPE item_string_to_type(const std::string &string)
{
  if(string == "coin")
    return ITEM_COIN;
  else if(string == "upgrader")
    return ITEM_UPGRADER;
  else if(string == "life")
    return ITEM_LIFE;
  else
    throw error::Exception() << "Unknown item-type '" << string << "'";
}
