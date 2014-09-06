#ifndef OBJECT_HPP
#define OBJECT_HPP

  #include "framework.hpp"

  #include "./MovementBuffer.hpp"
  #include "./Camera.hpp"
  #include "./Gravity.hpp"


  enum OBJECT_TYPE
  {
    OBJECT_PLAYER       = 0x00000001,
    OBJECT_PLATFORM     = 0x00000002,
    OBJECT_ENEMY        = 0x00000004,
    OBJECT_BULLET       = 0x00000008,
    OBJECT_ITEM         = 0x00000010,
    OBJECT_EFFECT       = 0x00000020,
    OBJECT_EVENTAREA    = 0x00000040,
    OBJECT_CORPSE       = 0x00000080,
    OBJECT_ALL          = 0x0000FFFF
  };



  class Object: public collision::RectMap
  {
    public:

      Object(const util::Timer &timer, OBJECT_TYPE type, const geo::IntPoint &position = geo::IntPoint(0, 0));
      virtual ~Object();


      /* Timer */
      const util::Timer &getTimer() const;


      /* Position */
      const geo::IntPoint &getPosition() const;
      void setPosition(const geo::IntPoint &position);
      void move(AXIS axis, s8 dir);


      /* MovementBuffer */
      MovementBuffer &getMovementBuffer();


      /* Alignment */
      const etc::Alignment &getAlignment() const;
      void setAlignment(const etc::Alignment &alignment);


      /* Type */
      OBJECT_TYPE getType() const;


      /* Touch/Coll */
      bool isLefttouch() const;
      void setLefttouch(bool is);
      bool isRighttouch() const;
      void setRighttouch(bool is);
      bool isToptouch() const;
      void setToptouch(bool is);
      bool isGroundtouch() const;
      void setGroundtouch(bool is);


      /* Gravity */
      Gravity &getGravity();


      /* Activity / Bounds */
      void setBoundsSize(const geo::IntSize &size);
      bool isBoundsCollision(const Object &object) const;
      bool isBoundsCollision(const geo::IntRect &rect) const;
      virtual bool isActiveInRect(const geo::IntRect &rect) const;


      /* Update */
      virtual void update();
      virtual void draw(const Camera &camera) = 0;


      /* Collision detection */
      bool isCollision(const Object &object) const;


    private:

      /* Data */
      const util::Timer &timer;
      geo::IntPoint position;
      MovementBuffer movementbuffer;
      etc::Alignment alignment;
      OBJECT_TYPE type;
      bool lefttouch;
      bool righttouch;
      bool toptouch;
      bool groundtouch;
      Gravity gravity;

      struct
      {
        geo::IntSize size;
        geo::IntRect rect;
        int circle_radius;
        geo::IntVector circle_offset;
      }bounds;


      /* Internals */
      void updateBounds();


  };

#endif
