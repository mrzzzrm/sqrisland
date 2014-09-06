#ifndef GRAVITY_HPP
#define GRAVITY_HPP

  #include "framework.hpp"


  class Gravity
  {
    public:

      Gravity(const util::Timer &timer);
      ~Gravity();


      bool isActive() const;
      void setActive(bool is);

      const double &getValue() const;
      void setValue(const double &value);

      void limit(const double &max);

      void flush();

      void accelerate(const double &acc);


    private:

      /* Data */
      const util::Timer &timer;
      bool active;
      double value;
  };

#endif // GRAVITY_HPP
