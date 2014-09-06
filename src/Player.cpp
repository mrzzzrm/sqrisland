#include "./Player.hpp"

using namespace std;
using namespace geo;
using namespace gfx;
using namespace etc;
using namespace util;
using namespace error;
using namespace input;
using namespace audio;


vector<Animation> Player::static_data::stand_left(3);
vector<Animation> Player::static_data::stand_right(3);
vector<Animation> Player::static_data::move_left(3);
vector<Animation> Player::static_data::move_right(3);
vector<Animation> Player::static_data::jump_left(3);
vector<Animation> Player::static_data::jump_right(3);
Sound Player::static_data::jump;


void Player::loadStaticData()
{
  try
  {
    for(int n = 0; n < 3; n++)
    {
      static_data::stand_left[n].load(string("dat/player/") + integer_to_string(n) + string("/stand_left.ani"));
      static_data::stand_right[n].load(string("dat/player/") + integer_to_string(n) + string("/stand_right.ani"));
      static_data::move_right[n].load(string("dat/player/") + integer_to_string(n) + string("/move_right.ani"));
      static_data::move_left[n].load(string("dat/player/") + integer_to_string(n) + string("/move_left.ani"));
      static_data::jump_left[n].load(string("dat/player/") + integer_to_string(n) + string("/jump_left.ani"));
      static_data::jump_right[n].load(string("dat/player/") + integer_to_string(n) + string("/jump_right.ani"));
      static_data::jump.load("dat/audio/sounds/jump.wav");
    }
  }
  catch(Exception &exception)
  {
    throw Exception() << "Error loading static data of Player: " << exception;
  }
}


void Player::clearStaticData()
{
  for(int n = 0; n < 3; n++)
  {
    static_data::stand_left[n].clear();
    static_data::stand_right[n].clear();
    static_data::move_right[n].clear();
    static_data::move_left[n].clear();
    static_data::jump_left[n].clear();
    static_data::jump_right[n].clear();
  }
  static_data::jump.clear();
}


Player::Player(const util::Timer &timer):
  Object(timer, OBJECT_PLAYER),
  jump(timer)
{
  RectMap::setSize(IntSize(96,96));
  RectMap::addRect(IntRect(13, 18, 73, 78));

  Object::setAlignment(ALIGN_BY_CENTER);
  Object::getGravity().setActive(true);

  lifecount = 0;
  coincount = 0;
  level = 0;

  speed = 0;
  jump_force = 0;
  spring_jump_max_delay = 0;

  topcoll = false;

  state = PLAYER_STAND_RIGHT;

  flickering = false;
  flickering_since = 0;
  flickering_framecount = 0;

  sig.dir               = 0;
  sig.key_jump.state    = SIG_NOT;
  sig.key_jump.dur      = 0;
  sig.key_jump.since    = 0;
  sig.spring_jump.state = SIG_NOT;
  sig.spring_jump.dur   = 0;
  sig.spring_jump.since = 0;
  sig.break_jump        = false;

  anim = &Player::static_data::move_right;
}


Player::~Player()
{

}


Jump &Player::getJump()
{
  return jump;
}


void Player::setSpeed(const double &speed)
{
  this->speed = speed;
}


void Player::setJumpForce(const double &force)
{
  jump_force = force;
}


void Player::setSpringJumpMaxDelay(const time_t &delay)
{
  spring_jump_max_delay = delay;
}


PLAYER_STATE Player::getState() const
{
  return (PLAYER_STATE)state;
}


void Player::setState(PLAYER_STATE state)
{
  this->state = state;
}


void Player::sigMove(s8 dir)
{
  sig.dir = dir;
}


void Player::sigStopMoving()
{
  sig.dir = 0;
}


void Player::sigJump(JUMP_TYPE type, time_t dur)
{
  if(type == JUMP_BY_KEY)
  {
    sig.key_jump.state = SIG_NEW;
    sig.key_jump.dur   = dur;
    sig.key_jump.since = Object::getTimer().getTicks();
  }
  else
  {
    sig.spring_jump.state = SIG_NEW;
    sig.spring_jump.dur   = dur;
    sig.spring_jump.since = Object::getTimer().getTicks();
  }
}

void Player::sigBreakJump()
{
  sig.break_jump = true;
}

void Player::sigStopAll()
{
  sigStopMoving();
  sigBreakJump();
}

void Player::sigDamaged()
{
  level--;
  if(level < 0)
    sigKilled();

  flickering = true;
  flickering_since = Object::getTimer().getTicks();
}

void Player::sigKilled()
{
  sigStopMoving();
  sigBreakJump();
  getGravity().setValue(-750);
  lifecount--;
}

void Player::sigRespawn()
{
  Object::getGravity().flush();

  state = PLAYER_STAND_RIGHT;
  level = 0;
}

void Player::collectItem(ITEM_TYPE type)
{
  switch(type)
  {
    case ITEM_NONE: break;

    case ITEM_COIN:
      coincount++;
      if(coincount >= 100)
      {
        coincount = 0;
        lifecount++;
      }
    break;

    case ITEM_UPGRADER:
      if(level < 2)
      {
        level++;
      }
    break;

    case ITEM_LIFE:
      lifecount++;
    break;
  }
}

int Player::getLevel() const
{
  return level;
}

void Player::setLevel(int level)
{
  this->level = level;
}

int Player::getCoincount() const
{
  return coincount;
}

void Player::setCoincount(int coincount)
{
  this->coincount = coincount;
}

int Player::getLifecount() const
{
  return lifecount;
}

void Player::setLifecount(int lifecount)
{
  this->lifecount = lifecount;
}

bool Player::isDead() const
{
  return lifecount < 0;
}

bool Player::isFlickering() const
{
  return flickering;
}

bool Player::isTopcoll() const
{
  return topcoll;
}

void Player::setTopcoll(bool is)
{
  topcoll = is;
}

void Player::update()
{
  Object::update();

  if(topcoll)
    getGravity().flush();

  /**
  	TEST
  **/
  if(sig.dir != 0)
    Object::getMovementBuffer().pushVector(FloatVector(Object::getTimer().getPerSecond(speed) * sig.dir, 0));

  if(sig.dir != 0)
  {
    state = (PLAYER_STATE)(state & ~PLAYER_DIRECTION_BITS);
    state = (PLAYER_STATE)(state | (sig.dir > 0 ? PLAYER_RIGHT:PLAYER_LEFT));
  }

  if(!Object::isGroundtouch() && !(state & PLAYER_JUMP))
  {
    state = (PLAYER_STATE)(state & ~PLAYER_ACTION_BITS);
    state = (PLAYER_STATE)(state | PLAYER_FALL);
  }

  switch(state)
  {
    case PLAYER_STAND_LEFT:  anim = &Player::static_data::stand_left;  break;
    case PLAYER_STAND_RIGHT: anim = &Player::static_data::stand_right; break;
    case PLAYER_MOVE_LEFT:   anim = &Player::static_data::move_left;   break;
    case PLAYER_MOVE_RIGHT:  anim = &Player::static_data::move_right;  break;
    case PLAYER_JUMP_LEFT:   anim = &Player::static_data::jump_left;   break;
    case PLAYER_JUMP_RIGHT:  anim = &Player::static_data::jump_right;  break;
    case PLAYER_FALL_LEFT:   anim = &Player::static_data::jump_left;   break;
    case PLAYER_FALL_RIGHT:  anim = &Player::static_data::jump_right;  break;

    default:break;
  }
  /**
  	/TEST
  **/


  if(sig.key_jump.state == SIG_NEW && Object::isGroundtouch())
  {
    SoundPlayer::play(static_data::jump);
    jump.start(sig.key_jump.dur);
    jump.setBreakable(true);
    sig.key_jump.state = SIG_NOT;
  }

  if(sig.key_jump.state != SIG_NOT && sig.spring_jump.state != SIG_NOT)
  {
    SoundPlayer::play(static_data::jump);
    jump.start(sig.key_jump.dur);
    jump.setBreakable(true);
    sig.key_jump.state = SIG_NOT;
    sig.spring_jump.state = SIG_NOT;
  }
  else if(sig.spring_jump.state == SIG_NEW)
  {
    SoundPlayer::play(static_data::jump);
     getGravity().setValue(-500);
//    jump.start(sig.spring_jump.dur);
//    jump.setBreakable(false);
    sig.spring_jump.state = SIG_PENDING;
  }

  jump.update();
  if(jump.is())
  {
    if(topcoll)
      jump.end();

    if(jump.hasLeftGround() && Object::isGroundtouch())
      jump.end();

    if(!jump.hasLeftGround() && !Object::isGroundtouch())
      jump.setLeftGround(true);

    if(sig.break_jump && jump.isBreakable())
    {
      getGravity().setValue(-jump_force * jump.getRelativeForce()/2);
      jump.end();
    }


    state = (PLAYER_STATE)(state & ~PLAYER_ACTION_BITS);
    if(!jump.is())
    {
      state = (PLAYER_STATE)(state | PLAYER_FALL);
    }
    else
    {
      Object::getMovementBuffer().pushVector(FloatVector(0, -Object::getTimer().getPerSecond(jump_force * jump.getRelativeForce())));
      state = (PLAYER_STATE)(state | PLAYER_JUMP);
    }
  }


  if(jump.is())
    Object::getGravity().setActive(false);
  else
    Object::getGravity().setActive(true);

  if(Object::isGroundtouch())
  {
    state = (PLAYER_STATE)(state & ~PLAYER_ACTION_BITS);
    if(sig.dir != 0)
      state = (PLAYER_STATE)(state | PLAYER_MOVE);
    else
      state = (PLAYER_STATE)(state | PLAYER_STAND);
  }


 if(Object::getTimer().getTicks() - sig.key_jump.since > spring_jump_max_delay)
    sig.key_jump.state = SIG_NOT;
  else if(sig.key_jump.state != SIG_NOT)
    sig.key_jump.state = SIG_PENDING;

  if(Object::getTimer().getTicks() - sig.spring_jump.since > spring_jump_max_delay)
    sig.spring_jump.state = SIG_NOT;
  else if(sig.spring_jump.state != SIG_NOT)
    sig.spring_jump.state = SIG_PENDING;

  sig.break_jump = false;


  /* Flickering */
  if(flickering)
  {
    if(Object::getTimer().getTicks() - flickering_since > 750)
      flickering = false;
    else
      flickering_framecount++;
  }

}


void Player::draw(const Camera &camera)
{
  if(anim != NULL)
  {
    if(!flickering || flickering_framecount%10 > 5)
      (*anim)[level].draw(Object::getPosition() - point_to_vector(camera.getPosition()), etc::ALIGN_BY_CENTER, IntRect(0, 0, -1, -1));
  }
}

