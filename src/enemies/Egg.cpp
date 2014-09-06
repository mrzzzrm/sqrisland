#include "./Egg.hpp"

using namespace std;
using namespace etc;
using namespace gfx;
using namespace geo;
using namespace util;
using namespace error;

namespace enemy
{
  Animation Egg::static_data::rolling_left;
  Animation Egg::static_data::rolling_right;

  void Egg::loadStaticData()
  {
    try
    {
      static_data::rolling_left.load("dat/enemies/egg/rolling_left.ani");
      static_data::rolling_right.load("dat/enemies/egg/rolling_right.ani");
    }
    catch(Exception &exception)
    {
      throw Exception() << "Error loading static data of enemy::Egg: " << exception;
    }
  }

  void Egg::clearStaticData()
  {
    static_data::rolling_left.clear();
    static_data::rolling_right.clear();
  }


  Egg::Egg(const util::Timer &timer, const tmx::base::Object &object):
    Enemy(timer, ENEMY_EGG, object)
  {
    anim = NULL;

    RectMap::setSize(IntSize(64, 64));
    RectMap::addRect(IntRect(10, 20, 44, 54));
    RectMap::addRect(IntRect(10, 10, 44, 10)); Enemy::setRectWoundable(1);

    Object::setAlignment(ALIGN_BY_CENTER);
    Object::getGravity().setActive(true);
    Object::setBoundsSize(IntSize(64, 64));

    Unit::setBodyrect(IntRect(-32, -32, 64, 64));
  }


  Egg::~Egg()
  {

  }


  void Egg::update()
  {
    Enemy::update();

    if(Enemy::getMoveDirection() == UNIT_MOVE_LEFT)
      anim = &static_data::rolling_left;
    else if(Enemy::getMoveDirection() == UNIT_MOVE_RIGHT)
      anim = &static_data::rolling_right;
    else
      anim = &static_data::rolling_right;
  }


  void Egg::draw(const Camera &camera)
  {
    if(anim != NULL)
      anim->draw(Object::getPosition() - point_to_vector(camera.getPosition()), etc::ALIGN_BY_CENTER, IntRect(0, 0, -1, -1));
  }
}
