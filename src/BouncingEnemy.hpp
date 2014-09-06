#ifndef BOUNCINGENEMY_HPP
#define BOUNCINGENEMY_HPP

  #include "framework.hpp"

  #include "./Enemy.hpp"
  #include "./Jump.hpp"

  class BouncingEnemy: public Enemy
  {
    public:

      BouncingEnemy(const util::Timer &timer, ENEMY_TYPE type, const tmx::base::Object &object);
      ~BouncingEnemy();


      const double &getBounceForce() const;
      void setBounceForce(const double &bounce_force);

      void update();


    private:

      /* Data */
      double bounce_force;


  };

#endif // BOUNCINGENEMY_HPP
