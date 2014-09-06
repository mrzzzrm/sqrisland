#ifndef ENEMIES_PUNK_HPP
#define ENEMIES_PUNK_HPP

  #include "framework.hpp"

  #include "../BouncingEnemy.hpp"

  namespace enemy
  {
    class Punk: public BouncingEnemy
    {
       public:

        static void loadStaticData();
        static void clearStaticData();

         Punk(const util::Timer &timer, const tmx::base::Object &object);
         ~Punk();

         void update();
         void draw(const Camera &camera);


       private:

        /* Static data */
        struct static_data
        {
          static gfx::Animation bouncing_left;
          static gfx::Animation bouncing_right;
        };

        /* Data */
        gfx::Animation *anim;


    };
  }

#endif // ENEMIES_PUNK_HPP
