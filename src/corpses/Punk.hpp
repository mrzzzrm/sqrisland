#ifndef CORPSES_PUNK_HPP
#define CORPSES_PUNK_HPP

  #include "framework.hpp"

  #include "../Corpse.hpp"

  namespace corpse
  {
    class Punk: public Corpse
    {
      public:

        static void loadStaticData();
        static void clearStaticData();

        Punk(const util::Timer &timer, const Enemy &enemy, ENEMY_DEATH_TYPE death_type);
        ~Punk();


        void update();
        void draw(const Camera &camera);

      private:

        /* Static data */
        struct static_data
        {
          static gfx::Animation falling_left;
          static gfx::Animation falling_right;
          static gfx::Animation squished_left;
          static gfx::Animation squished_right;
        };

        /* Data */
        gfx::Animation *anim;
        double movement;
    };
  }

#endif // CORPSES_PUNK_HPP
