#include "Egg.hpp"

using namespace std;;
using namespace geo;
using namespace gfx;
using namespace util;
using namespace etc;
using namespace error;

namespace corpse
{
  Animation Egg::static_data::rolling_left;
  Animation Egg::static_data::rolling_right;
  Animation Egg::static_data::squished_left;
  Animation Egg::static_data::squished_right;

  void Egg::loadStaticData()
  {
    try
    {
//      static_data::rolling_left.load("dat/corpses/egg/rolling_left.ani");
//      static_data::rolling_right.load("dat/corpses/egg/rolling_right.ani");
      static_data::squished_left.load("dat/enemies/egg/corpse/squished_left.ani");
      static_data::squished_right.load("dat/enemies/egg/corpse/squished_right.ani");
    }
    catch(Exception &exception)
    {
      throw Exception() << "Error loading static data of corpse::Egg: " << exception;
    }
  }

  void Egg::clearStaticData()
  {
    static_data::rolling_left.clear();
    static_data::rolling_right.clear();
    static_data::squished_left.clear();
    static_data::squished_right.clear();
  }


  Egg::Egg(const util::Timer &timer, const Enemy &enemy, ENEMY_DEATH_TYPE death_type):
    Corpse(timer, ENEMY_EGG, death_type)
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
        RectMap::addRect(IntRect(0, 0, 64, 64));
        Object::setBoundsSize(IntSize(64, 64));
        Object::setPosition(enemy.getPosition());
        Object::getGravity().setActive(true);
      }
      break;
      case ENEMY_DEATH_BY_BULLET:
      {
        if(enemy.getMoveDirection() == UNIT_MOVE_LEFT){
          anim = &static_data::rolling_left;
        }
        else if(enemy.getMoveDirection() == UNIT_MOVE_RIGHT){
          anim = &static_data::rolling_right;
        }
        else{
          anim = &static_data::rolling_right;
        }

        movement = enemy.getSpeed() * enemy.getXMoveDirection();

        Object::setBoundsSize(IntSize(38, 36));
        Object::setPosition(enemy.getPosition());
        Object::getGravity().setActive(true);
      }
      break;
    }

    Object::setAlignment(ALIGN_BY_CENTER);
  }

  Egg::~Egg()
  {

  }

  void Egg::update()
  {
    Corpse::update();

    Object::getMovementBuffer().pushVector(FloatVector(Object::getTimer().getPerSecond(movement), 0));
  }

  void Egg::draw(const Camera &camera)
  {
    if(anim != NULL)
      anim->draw(Object::getPosition() - point_to_vector(camera.getPosition()), etc::ALIGN_BY_CENTER, IntRect(0, 0, -1, -1));
  }
}
