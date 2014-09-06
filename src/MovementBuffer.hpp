#ifndef MOVEMENTBUFFER_HPP
#define MOVEMENTBUFFER_HPP

  #include "framework.hpp"



  enum AXIS
  {
    AXIS_X,
    AXIS_Y,
  };


  class MovementBuffer: public geo::FloatVector
  {
    public:

      MovementBuffer(const geo::FloatVector &vector = geo::IntVector(0, 0));
      MovementBuffer(const MovementBuffer &movementbuffer);
      ~MovementBuffer();


      /* Load */
      void load(const geo::FloatVector &vector);
      void load(const MovementBuffer &movementbuffer);


      /* Clear */
      void clear();


      /* Check axises */
      bool hasIntValue(AXIS axis) const;


      /* Push */
      void pushVector(const geo::FloatVector &vector);


      /* Pop/Push */
      s8 pop(AXIS axis);
      void push(AXIS axis, s8 dir);


      /* Clear integer value of axis  */
      void clearIntValue(AXIS axis);


    private:

      /* Data */
  };

#endif // MOVEMENTBUFFER_HPP
