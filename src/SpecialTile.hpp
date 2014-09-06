#ifndef SPECIALTILE_HPP
#define SPECIALTILE_HPP

  #include "framework.hpp"

  #include "./Camera.hpp"
  #include "./Object.hpp"


  enum SPECIAL_TILE_TYPE
  {
    SPECIAL_TILE_CONTAINER,
    SPECIAL_TILE_FRAGILE,
    SPECIAL_TILE_DAMAGING
  };


  class SpecialTile: public collision::RectMap
  {
    public:

      SpecialTile(const util::Timer &timer, SPECIAL_TILE_TYPE type, const tmx::base::Properties &properties, const tmx::base::TileLayer &tilelayer);
      virtual ~SpecialTile();


      const util::Timer &getTimer() const;
      SPECIAL_TILE_TYPE getType() const;

      int getPhase() const;
      virtual void nextPhase();

      const geo::IntPoint &getPosition() const;
      void setPosition(const geo::IntPoint &position);

      bool isCollision(const Object &object) const;

      virtual void update() = 0;
      virtual void draw(const Camera &camera) = 0;


    private:

      /* Data */
      const util::Timer &timer;
      SPECIAL_TILE_TYPE type;
      int phase;
      geo::IntPoint position;
  };


  SPECIAL_TILE_TYPE special_tile_string_to_type(const std::string &string);

#endif // SPECIALTILE_HPP
