#ifndef DAMAGINGTILE_HPP
#define DAMAGINGTILE_HPP

  #include "framework.hpp"

  #include "./SpecialTile.hpp"


  class DamagingTile: public SpecialTile
  {
    public:

      DamagingTile(const util::Timer &timer, const tmx::base::Properties &properties, const tmx::base::TileLayer &tilelayer);
      ~DamagingTile();



      gfx::Animation &getAnimation();

      int getDamage() const;
      void setDamage(int damage);

      void update();
      void draw(const Camera &camera);


    private:

      /* Data */
      gfx::Animation animation;
      int damage;
  };


#endif // DAMAGINGTILE_HPP
