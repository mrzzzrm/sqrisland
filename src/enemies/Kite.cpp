#include "./Kite.hpp"

using namespace std;
using namespace etc;
using namespace gfx;
using namespace geo;
using namespace util;
using namespace error;

namespace enemy
{
  Animation Kite::static_data::flying_up;
  Animation Kite::static_data::flying_down;

  void Kite::loadStaticData()
  {
    try
    {
      static_data::flying_up.load("dat/enemies/kite/flying_up.ani");
      static_data::flying_down.load("dat/enemies/kite/flying_down.ani");
    }
    catch(Exception &exception)
    {
      throw Exception() << "Error loading static data of enemy::Kite: " << exception;
    }
  }

  void Kite::clearStaticData()
  {
    static_data::flying_up.clear();
    static_data::flying_down.clear();
  }


  Kite::Kite(const util::Timer &timer, const tmx::base::Object &object):
    Enemy(timer, ENEMY_KITE, object)
  {
    anim = NULL;

    RectMap::setSize(IntSize(96, 96));
    RectMap::addRect(IntRect(25, 10, 40, 50));
    RectMap::addRect(IntRect(25, 0, 40, 10)); Enemy::setRectWoundable(1);

    Object::setAlignment(ALIGN_BY_CENTER);
    Object::getGravity().setActive(false);
    Object::setBoundsSize(IntSize(96, 96));

    Unit::setBodyrect(IntRect(-48, -48, 96, 96));
  }


  Kite::~Kite()
  {

  }


  void Kite::update()
  {
    Enemy::update();

    if(Enemy::getMoveDirection() == UNIT_MOVE_UP)
      anim = &static_data::flying_up;
    else if(Enemy::getMoveDirection() == UNIT_MOVE_DOWN)
      anim = &static_data::flying_down;
    else
      anim = &static_data::flying_down;
  }


  void Kite::draw(const Camera &camera)
  {
    if(anim != NULL)
      anim->draw(Object::getPosition() - point_to_vector(camera.getPosition()), etc::ALIGN_BY_CENTER, IntRect(0, 0, -1, -1));
  }
}
