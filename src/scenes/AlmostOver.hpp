#ifndef SCENE_ALMOSTOVER_HPP
#define SCENE_ALMOSTOVER_HPP


  #include "framework.hpp"

  #include "../Scene.hpp"

  namespace scene
  {
    class AlmostOver: public Scene
    {
      public:

        static void loadStaticData();
        static void clearStaticData();

        AlmostOver();
        ~AlmostOver();

        bool isOver();

        void update();
        void draw();


      private:

    };
  }


#endif // SCENE_ALMOSTOVER_HPP
