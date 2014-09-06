#ifndef TESTITEM_HPP
#define TESTITEM_HPP

  #include "framework.hpp"

  #include "../Item.hpp"

  namespace item
  {
    class Coin: public Item
    {
      public:

        static void loadStaticData();
        static void clearStaticData();

        Coin(const util::Timer &timer, const tmx::base::Object &object);
        ~Coin();

        void update();
        void draw(const Camera &camera);


      private:

        /* Static data */
        struct static_data
        {
          static gfx::Animation anim;
        };

        /* Data */
        gfx::Animation *anim;
    };
  }

#endif // TESTITEM_HPP
