#ifndef SAVESTATE_HPP
#define SAVESTATE_HPP

  #include "framework.hpp"


  class Savestate
  {
    public:

      Savestate();
      ~Savestate();

      void setLevel(int level);
      int getLevel() const;
      void setLifecount(int lifecount);
      int getLifecount() const;
      void setCoincount(int coincount);
      int getCoincount();
      void setPlayerlevel(int playerlevel);
      int getPlayerlevel() const;

      void load();
      void save();
      static bool exists();
      static void remove();


    private:

      /* Data */
      int level;
      int lifecount;
      int coincount;
      int playerlevel;
  };


#endif // SAVESTATE_HPP
