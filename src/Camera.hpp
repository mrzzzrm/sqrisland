#ifndef CAMERA_HPP
#define CAMERA_HPP

  #include "framework.hpp"



  class Camera
  {
    public:

      Camera(const util::Timer &timer);
      ~Camera();


      /* Clear */
      void clear();


      /* Position */
      const geo::FloatPoint getPosition() const;


      /* Fixed Position */
      const geo::FloatPoint &getFixedPosition() const;
      void setFixedPosition(const geo::FloatPoint &fixed_position);


      /* Offset */
      const geo::FloatVector &getOffset() const;
      void setOffset(const geo::FloatVector &offset);


      /* Size */
      const geo::IntSize &getViewSize() const;
      void setViewSize(const geo::IntSize &size);
      const geo::IntSize &getMapSize() const;
      void setMapSize(const geo::IntSize &size);


      /* Speed */
      const double &getSpeed() const;
      void setSpeed(const double &speed);


      /* Move */
      void move(const geo::IntPoint &target);
      void warp(const geo::IntPoint &target);



    private:

      /* Data */
      const util::Timer &timer;

      geo::FloatPoint fixed_position;
      geo::FloatVector offset;

      geo::IntSize view_size;
      geo::IntSize map_size;

      double speed;
  };

#endif
