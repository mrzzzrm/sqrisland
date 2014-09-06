#include "./Life.hpp"


using namespace geo;
using namespace gfx;
using namespace etc;
using namespace util;
using namespace error;


namespace item
{
  Animation Life::static_data::anim;

  void Life::loadStaticData()
  {
    try
    {
      static_data::anim.load("dat/items/life.ani");
    }
    catch(error::Exception &exception)
    {
      throw Exception() << "Error loading static data of item::Life " << exception;
    }
  }

  void Life::clearStaticData()
  {
    static_data::anim.clear();
  }

  Life::Life(const util::Timer &timer, const tmx::base::Object &object):
    Item(timer, ITEM_LIFE, object)
  {
    initBases();
    anim = &static_data::anim;
    dir = 0;
  }

  Life::Life(const util::Timer &timer):
    Item(timer, ITEM_LIFE)
  {
    initBases();
    anim = &static_data::anim;
    dir = IntRandom(2) * 2 - 1;
  }


  Life::~Life()
  {

  }


  void Life::update()
  {
    Item::update();
    Object::getMovementBuffer().pushVector(getTimer().getPerSecond(FloatVector(dir * 150, 0)));
  }


  void Life::draw(const Camera &camera)
  {
    if(anim != NULL)
      anim->draw(Object::getPosition() - point_to_vector(camera.getPosition()), etc::ALIGN_BY_CENTER, IntRect(0, 0, -1, -1));
  }

  void Life::initBases()
  {
    RectMap::setSize(IntSize(40, 40));
    RectMap::addRect(IntRect(0, 0, 40, 40));

    Object::setAlignment(ALIGN_BY_CENTER);
    Object::getGravity().setActive(true);
    Object::setBoundsSize(IntSize(40, 40));
  }

}

