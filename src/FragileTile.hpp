#ifndef FRAGILETILE_HPP
#define FRAGILETILE_HPP

  #include "framework.hpp"

  #include "./SpecialTile.hpp"


  class FragileTile: public SpecialTile
  {
    public:

      FragileTile(const util::Timer &timer, const tmx::base::Properties &properties, const tmx::base::TileLayer &tilelayer);
      ~FragileTile();


      void nextPhase();

      gfx::Animation &getAnimation(int index);

      void update();
      void draw(const Camera &camera);


    private:

      /* Data */
      gfx::Animation animation[2];
  };


#endif // FRAGILETILE_HPP
