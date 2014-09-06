#ifndef PLAYER_HPP
#define PLAYER_HPP

  #include "framework.hpp"

  #include "./Object.hpp"
  #include "./Item.hpp"
  #include "./Jump.hpp"


  enum PLAYER_STATE
  {
    PLAYER_LEFT         = 0x00000001,
    PLAYER_RIGHT        = 0x00000002,

    PLAYER_STAND        = 0x00000004,
    PLAYER_MOVE         = 0x00000008,
    PLAYER_JUMP         = 0x00000010,
    PLAYER_FALL         = 0x00000020,

    PLAYER_STAND_LEFT   = PLAYER_LEFT  | PLAYER_STAND,
    PLAYER_STAND_RIGHT  = PLAYER_RIGHT | PLAYER_STAND,
    PLAYER_MOVE_LEFT    = PLAYER_LEFT  | PLAYER_MOVE,
    PLAYER_MOVE_RIGHT   = PLAYER_RIGHT | PLAYER_MOVE,
    PLAYER_JUMP_LEFT    = PLAYER_LEFT  | PLAYER_JUMP,
    PLAYER_JUMP_RIGHT   = PLAYER_RIGHT | PLAYER_JUMP,
    PLAYER_FALL_LEFT    = PLAYER_LEFT  | PLAYER_FALL,
    PLAYER_FALL_RIGHT   = PLAYER_RIGHT | PLAYER_FALL,

    PLAYER_DIRECTION_BITS  = PLAYER_LEFT  | PLAYER_RIGHT,
    PLAYER_ACTION_BITS     = PLAYER_STAND | PLAYER_MOVE | PLAYER_JUMP | PLAYER_FALL
  };


  class Player: public Object
  {
    public:

      static void loadStaticData();
      static void clearStaticData();


      Player(const util::Timer &timer);
      ~Player();



      /* Jump */
      Jump &getJump();
      void setSpeed(const double &speed);
      void setJumpForce(const double &force);
      void setSpringJumpMaxDelay(const time_t &delay);


      /* State */
      PLAYER_STATE getState() const;
      void setState(PLAYER_STATE state);


      /* Signals */
      void sigMove(s8 dir);
      void sigStopMoving();
      void sigJump(JUMP_TYPE type, time_t dur);
      void sigBreakJump();
      void sigStopAll();
      void sigDamaged();
      void sigKilled();
      void sigRespawn();


      /* Items/Lives/Level */
      void collectItem(ITEM_TYPE type);
      int getLevel() const;
      void setLevel(int level);
      int getCoincount() const;
      void setCoincount(int coincount);
      int getLifecount() const;
      void setLifecount(int lifecount);
      bool isDead() const;
      bool isFlickering() const;



      /* Topcoll */
      bool isTopcoll() const;
      void setTopcoll(bool is);


      /* Update */
      void update();


      /* Draw */
      void draw(const Camera &camera);


    private:

      /* Static data */
      struct static_data
      {
        static std::vector<gfx::Animation> stand_left;
        static std::vector<gfx::Animation> stand_right;
        static std::vector<gfx::Animation> move_left;
        static std::vector<gfx::Animation> move_right;
        static std::vector<gfx::Animation> jump_left;
        static std::vector<gfx::Animation> jump_right;
        static audio::Sound jump;
      };


      /* Data */
      int lifecount;
      int coincount;
      int level;

      double speed;
      double jump_force;
      time_t spring_jump_max_delay;
      PLAYER_STATE state;

      bool topcoll;

      Jump jump;

      bool flickering;
      time_t flickering_since;
      int flickering_framecount;


      enum SIG_STATE
      {
        SIG_NEW,
        SIG_PENDING,
        SIG_NOT
      };
      struct
      {
        s8 dir;
        bool break_jump;
        struct
        {
          SIG_STATE state;
          time_t dur;
          time_t since;
        }key_jump, spring_jump;
      }sig;

      std::vector<gfx::Animation> *anim;
  };

#endif
