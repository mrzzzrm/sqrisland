#include "./Upgrader.hpp"


using namespace geo;
using namespace gfx;
using namespace etc;
using namespace util;
using namespace error;


namespace item
{
  Animation Upgrader::static_data::anim;

  void Upgrader::loadStaticData()
  {
    try
    {
      static_data::anim.load("dat/items/upgrader.ani");
    }
    catch(error::Exception &exception)
    {
      throw Exception() << "Error loading static data of item::Upgrader " << exception;
    }
  }

  void Upgrader::clearStaticData()
  {
    static_data::anim.clear();
  }

  Upgrader::Upgrader(const util::Timer &timer, const tmx::base::Object &object):
    Item(timer, ITEM_UPGRADER, object)
  {
    initBases();
    anim = &static_data::anim;
    dir = 0;
  }

  Upgrader::Upgrader(const util::Timer &timer):
    Item(timer, ITEM_UPGRADER)
  {
    initBases();
    anim = &static_data::anim;
    dir = IntRandom(2) * 2 - 1;
  }


  Upgrader::~Upgrader()
  {

  }


  void Upgrader::update()
  {
    Item::update();

    if(Object::isRighttouch())
      dir = -1;
    if(Object::isLefttouch())
      dir = 1;
    if(Object::isGroundtouch())
      Object::getGravity().setValue(-500);


    Object::getMovementBuffer().pushVector(getTimer().getPerSecond(FloatVector(dir * 150, 0)));
  }


  void Upgrader::draw(const Camera &camera)
  {
    if(anim != NULL)
      anim->draw(Object::getPosition() - point_to_vector(camera.getPosition()), etc::ALIGN_BY_CENTER, IntRect(0, 0, -1, -1));
  }

  void Upgrader::initBases()
  {
    RectMap::setSize(IntSize(40, 40));
    RectMap::addRect(IntRect(0, 0, 40, 40));

    Object::setAlignment(ALIGN_BY_CENTER);
    Object::getGravity().setActive(true);
    Object::setBoundsSize(IntSize(40, 40));
  }

}

