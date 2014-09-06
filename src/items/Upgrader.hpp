#ifndef ITEM_UPGRADER_HPP
#define ITEM_UPGRADER_HPP

  #include "framework.hpp"

  #include "../Item.hpp"


  namespace item
  {
    class Upgrader: public Item
    {
      public:

        static void loadStaticData();
        static void clearStaticData();

        Upgrader(const util::Timer &timer, const tmx::base::Object &object);
        Upgrader(const util::Timer &timer);
        ~Upgrader();

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

#endif // ITEM_UPGRADER_HPP
