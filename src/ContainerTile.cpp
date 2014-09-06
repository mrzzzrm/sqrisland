#include "./ContainerTile.hpp"

using namespace std;
using namespace gfx;
using namespace util;
using namespace error;


ContainerTile::ContainerTile(const util::Timer &timer, const tmx::base::Properties &properties, const tmx::base::TileLayer &tilelayer):
  SpecialTile(timer, SPECIAL_TILE_CONTAINER, properties, tilelayer)
{
  item_type = ITEM_NONE;
}


ContainerTile::~ContainerTile()
{

}


void ContainerTile::nextPhase()
{
  SpecialTile::nextPhase();

  if(getPhase() < 3)
    animation[getPhase()].reset();
}


Animation &ContainerTile::getAnimation(int index)
{
  if(!in_range(index, 2))
    throw Exception() << "No such animation '" << index << "' in ContainerTile";

  return animation[index];
}


ITEM_TYPE ContainerTile::getItemType() const
{
  return item_type;
}


void ContainerTile::setItemType(ITEM_TYPE item_type)
{
  this->item_type = item_type;
}


void ContainerTile::update()
{
  if(getPhase() == 1 && animation[1].getLoopCount() > 0)
    nextPhase();
}


void ContainerTile::draw(const Camera &camera)
{
  if(getPhase() < 3)
    animation[getPhase()].draw(SpecialTile::getPosition() - point_to_vector(camera.getPosition()), etc::ALIGN_BY_CORNER);
}

