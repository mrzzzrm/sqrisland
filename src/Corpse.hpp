#ifndef CORPSE_HPP
#define CORPSE_HPP

  #include "framework.hpp"

  #include "./Object.hpp"
  #include "./Enemy.hpp"


  enum ENEMY_DEATH_TYPE
  {
    ENEMY_DEATH_BY_JUMP,
    ENEMY_DEATH_BY_BULLET
  };

  class Corpse: public Object
  {
    public:

      Corpse(const util::Timer &timer, ENEMY_TYPE enemy_type, ENEMY_DEATH_TYPE death_type);
      ~Corpse();


      ENEMY_TYPE getEnemyType() const;
      ENEMY_DEATH_TYPE getDeathType() const;

      bool isToBeRemoved() const;

      void update();


    private:

      /* Data */
      ENEMY_TYPE enemy_type;
      ENEMY_DEATH_TYPE death_type;
      bool to_be_removed;
  };


#endif // CORPSE_HPP
