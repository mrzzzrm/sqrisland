#ifndef UNIT_HPP
#define UNIT_HPP

  #include "framework.hpp"

  #include "./Object.hpp"


  enum UNIT_MOVEMENT_TYPE
  {
    UNIT_MOVEMENT_NONE,
    UNIT_MOVEMENT_AXIS_PATROL,
    UNIT_MOVEMENT_AXIS_CONST,
    UNIT_MOVEMENT_BORDER_PATROL
  };

  enum UNIT_MOVE_DIRECTION
  {
    UNIT_MOVE_NOT,
    UNIT_MOVE_LEFT,
    UNIT_MOVE_RIGHT,
    UNIT_MOVE_UP,
    UNIT_MOVE_DOWN
  };


  class Unit: public Object
  {
    public:

      Unit(const util::Timer &timer, OBJECT_TYPE type, const tmx::base::Object &object);
      ~Unit();


      /* Activity */
      bool isActiveInRect(const geo::IntRect &rect) const;


      /* Speed */
      const double &getSpeed() const;
      void setSpeed(const double &speed);


      /* Movement */
      UNIT_MOVEMENT_TYPE getMovementType() const;
      UNIT_MOVE_DIRECTION getMoveDirection() const;
      s8 getXMoveDirection() const;
      s8 getYMoveDirection() const;
      void setAxisPatrol(AXIS axis, int m1, int m2, int target);
      void setAxisConst(AXIS axis, s8 dir);
      void setBorderPatrol(AXIS axis, s8 dir);


      /* Boundsrect */
      const geo::IntRect &getBodyrect() const;
      void setBodyrect(const geo::IntRect &bodyrect);


      /* Update */
      void update();


    private:

      /* Data */
      UNIT_MOVEMENT_TYPE movement_type;
      UNIT_MOVE_DIRECTION move_direction;

      double speed;

      struct
      {
        AXIS axis;
        int m1, m2;
        int target;
      }axis_patrol;
      struct
      {
        AXIS axis;
        s8 dir;
      }axis_const;
      struct
      {
        AXIS axis;
        s8 dir;
      }border_patrol;

      geo::IntRect bodyrect;


      /* Internals */
      void AxisPatrol();
      void AxisConst();
      void BorderPatrol();



  };

#endif // UNIT_HPP
