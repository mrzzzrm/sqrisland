#include "./FragileTile.hpp"

using namespace std;
using namespace gfx;
using namespace util;
using namespace error;


FragileTile::FragileTile(const util::Timer &timer, const tmx::base::Properties &properties, const tmx::base::TileLayer &tilelayer):
  SpecialTile(timer, SPECIAL_TILE_FRAGILE, properties, tilelayer)
{

}


FragileTile::~FragileTile()
{

}

void FragileTile::nextPhase()
{
  SpecialTile::nextPhase();

  if(getPhase() < 2)
    animation[getPhase()].reset();
}


Animation &FragileTile::getAnimation(int index)
{
  if(!in_range(index, 1))
    throw Exception() << "No such animation '" << index << "' in FragileTile";

  return animation[index];
}



void FragileTile::update()
{
  if(getPhase() == 1 && animation[1].getLoopCount() > 0)
    nextPhase();
}


void FragileTile::draw(const Camera &camera)
{
  if(getPhase() < 2)
    animation[getPhase()].draw(SpecialTile::getPosition() - point_to_vector(camera.getPosition()), etc::ALIGN_BY_CORNER);
}

