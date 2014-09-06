#ifndef SPECIALTILEMAP_HPP
#define SPECIALTILEMAP_HPP

  #include "framework.hpp"

  #include "./Player.hpp"
  #include "./Camera.hpp"
  #include "./AnimationManager.hpp"

  #include "./ContainerTile.hpp"
  #include "./FragileTile.hpp"
  #include "./DamagingTile.hpp"


  class SpecialTileMap
  {
    public:

      SpecialTileMap(const util::Timer &timer, const tmx::base::Map &tmxmap, AnimationManager  &animations);
      ~SpecialTileMap();


      void load(tmx::base::TileLayer &tilelayer, std::map<int, tmx::base::Properties*> &tile_properties);

      bool isCollision(const Object &object) const;
      std::list<SpecialTile*> getCollisionWithPlayer(const Player &player);


      void update();
      void draw(const Camera &camera);


    private:

      const util::Timer &timer;
      const tmx::base::Map &tmxmap;
      AnimationManager  &animations;
      std::vector<std::vector<SpecialTile*> > map;


      /* Internals */
      SpecialTile *LoadContainerTile(int gid, const tmx::base::Properties &properties, const tmx::base::TileLayer &tilelayer);
      SpecialTile *LoadFragileTile(int gid, const tmx::base::Properties &properties, const tmx::base::TileLayer &tilelayer);
      SpecialTile *LoadDamagingTile(int gid, const tmx::base::Properties &properties, const tmx::base::TileLayer &tilelayer);

      void CollisionCheckArea(const Object &object, int &ftx, int &fty, int &ltx, int &lty) const;
  };

#endif // SPECIALTILEMAP_HPP
