#ifndef WAYPOINT_HPP
#define WAYPOINT_HPP

  #include "framework.hpp"


  class Waypoint: public geo::IntPoint
  {
    public:

      Waypoint(const geo::IntPoint &point, int wait, double speed);
      ~Waypoint();


      /* Clear */
      void clear();


      /* Wait */
      int getWait() const;
      void setWait(int wait);


      /* Speed */
      const double &getSpeed() const;
      void setSpeed(const double &speed);



    private:

      /* Data */
      int wait;
      double speed;

  };

#endif // WAYPOINT_HPP
