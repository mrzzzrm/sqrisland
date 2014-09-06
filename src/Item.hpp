#ifndef ITEM_HPP
#define ITEM_HPP

  #include "framework.hpp"

  #include "./Object.hpp"



  enum ITEM_TYPE
  {
    ITEM_NONE,
    ITEM_COIN,
    ITEM_UPGRADER,
    ITEM_LIFE
  };

  class Item: public Object
  {
    public:

      Item(const util::Timer &timer, ITEM_TYPE type, const tmx::base::Object &object);
      Item(const util::Timer &timer, ITEM_TYPE type);
      ~Item();


      ITEM_TYPE getType() const;

      void update();


    private:

      /* Data */
      ITEM_TYPE type;
  };


  ITEM_TYPE item_string_to_type(const std::string &string);


#endif // ITEM_HPP
