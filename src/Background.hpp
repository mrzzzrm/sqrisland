#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

  #include "framework.hpp"

  #include "./Camera.hpp"

  class Background
  {
    public:

      Background();
      ~Background();


      /* Load */
      void load(const tmx::base::Map &map);


      /* Draw */
      void draw(const Camera &camera);


    private:

      /* Data */
      struct Layer
      {
        gfx::Image image;
        double relx, rely;
      };
      std::vector<Layer*> layers;

  };

#endif // BACKGROUND_HPP
