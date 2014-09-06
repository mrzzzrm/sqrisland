#ifndef SCENE_GAMEFINISHED_HPP
#define SCENE_GAMEFINISHED_HPP

  #include "framework.hpp"

  #include "../Scene.hpp"

  namespace scene
  {
    class GameFinished: public Scene
    {
      public:

        static void loadStaticData();
        static void clearStaticData();

        GameFinished();
        ~GameFinished();

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

#endif // SCENE_GAMEFINISHED_HPP
