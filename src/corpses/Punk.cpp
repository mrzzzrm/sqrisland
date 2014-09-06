#include "Punk.hpp"

using namespace std;;
using namespace geo;
using namespace gfx;
using namespace util;
using namespace etc;
using namespace error;


namespace corpse
{
  Animation Punk::static_data::falling_left;
  Animation Punk::static_data::falling_right;
  Animation Punk::static_data::squished_left;
  Animation Punk::static_data::squished_right;

  void Punk::loadStaticData()
  {
    try
    {
      static_data::falling_left.load("dat/enemies/punk/corpse/falling_left.ani");
      static_data::falling_right.load("dat/enemies/punk/corpse/falling_right.ani");
      static_data::squished_left.load("dat/enemies/punk/corpse/squished_left.ani");
      static_data::squished_right.load("dat/enemies/punk/corpse/squished_right.ani");
    }
    catch(Exception &exception)
    {
      throw Exception() << "Error loading static data of corpse::Punk: " << exception;
    }
  }

  void Punk::clearStaticData()
  {
    static_data::falling_left.clear();
    static_data::falling_right.clear();
    static_data::squished_left.clear();
    static_data::squished_right.clear();
  }


  Punk::Punk(const util::Timer &timer, const Enemy &enemy, ENEMY_DEATH_TYPE death_type):
    Corpse(timer, ENEMY_PUNK, death_type)
  {
    movement = 0;

    switch(death_type)
    {
      case ENEMY_DEATH_BY_JUMP:
      {
        if(enemy.getMoveDirection() == UNIT_MOVE_LEFT)
          anim = &static_data::squished_left;
        else if(enemy.getMoveDirection() == UNIT_MOVE_RIGHT)
          anim = &static_data::squished_right;
        else
          anim = &static_data::squished_right;

        RectMap::setSize(IntSize(64, 64));
        RectMap::addRect(IntRect(10, 42, 44, 20));
        Object::setBoundsSize(IntSize(64, 64));
        Object::setPosition(enemy.getPosition() + IntVector(0, 0));
        Object::getGravity().setActive(true);
      }
      break;
      case ENEMY_DEATH_BY_BULLET:
      {
        if(enemy.getMoveDirection() == UNIT_MOVE_LEFT){
          anim = &static_data::falling_left;
        }
        else if(enemy.getMoveDirection() == UNIT_MOVE_RIGHT){
          anim = &static_data::falling_right;
        }
        else{
          anim = &static_data::falling_right;
        }

        movement = enemy.getSpeed() * enemy.getXMoveDirection();

        Object::setBoundsSize(IntSize(64, 64));
        Object::setPosition(enemy.getPosition());
        Object::getGravity().setActive(true);
      }
      break;
    }

    Object::setAlignment(ALIGN_BY_CENTER);
  }

  Punk::~Punk()
  {

  }

  void Punk::update()
  {
    Corpse::update();

    Object::getMovementBuffer().pushVector(FloatVector(Object::getTimer().getPerSecond(movement), 0));
  }

  void Punk::draw(const Camera &camera)
  {
    if(anim != NULL)
      anim->draw(Object::getPosition() - point_to_vector(camera.getPosition()), etc::ALIGN_BY_CENTER, IntRect(0, 0, -1, -1));
  }
}
