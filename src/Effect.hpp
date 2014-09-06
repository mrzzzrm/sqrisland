#ifndef EFFECT_HPP
#define EFFECT_HPP

  #include "framework.hpp"

  #include "./Object.hpp"
  #include "./Camera.hpp"


  enum EFFECT_TYPE
  {
    EFFECT_LOOPED,
    EFFECT_SINGLE_RUN
  };

  class Effect: public Object
  {
    public:

      Effect(const util::Timer &timer, EFFECT_TYPE type);
      ~Effect();

      EFFECT_TYPE getType() const;

      virtual void setBounds() = 0;


    private:

      /* Data */
      EFFECT_TYPE type;

  };

#endif // EFFECT_HPP
