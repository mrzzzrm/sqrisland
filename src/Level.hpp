#ifndef LEVEL_HPP
#define LEVEL_HPP

  #include "framework.hpp"

  #include <map>

  #include "./Settings.hpp"
  #include "./Savestate.hpp"

  #include "./Camera.hpp"
  #include "./Object.hpp"
  #include "./Platform.hpp"
  #include "./Player.hpp"
  #include "./Background.hpp"
  #include "./Enemy.hpp"
  #include "./Corpse.hpp"
  #include "./Bullet.hpp"
  #include "./Effect.hpp"
  #include "./AnimationManager.hpp"
  #include "./TextManager.hpp"
  #include "./SpecialTileMap.hpp"
  #include "./Textbox.hpp"
  #include "./LevelIntro.hpp"
  #include "./LevelInterface.hpp"
  #include "./LevelMenu.hpp"
  #include "./LoopedEffect.hpp"
  #include "./SingleRunEffect.hpp"
  #include "./EventArea.hpp"

  #include "./bullets/bullets.hpp"
  #include "./corpses/corpses.hpp"
  #include "./enemies/enemies.hpp"
  #include "./items/items.hpp"
  #include "./events/events.hpp"



  enum BLACKLIST_MEMBER
  {
    BLACKLIST_NOT,
    BLACKLIST_DISABLED,
    BLACKLIST_REMOVE
  };

  enum OBJECT_CLASS
  {
    OBJECT_CLASS_PLATFORM_TRAVELER = OBJECT_PLAYER,
    OBJECT_CLASS_TOUCH_DETECTION   = OBJECT_PLAYER | OBJECT_ENEMY | OBJECT_ITEM | OBJECT_BULLET
  };

  enum INPUT_FOCUS
  {
    INPUT_FOCUS_NONE,
    INPUT_FOCUS_INTRO,
    INPUT_FOCUS_PLAYER,
    INPUT_FOCUS_TEXTBOX
  };

  enum LEVEL_STATE
  {
    LEVEL_STATE_INTRO,
    LEVEL_STATE_MAIN,
    LEVEL_STATE_MENU,
    LEVEL_STATE_DYING,
    LEVEL_STATE_RESPAWNING,
    LEVEL_STATE_NEXT_LEVEL,
    LEVEL_STATE_QUITTING,
    LEVEL_STATE_GAMEOVER
  };


  class Level: public collision::RasterMap, public tmx::render::Map, public util::Timer
  {
    public:

      static void loadStaticData();
      static void clearStaticData();


      Level();
      ~Level();


      void load(const Settings &settings, const util::Path &path);
      void startFromBeginning(const Settings &settings);
      void startFromSavestate(const Settings &settings);
      void start();

      static bool nextLevelExists();

      bool isOver() const;
      LEVEL_STATE getState() const;
      int getIndex() const;

      void update();
      void draw();


    private:


      /* Static Data */
      struct static_data
      {
        static audio::Sound click, coin, killed, one_up, gameover, hit;
      };


      /* Data */
      util::Path directory;

      Camera camera;
      Background background;
      Settings settings;
      AnimationManager animations;
      TextManager textmanager;
      Player player;
      SpecialTileMap special_tiles;
      Textbox textbox;
      LevelIntro intro;
      LevelInterface interface;
      LevelMenu menu;
      gfx::ColorFading fading;

      int index;
      LEVEL_STATE state;
      bool event_pending;
      bool objective_reached;
      time_t last_activity_check;
      int action_tilelayer_id;
      INPUT_FOCUS input_focus;
      geo::IntPoint reappearposition;



      struct
      {
        std::list<Platform*> platforms;
        std::list<Enemy*> enemies;
        std::list<Bullet*> bullets;
        std::list<Item*> items;
        std::list<Corpse*> corpses;

        std::vector<std::list<Effect*> > effectlayers;

        std::list<EventArea*> eventareas;
      }active, inactive;
      std::map<Object*, BLACKLIST_MEMBER> blacklist;


      /* Internals */
      void init();


      /* Load */
      void LoadPlatforms(const tmx::base::Objectgroup &objectgroup, tmx::base::TileLayer &tilelayer);
      void LoadEnemies(const tmx::base::Objectgroup &objectgroup);
      void LoadItems(const tmx::base::Objectgroup &objectgroup);
      void LoadEventAreas(const tmx::base::Objectgroup &objectgroup);
      void LoadCollisionMap(const tmx::base::TileLayer &tilelayer);
      void LoadSpecialTilesAndEffects(tmx::base::TileLayer &collision_tilelayer);
      void LoadIntro();
      void LoadPlayer();
      void LoadCamera();
      void LoadFading();


      /* Object */
      std::list<Object*> ActiveObjects(u32 flag);
      std::list<Object*> InactiveObjects(u32 flag);


      /* Updates */
      void UpdateStateIntro();
      void UpdateStateMain();
      void UpdateStateMenu();
      void UpdateStateDying();
      void UpdateStateRespawning();
      void UpdateStateNextLevel();
      void UpdateStateQuitting();
      void UpdateStateGameOver();

      void UpdateMainTasks();

      void UpdateTouchDetection();
      void UpdateGravity();
      void UpdateObjects();
      void UpdateCamera();
      void UpdateSpecialTiles();
      void UpdateSingleRunEffects();
      void UpdateActivity();

      void UpdateMainPlayerInput();


      /* Touch detection */
      void TouchDetection(Object &object);


      /* Movement */
      void MoveObject(Object &object);
      void MovePlayer();
      void MovePlatform(Platform &platform);
      void MoveEnemy(Enemy &enemy);
      void MoveBullet(Bullet &bullet);
      void MoveItem(Item &item);
      void MoveCorpse(Corpse &corpse);


      /* Collision (returning the object that that was hit or NULL)*/
      bool ObjectCollisionWithLevel(const Object &object);
      bool ObjectCollisionWithPlayer(const Object &object);
      Platform *ObjectCollisionWithPlatform(const Object &object);
      Enemy *ObjectCollisionWithEnemy(const Object &object);
      Bullet *ObjectCollisionWithBullet(const Object &object);
      Item *ObjectCollisionWithItem(const Object &object);


      /* Illegal overlap with another object/with the level */
      bool ObjectIllegalOverlap(const Object &object);
      bool PlayerIllegalOverlap();
      bool PlatformIllegalOverlap(const Platform &platform);
      bool EnemyIllegalOverlap(const Enemy &enemy);
      bool BulletIllegalOverlap(const Bullet &bullet);
      bool ItemIllegalOverlap(const Item &item);
      bool CorpseIllegalOverlap(const Corpse &corpse);


      /* Detect Collisions and perform all side effects.
         Return flag of the objects that need to be stopped from moving */
      bool PlayerBottomBorderCollision();
      OBJECT_TYPE PlayerEnemyCollision(Enemy &enemy);
      OBJECT_TYPE EnemyBulletCollision(Enemy &enemy, Bullet &bullet);
      OBJECT_TYPE PlayerItemCollision(Item &item);
      OBJECT_TYPE PlayerEventAreaCollision(EventArea &eventarea, bool manual);


      /* Removals */
      void RemoveEnemy(Enemy &enemy, ENEMY_DEATH_TYPE death_type);
      void RemoveBullet(Bullet &bullet);
      void RemoveItem(Item &item);


      /* Special inactive removal */
      void RemoveInactiveBullets();
      void RemoveInactiveCorpses();
      void RemoveInactiveLifeItems();


      /* Player  */
      void PlayerDamaged();
      void PlayerKilled();
      void PlayerCollectItem(ITEM_TYPE type);
      void PlayerSpecialTilesInteraction(std::list<SpecialTile*> &special_tiles, AXIS axis, s8 sgn);


      /* Updates */
      void CheckActivity();
      template<typename T> void CheckActivity(std::list<T*> &actives, std::list<T*> &inactives, const geo::IntRect &active_rect);
      void BlacklistCleanup();
      template<typename T> void BlacklistCleanup(std::list<T*> &actives, std::list<T*> &inactives, std::map<Object*, BLACKLIST_MEMBER>::iterator &i);
      void InputFocus();
      void EventCheck();


      /* Save */
      void Save();


  };


#endif
