#include "./DamagingTile.hpp"

using namespace std;
using namespace gfx;
using namespace error;


DamagingTile::DamagingTile(const util::Timer &timer, const tmx::base::Properties &properties, const tmx::base::TileLayer &tilelayer):
  SpecialTile(timer, SPECIAL_TILE_DAMAGING, properties, tilelayer)
{
  damage = 0;
}


DamagingTile::~DamagingTile()
{

}


Animation &DamagingTile::getAnimation()
{
  return animation;
}


int DamagingTile::getDamage() const
{
  return damage;
}


void DamagingTile::setDamage(int damage)
{
  this->damage = damage;
}


void DamagingTile::update()
{

}


void DamagingTile::draw(const Camera &camera)
{
  animation.draw(SpecialTile::getPosition() - point_to_vector(camera.getPosition()), etc::ALIGN_BY_CORNER);
}

