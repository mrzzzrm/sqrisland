#ifndef CONTAINERTILE_HPP
#define CONTAINERTILE_HPP

  #include "framework.hpp"

  #include "./SpecialTile.hpp"
  #include "./Item.hpp"


  class ContainerTile: public SpecialTile
  {
    public:

      ContainerTile(const util::Timer &timer, const tmx::base::Properties &properties, const tmx::base::TileLayer &tilelayer);
      ~ContainerTile();


      void nextPhase();

      gfx::Animation &getAnimation(int index);

      ITEM_TYPE getItemType() const;
      void setItemType(ITEM_TYPE item_type);

      void update();
      void draw(const Camera &camera);


    private:

      /* Data */
      gfx::Animation animation[3];
      ITEM_TYPE item_type;
  };


#endif // CONTAINERTILE_HPP
