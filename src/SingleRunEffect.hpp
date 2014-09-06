#ifndef SINGLERUNEFFECT_HPP
#define SINGLERUNEFFECT_HPP

  #include "framework.hpp"

  #include "./Effect.hpp"


  class SingleRunEffect: public gfx::Animation, public Effect
  {
    public:

      SingleRunEffect(const util::Timer &timer);
      ~SingleRunEffect();


      bool hasRan() const;

      void setBounds();

      void draw(const Camera &camera);


    private:

      /* Data */
  };


#endif // SINGLERUNEFFECT_HPP
