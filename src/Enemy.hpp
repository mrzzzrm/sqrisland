#ifndef ENEMY_HPP
#define ENEMY_HPP

  #include "framework.hpp"

  #include "./Unit.hpp"


  enum ENEMY_TYPE
  {
    ENEMY_EGG,
    ENEMY_KITE,
    ENEMY_PUNK,
    ENEMY_TRI
  };


  enum ENEMY_PLAYER_COLLISION_TYPE
  {
    ENEMY_PLAYER_COLLISION_NONE,
    ENEMY_PLAYER_COLLISION_ENEMY_DAMAGED,
    ENEMY_PLAYER_COLLISION_PLAYER_DAMAGED
  };


  class Player;

  class Enemy: public Unit
  {
    public:

      Enemy(const util::Timer &timer, ENEMY_TYPE type, const tmx::base::Object &object);
      ~Enemy();


      /* Type */
      ENEMY_TYPE getType() const;


      /* HP */
      int getHP() const;
      void setHP(int hp);
      void decHP(int hp);


      /* Woundable rects */
      void setRectWoundable(int i);


      /* Enemy<->Player */
      ENEMY_PLAYER_COLLISION_TYPE getCollisionWithPlayer(const Player &player) const;


      /* Update */
      void update();



    private:

      /* Data */
      int hp;
      ENEMY_TYPE type;
      std::list<int> woundable_rects;

  };

#endif // ENEMY_HPP
