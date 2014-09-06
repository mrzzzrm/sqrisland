#ifndef ENEMIES_EGG_HPP
#define ENEMIES_EGG_HPP

  #include "framework.hpp"

  #include "../Enemy.hpp"

  namespace enemy
  {
    class Egg: public Enemy
    {
      public:

        static void loadStaticData();
        static void clearStaticData();


        Egg(const util::Timer &timer, const tmx::base::Object &object);
        ~Egg();

        void update();
        void draw(const Camera &camera);


      private:

        /* Static data */
        struct static_data
        {
          static gfx::Animation rolling_left;
          static gfx::Animation rolling_right;
        };



        /* Data */
        gfx::Animation *anim;
    };
  }

#endif // ENEMIES_EGG_HPP
