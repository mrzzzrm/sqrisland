#include "Punk.hpp"


using namespace std;
using namespace gfx;
using namespace etc;
using namespace geo;
using namespace util;
using namespace error;



namespace enemy
{
  Animation Punk::static_data::bouncing_left;
  Animation Punk::static_data::bouncing_right;


  void Punk::loadStaticData()
  {
    try
    {
      static_data::bouncing_left.load("dat/enemies/punk/bouncing_left.ani");
      static_data::bouncing_right.load("dat/enemies/punk/bouncing_right.ani");
    }
    catch(Exception &exception)
    {
      throw Exception() << "Error loading static data of enemy::Punk: " << exception;
    }
  }

  void Punk::clearStaticData()
  {
    static_data::bouncing_left.clear();
    static_data::bouncing_right.clear();
  }

  Punk::Punk(const util::Timer &timer, const tmx::base::Object &object):
    BouncingEnemy(timer, ENEMY_PUNK, object)
  {
    anim = NULL;

    RectMap::setSize(IntSize(64, 64));
    RectMap::addRect(IntRect(9, 20, 49, 38));
    RectMap::addRect(IntRect(9, 10, 49, 10)); Enemy::setRectWoundable(1);

    Object::setAlignment(ALIGN_BY_CENTER);
    Object::getGravity().setActive(true);
    Object::setBoundsSize(IntSize(64, 64));

    Unit::setBodyrect(IntRect(-32, -32, 64, 64));
  }

  Punk::~Punk()
  {

  }

  void Punk::update()
  {
    BouncingEnemy::update();

    if(Enemy::getMoveDirection() == UNIT_MOVE_LEFT)
      anim = &static_data::bouncing_left;
    else if(Enemy::getMoveDirection() == UNIT_MOVE_RIGHT)
      anim = &static_data::bouncing_right;
    else
      anim = &static_data::bouncing_right;
  }

  void Punk::draw(const Camera &camera)
  {
    if(anim != NULL)
      anim->draw(Object::getPosition() - point_to_vector(camera.getPosition()), etc::ALIGN_BY_CENTER, IntRect(0, 0, -1, -1));
  }
}

