#include "./Tri.hpp"

using namespace std;
using namespace etc;
using namespace gfx;
using namespace geo;
using namespace util;
using namespace error;


namespace enemy
{
  Animation Tri::static_data::walking_left;
  Animation Tri::static_data::walking_right;

  void Tri::loadStaticData()
  {
    try
    {
      static_data::walking_left.load("dat/enemies/tri/walking_left.ani");
      static_data::walking_right.load("dat/enemies/tri/walking_right.ani");
    }
    catch(Exception &exception)
    {
      throw Exception() << "Error loading static data of enemy::Tri: " << exception;
    }
  }

  void Tri::clearStaticData()
  {
    static_data::walking_left.clear();
    static_data::walking_right.clear();
  }


  Tri::Tri(const util::Timer &timer, const tmx::base::Object &object):
    Enemy(timer, ENEMY_TRI, object)
  {
    anim = NULL;

    RectMap::setSize(IntSize(61, 55));
    RectMap::addRect(IntRect(2, 18, 55, 37));
    RectMap::addRect(IntRect(2, 8, 55, 10)); Enemy::setRectWoundable(1);

    Object::setAlignment(ALIGN_BY_CENTER);
    Object::getGravity().setActive(true);
    Object::setBoundsSize(IntSize(61, 55));

    Unit::setBodyrect(IntRect(-30, -27, 61, 55));
  }


  Tri::~Tri()
  {

  }


  void Tri::update()
  {
    Enemy::update();

    if(Enemy::getMoveDirection() == UNIT_MOVE_LEFT)
      anim = &static_data::walking_left;
    else if(Enemy::getMoveDirection() == UNIT_MOVE_RIGHT)
      anim = &static_data::walking_right;
    else
      anim = &static_data::walking_right;
  }


  void Tri::draw(const Camera &camera)
  {
    if(anim != NULL)
      anim->draw(Object::getPosition() - point_to_vector(camera.getPosition()), etc::ALIGN_BY_CENTER, IntRect(0, 0, -1, -1));
  }
}
