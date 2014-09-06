#ifndef LOOPEDEFFECT_HPP
#define LOOPEDEFFECT_HPP

  #include "framework.hpp"

  #include "./Effect.hpp"


  class LoopedEffect: public gfx::Animation, public Effect
  {
    public:

      LoopedEffect(const util::Timer &timer);
      ~LoopedEffect();


      void setBounds();

      void draw(const Camera &camera);


    private:

      /* Data */
  };

#endif // LOOPEDEFFECT_HPP
