#include "./SpecialTile.hpp"

using namespace std;
using namespace etc;
using namespace geo;
using namespace util;
using namespace error;


SpecialTile::SpecialTile(const Timer &timer, SPECIAL_TILE_TYPE type, const tmx::base::Properties &properties, const tmx::base::TileLayer &tilelayer):
  timer(timer)
{
  this->type = type;
  phase = 0;

  /* Collision */
  RectMap::setSize(IntSize(tilelayer.getMap().getTileWidth(), tilelayer.getMap().getTileHeight()));

  if(properties.hasValue("rect[0]"))
  {
    int ri = 0;

    do
    {
      string rect_str;
      vector<string> rect_segs;
      IntRect rect;

      rect_str = properties.getValue(string("rect[") + integer_to_string(ri) + string("]"));
      rect_segs = string_split_to_vector(rect_str, "/");

      if(rect_segs.size() != 4)
        throw Exception() << "Couldn't load rect " << ri << ", wrong rect string '" << rect_str << "'";

      rect.setX((int)(string_to_float(rect_segs[0]) * tilelayer.getMap().getTileWidth()));
      rect.setY((int)(string_to_float(rect_segs[1]) * tilelayer.getMap().getTileHeight()));
      rect.setWidth((int)(string_to_float(rect_segs[2]) * tilelayer.getMap().getTileWidth()));
      rect.setHeight((int)(string_to_float(rect_segs[3]) * tilelayer.getMap().getTileHeight()));

      RectMap::addRect(rect);

      ri++;
    }while(properties.hasValue(string("rect[") + integer_to_string(ri) + string("]")));
  }
  else
  {
    IntRect rect;

    rect.setX(0);
    rect.setY(0);
    rect.setWidth(tilelayer.getMap().getTileWidth());
    rect.setHeight(tilelayer.getMap().getTileHeight());

    RectMap::addRect(rect);
  }
}


SpecialTile::~SpecialTile()
{

}


const Timer &SpecialTile::getTimer() const
{
  return timer;
}


SPECIAL_TILE_TYPE SpecialTile::getType() const
{
  return type;
}


int SpecialTile::getPhase() const
{
  return phase;
}


void SpecialTile::nextPhase()
{
  phase++;
}


const geo::IntPoint &SpecialTile::getPosition() const
{
  return position;
}


void SpecialTile::setPosition(const geo::IntPoint &position)
{
  this->position = position;
}


bool SpecialTile::isCollision(const Object &object) const
{
  return RectMap::isCollision(object, position, object.getPosition(), IntRect(0, 0, -1, -1), IntRect(0, 0, -1, -1), ALIGN_BY_CORNER, object.getAlignment());
}


SPECIAL_TILE_TYPE special_tile_string_to_type(const std::string &string)
{
  if(string == "container")
    return SPECIAL_TILE_CONTAINER;
  else if(string == "fragile")
    return SPECIAL_TILE_FRAGILE;
  else if(string == "damaging")
    return SPECIAL_TILE_DAMAGING;
  else
    throw Exception() << "Unknown SpecialTile type '" << string << "'";
}
