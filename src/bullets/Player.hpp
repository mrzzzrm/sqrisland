#ifndef BULLETS_PLAYER_HPP
#define BULLETS_PLAYER_HPP

  #include "framework.hpp"

  #include "../Bullet.hpp"

  namespace bullet
  {
    class Player: public Bullet
    {
      public:

        static void loadStaticData();
        static void clearStaticData();

        Player(const util::Timer &timer, int level);
        ~Player();

        int getLevel() const;

        void update();
        void draw(const Camera &camera);


      private:

        /* Static data */
        struct static_data
        {
          static gfx::Animation anim;
        };

        /* Data */
        int level;
        gfx::Animation *anim;;

    };
  }

#endif // BULLETS_PLAYER_HPP
