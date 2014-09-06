#include "./Player.hpp"

using namespace std;
using namespace geo;
using namespace etc;
using namespace gfx;
using namespace util;
using namespace error;


namespace bullet
{
  Animation Player::static_data::anim;


  void Player::loadStaticData()
  {
    try
    {
      static_data::anim.load("dat/player/bullet.ani");
    }
    catch(Exception &exception)
    {
      throw Exception() << "Error loading static data of bullet::Player: " << exception;
    }
  }

  void Player::clearStaticData()
  {
    static_data::anim.clear();
  }

  Player::Player(const util::Timer &timer, int level):
    Bullet(timer, BULLET_PLAYER)
  {
    this->level = level;

    RectMap::setSize(IntSize(24, 24));
    RectMap::addRect(IntRect(0, 0, 24, 24));

    Object::setAlignment(ALIGN_BY_CENTER);
    Object::getGravity().setActive(true);

    Object::setBoundsSize(IntSize(24, 24));

    anim = &static_data::anim;
  }

  Player::~Player()
  {

  }

  int Player::getLevel() const
  {
    return level;
  }

  void Player::update()
  {
    Bullet::update();

    if(Object::isGroundtouch() && level == 2)
    {
      Object::getGravity().setValue(-300);
    }
  }

  void Player::draw(const Camera &camera)
  {
    if(anim != NULL)
      anim->draw(Object::getPosition() - point_to_vector(camera.getPosition()), etc::ALIGN_BY_CENTER, IntRect(0, 0, -1, -1));
  }
}

