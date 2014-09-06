#ifndef PLATFORM_HPP
#define PLATFORM_HPP

  #include "framework.hpp"

  #include "./Object.hpp"
  #include "./Waypoint.hpp"



  class Platform: public Object
  {
    public:

      Platform(const util::Timer &timer, tmx::base::Map &map, tmx::base::TileLayer &tilelayer, tmx::base::Object &object);
      ~Platform();

      bool isActiveInRect(const geo::IntRect &rect) const;

      void update();
      void draw(const Camera &camera);


    private:

      /* Data */
      std::list<std::pair<gfx::ImageClipping, geo::IntVector> > tiles;
      std::list<Waypoint*> waypoints;
      std::list<Waypoint*>::iterator waypoint_iterator;
      struct
      {
        bool is;
        int since;
      }waiting;


      /* Internals */
      void LoadWaypoints(tmx::base::Object &object);
      void LoadRects(tmx::base::Object &object);
      void LoadImage(tmx::base::Map &map, tmx::base::TileLayer &tilelayer, tmx::base::Object &object);
  };

#endif
