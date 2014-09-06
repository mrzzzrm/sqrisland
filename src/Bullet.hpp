#ifndef BULLET_HPP
#define BULLET_HPP

  #include "framework.hpp"

  #include "./Object.hpp"


  enum BULLET_TYPE
  {
    BULLET_PLAYER
  };


  class Bullet: public Object
  {
    public:

      Bullet(const util::Timer &timer, BULLET_TYPE type);
      ~Bullet();


      /* Type */
      BULLET_TYPE getType() const;


      /* Movement */
      util::Movement &getMovement();
      void setMovement(const util::Movement &movement);


      /* Update */
      void update();


    private:

      /* Data */
      BULLET_TYPE type;
      util::Movement movement;
  };

#endif // BULLET_HPP
