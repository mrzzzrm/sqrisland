#include "./Coin.hpp"

using namespace geo;
using namespace gfx;
using namespace etc;
using namespace util;
using namespace error;


namespace item
{
  Animation Coin::static_data::anim;

  void Coin::loadStaticData()
  {
    try
    {
      static_data::anim.load("dat/items/coin.ani");
    }
    catch(error::Exception &exception)
    {
      throw Exception() << "Error loading static data of item::Coin: " << exception;
    }
  }

  void Coin::clearStaticData()
  {
    static_data::anim.clear();
  }

  Coin::Coin(const util::Timer &timer, const tmx::base::Object &object):
    Item(timer, ITEM_COIN, object)
  {
    RectMap::setSize(IntSize(48, 48));

    RectMap::addRect(IntRect(0, 0, 48, 48));

    Object::setAlignment(ALIGN_BY_CENTER);
    Object::getGravity().setActive(false);
    Object::setBoundsSize(IntSize(48, 48));

    anim = &static_data::anim;
  }


  Coin::~Coin()
  {

  }


  void Coin::update()
  {
    Item::update();
  }


  void Coin::draw(const Camera &camera)
  {
    if(anim != NULL)
      anim->draw(Object::getPosition() - point_to_vector(camera.getPosition()), etc::ALIGN_BY_CENTER, IntRect(0, 0, -1, -1));
  }

}

