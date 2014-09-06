#include "Tri.hpp"

using namespace std;;
using namespace geo;
using namespace gfx;
using namespace util;
using namespace etc;
using namespace error;


namespace corpse
{
  Animation Tri::static_data::falling_left;
  Animation Tri::static_data::falling_right;
  Animation Tri::static_data::squished_left;
  Animation Tri::static_data::squished_right;

  void Tri::loadStaticData()
  {
    try
    {
      static_data::falling_left.load("dat/enemies/tri/corpse/falling_left.ani");
      static_data::falling_right.load("dat/enemies/tri/corpse/falling_right.ani");
      static_data::squished_left.load("dat/enemies/tri/corpse/squished_left.ani");
      static_data::squished_right.load("dat/enemies/tri/corpse/squished_right.ani");
    }
    catch(Exception &exception)
    {
      throw Exception() << "Error loading static data of corpse::Tri: " << exception;
    }
  }

  void Tri::clearStaticData()
  {
    static_data::falling_left.clear();
    static_data::falling_right.clear();
    static_data::squished_left.clear();
    static_data::squished_right.clear();
  }


  Tri::Tri(const util::Timer &timer, const Enemy &enemy, ENEMY_DEATH_TYPE death_type):
    Corpse(timer, ENEMY_TRI, death_type)
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

        RectMap::setSize(IntSize(61, 55));
        RectMap::addRect(IntRect(5, 38, 51, 14));
        Object::setBoundsSize(IntSize(61, 55));
        Object::setPosition(enemy.getPosition());
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

        Object::setBoundsSize(IntSize(61, 55));
        Object::setPosition(enemy.getPosition());
        Object::getGravity().setActive(true);
      }
      break;
    }

    Object::setAlignment(ALIGN_BY_CENTER);
  }

  Tri::~Tri()
  {

  }

  void Tri::update()
  {
    Corpse::update();

    Object::getMovementBuffer().pushVector(FloatVector(Object::getTimer().getPerSecond(movement), 0));
  }

  void Tri::draw(const Camera &camera)
  {
    if(anim != NULL)
      anim->draw(Object::getPosition() - point_to_vector(camera.getPosition()), etc::ALIGN_BY_CENTER, IntRect(0, 0, -1, -1));
  }
}
