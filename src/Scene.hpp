#ifndef SCENE_HPP
#define SCENE_HPP

  #include "framework.hpp"

  enum SCENE_STATE
  {
    SCENE_STATE_IN,
    SCENE_STATE_MAIN,
    SCENE_STATE_OUT
  };

  class Scene
  {
    public:

      Scene();
      ~Scene();

      virtual bool isOver() = 0;

      SCENE_STATE getState() const;
      void setState(SCENE_STATE state);

      virtual void update() = 0;
      virtual void draw() = 0;

    private:

      SCENE_STATE state;
  };

#endif // SCENE_HPP
