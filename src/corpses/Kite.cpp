#include "Kite.hpp"

using namespace std;;
using namespace geo;
using namespace gfx;
using namespace util;
using namespace etc;
using namespace error;

namespace corpse
{
  Animation Kite::static_data::falling;

  void Kite::loadStaticData()
  {
    try
    {
      static_data::falling.load("dat/enemies/kite/corpse/falling.ani");
    }
    catch(Exception &exception)
    {
      throw Exception() << "Error loading static data of corpse::Kite: " << exception;
    }
  }

  void Kite::clearStaticData()
  {
    static_data::falling.clear();
  }


  Kite::Kite(const util::Timer &timer, const Enemy &enemy, ENEMY_DEATH_TYPE death_type):
    Corpse(timer, ENEMY_KITE, death_type)
  {
    anim = &static_data::falling;

    movement = enemy.getSpeed() * 2.5;

    Object::setBoundsSize(IntSize(96, 96));
    Object::setPosition(enemy.getPosition());
    Object::getGravity().setActive(false);
    Object::setAlignment(ALIGN_BY_CENTER);
  }

  Kite::~Kite()
  {

  }

  void Kite::update()
  {
    Corpse::update();

    Object::getMovementBuffer().pushVector(FloatVector(0, Object::getTimer().getPerSecond(movement)));
  }

  void Kite::draw(const Camera &camera)
  {
    if(anim != NULL)
      anim->draw(Object::getPosition() - point_to_vector(camera.getPosition()), etc::ALIGN_BY_CENTER, IntRect(0, 0, -1, -1));
  }
}
