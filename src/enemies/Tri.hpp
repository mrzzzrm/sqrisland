#ifndef ENEMIES_TRI_HPP
#define ENEMIES_TRI_HPP

  #include "framework.hpp"

  #include "../Enemy.hpp"

  namespace enemy
  {
    class Tri: public Enemy
    {
      public:

        static void loadStaticData();
        static void clearStaticData();


        Tri(const util::Timer &timer, const tmx::base::Object &object);
        ~Tri();

        void update();
        void draw(const Camera &camera);


      private:

        /* Static data */
        struct static_data
        {
          static gfx::Animation walking_left;
          static gfx::Animation walking_right;
        };

        /* Data */
        gfx::Animation *anim;
    };
  }

#endif // ENEMIES_TRI_HPP
