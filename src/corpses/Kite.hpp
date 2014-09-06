#ifndef CORPSES_KITE_HPP
#define CORPSES_KITE_HPP

  #include "framework.hpp"

  #include "../Corpse.hpp"

  namespace corpse
  {
    class Kite: public Corpse
    {
      public:

        static void loadStaticData();
        static void clearStaticData();

        Kite(const util::Timer &timer, const Enemy &enemy, ENEMY_DEATH_TYPE death_type);
        ~Kite();


        void update();
        void draw(const Camera &camera);

      private:

        /* Static data */
        struct static_data
        {
          static gfx::Animation falling;
        };

        /* Data */
        gfx::Animation *anim;
        double movement;
    };
  }

#endif // CORPSES_KITE_HPP
