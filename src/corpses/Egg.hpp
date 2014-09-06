#ifndef CORPSES_EGG_HPP
#define CORPSES_EGG_HPP

  #include "framework.hpp"

  #include "../Corpse.hpp"

  namespace corpse
  {
    class Egg: public Corpse
    {
      public:

        static void loadStaticData();
        static void clearStaticData();

        Egg(const util::Timer &timer, const Enemy &enemy, ENEMY_DEATH_TYPE death_type);
        ~Egg();


        void update();
        void draw(const Camera &camera);

      private:

        /* Static data */
        struct static_data
        {
          static gfx::Animation rolling_left;
          static gfx::Animation rolling_right;
          static gfx::Animation squished_left;
          static gfx::Animation squished_right;
        };

        /* Data */
        gfx::Animation *anim;
        double movement;
    };
  }

#endif // CORPSES_EGG_HPP
