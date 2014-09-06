#ifndef ITEM_LIFE_HPP
#define ITEM_LIFE_HPP

  #include "framework.hpp"

  #include "../Item.hpp"


  namespace item
  {
    class Life: public Item
    {
      public:

        static void loadStaticData();
        static void clearStaticData();

        Life(const util::Timer &timer, const tmx::base::Object &object);
        Life(const util::Timer &timer);
        ~Life();

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
        s8 dir;

        /* Internals */
        void initBases();
    };
  }
#endif // ITEM_LIFE_HPP
