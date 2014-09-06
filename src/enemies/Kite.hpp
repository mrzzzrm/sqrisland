#ifndef ENEMIES_KITE_HPP
#define ENEMIES_KITE_HPP

  #include "framework.hpp"

  #include "../Enemy.hpp"

  namespace enemy
  {
    class Kite: public Enemy
    {
      public:

        static void loadStaticData();
        static void clearStaticData();


        Kite(const util::Timer &timer, const tmx::base::Object &object);
        ~Kite();

        void update();
        void draw(const Camera &camera);


      private:

        /* Static data */
        struct static_data
        {
          static gfx::Animation flying_up;
          static gfx::Animation flying_down;
        };



        /* Data */
        gfx::Animation *anim;
    };
  }

#endif // ENEMIES_KITE_HPP
