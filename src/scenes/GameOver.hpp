#ifndef SCENE_GAMEOVER_HPP
#define SCENE_GAMEOVER_HPP

  #include "framework.hpp"

  #include "../Scene.hpp"

  namespace scene
  {
    class GameOver: public Scene
    {
      public:

        static void loadStaticData();
        static void clearStaticData();

        GameOver();
        ~GameOver();

        bool isOver();

        void update();
        void draw();


      private:

        /* static data */
        struct static_data
        {
          static gfx::Animation background;
        };

        /* Data */
        util::Timer timer;
        gfx::ColorFading fading;
        time_t start;
        bool over;
    };
  }

#endif // SCENE_GAMEOVER_HPP
