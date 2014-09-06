#include "./Level.hpp"

using namespace std;
using namespace gfx;
using namespace audio;
using namespace etc;
using namespace geo;
using namespace util;
using namespace input;
using namespace collision;
using namespace error;
using namespace tmx::render;



Sound Level::static_data::click;
Sound Level::static_data::coin;
Sound Level::static_data::killed;
Sound Level::static_data::one_up;
Sound Level::static_data::gameover;
Sound Level::static_data::hit;


void Level::loadStaticData()
{
  static_data::click.load("dat/audio/sounds/click.wav");
  static_data::coin.load("dat/audio/sounds/coin.ogg");
  static_data::killed.load("dat/audio/sounds/killed.wav");
  static_data::one_up.load("dat/audio/sounds/one_up.wav");
  static_data::gameover.load("dat/audio/sounds/killed.wav");
  static_data::hit.load("dat/audio/sounds/killed.wav");
}


void Level::clearStaticData()
{
  static_data::click.clear();
  static_data::coin.clear();
  static_data::killed.clear();
  static_data::one_up.clear();
  static_data::hit.clear();
}


Level::Level():
  camera(*this),
  player(*this),
  special_tiles(*this, *this, animations),
  textbox(*this),
  intro(*this),
  interface(*this)
{
  init();
}

Level::~Level()
{

}


void Level::load(const Settings &settings, const Path &path)
{
  int collision_tilelayer_id;
  int platforms_objectgroup_id;
  int platforms_tilelayer_id;
  int enemies_objectgroup_id;
  int items_objectgroup_id;
  int eventareas_objectgroup_id;


  Map::load(path);
  directory = path.getDirectory();

  /* Settings */
  this->settings = settings;
  this->settings.multiplyTileBasedSpeeds(sqrt(Map::getTileWidth() * Map::getTileWidth() + Map::getTileHeight() * Map::getTileHeight()));

  /* Load layer ids */
  action_tilelayer_id = -1;
  collision_tilelayer_id = -1;
  platforms_objectgroup_id = -1;
  platforms_tilelayer_id = -1;
  enemies_objectgroup_id = -1;
  items_objectgroup_id = -1;
  eventareas_objectgroup_id = -1;

  for(int tl = 0; tl < Map::getTileLayerCount(); tl++)
  {
    tmx::base::TileLayer &tilelayer = Map::getTileLayer(tl);
    if(tilelayer.getProperties().hasValue("action"))    action_tilelayer_id = tl;
    if(tilelayer.getProperties().hasValue("collision")) collision_tilelayer_id = tl;
    if(tilelayer.getProperties().hasValue("platforms")) platforms_tilelayer_id = tl;
  }
  for(int og = 0; og < Map::getObjectgroupCount(); og++)
  {
    tmx::base::Objectgroup &objectgroup = Map::getObjectgroup(og);
    if(objectgroup.getProperties().hasValue("platforms")) platforms_objectgroup_id = og;
    if(objectgroup.getProperties().hasValue("enemies")) enemies_objectgroup_id = og;
    if(objectgroup.getProperties().hasValue("items")) items_objectgroup_id = og;
    if(objectgroup.getProperties().hasValue("eventareas")) eventareas_objectgroup_id = og;
  }

  if(action_tilelayer_id == -1)      throw Exception() << "No such tilelayer_id set: 'action'";
  if(collision_tilelayer_id == -1)   throw Exception() << "No such tilelayer_id set: 'collision'";
  if((platforms_tilelayer_id != -1) ^ (platforms_objectgroup_id != -1))   throw Exception() << "'platforms' property must be set either for a tilelayer and a objectgroup or for none";

  if(enemies_objectgroup_id == -1)    cout << "No enemies objectgroup defined" << endl;
  if(items_objectgroup_id == -1)      cout << "No items objectgroup defined" << endl;
  if(eventareas_objectgroup_id == -1) cout << "No eventareas objectgroup defined" << endl;

  if(platforms_objectgroup_id != -1)
    LoadPlatforms(Map::getObjectgroup(platforms_objectgroup_id), Map::getTileLayer(platforms_tilelayer_id));
  if(enemies_objectgroup_id != -1)
    LoadEnemies(Map::getObjectgroup(enemies_objectgroup_id));
  if(items_objectgroup_id != -1)
    LoadItems(Map::getObjectgroup(items_objectgroup_id));
  if(eventareas_objectgroup_id != -1)
    LoadEventAreas(Map::getObjectgroup(eventareas_objectgroup_id));

  LoadSpecialTilesAndEffects(Map::getTileLayer(collision_tilelayer_id));
  LoadCollisionMap(Map::getTileLayer(collision_tilelayer_id)); // Has to be done after platform generation and special tile loading!!!

  LoadIntro();
  LoadPlayer();
  LoadCamera();
  LoadFading();

  background.load(*this);
}

void Level::startFromBeginning(const Settings &settings)
{
  load(settings, "dat/levels/0.tmx");
  player.setLifecount(3);
  interface.setLifecount(3);
  index = 0;
}

void Level::startFromSavestate(const Settings &settings)
{
  Savestate savestate;

  savestate.load();

  load(settings, string("dat/levels/") + integer_to_string(savestate.getLevel()) + string(".tmx"));
  index = savestate.getLevel();

  player.setCoincount(savestate.getCoincount());
  player.setLifecount(savestate.getLifecount());
  player.setLevel(savestate.getPlayerlevel());

  interface.setCoincount(savestate.getCoincount());
  interface.setLifecount(savestate.getLifecount());
}

void Level::start()
{
  intro.start();
  Timer::update();

}

bool Level::nextLevelExists()
{
  Savestate savestate;
  savestate.load();

  return Path(string("dat/levels/") + integer_to_string(savestate.getLevel()) + string(".tmx")).exists();
}

bool Level::isOver() const
{
  return (state == LEVEL_STATE_NEXT_LEVEL ||
          state == LEVEL_STATE_QUITTING ||
          state == LEVEL_STATE_GAMEOVER) && fading.isDone();
}

LEVEL_STATE Level::getState() const
{
  return state;
}

int Level::getIndex() const
{
  return index;
}

void Level::update()
{
  Timer::update();
  interface.update();

  switch(state)
  {
    case LEVEL_STATE_INTRO:      UpdateStateIntro();       break;
    case LEVEL_STATE_MAIN:       UpdateStateMain();        break;
    case LEVEL_STATE_MENU:       UpdateStateMenu();        break;
    case LEVEL_STATE_DYING:      UpdateStateDying();       break;
    case LEVEL_STATE_RESPAWNING: UpdateStateRespawning();  break;
    case LEVEL_STATE_NEXT_LEVEL: UpdateStateNextLevel();   break;
    case LEVEL_STATE_QUITTING:   UpdateStateQuitting();    break;
    case LEVEL_STATE_GAMEOVER:   UpdateStateGameOver();    break;
  }
}


void Level::draw()
{
  list<Object*> objects;


  /* Background */
  background.draw(camera);

  /* Layers behind action */
  for(int tl = 0; tl <= action_tilelayer_id; tl++)
  {
    Map::drawTileLayer(tl, IntRect(0, 0, -1, -1), camera.getPosition());

    for(list<Effect*>::iterator i = active.effectlayers[tl].begin(); i != active.effectlayers[tl].end(); i++)
    {
      Effect *effect = *i;
      effect->draw(camera);
    }
  }
  special_tiles.draw(camera);


  /* Action */
  objects = ActiveObjects(OBJECT_ALL ^ OBJECT_PLAYER);
  for(list<Object*>::iterator i = objects.begin(); i != objects.end(); i++)
  {
    (*i)->draw(camera);
  }
  if(blacklist[&player] == BLACKLIST_NOT)
    player.draw(camera);



  /* Layer in front of action */
  for(int tl = action_tilelayer_id + 1; tl < Map::getTileLayerCount(); tl++)
  {
    Map::drawTileLayer(tl, IntRect(0, 0, -1, -1), camera.getPosition());

    for(list<Effect*>::iterator i = active.effectlayers[tl].begin(); i != active.effectlayers[tl].end(); i++)
    {
      Effect *effect = *i;
      effect->draw(camera);
    }
  }

  /* Textbox, if active */
  if(textbox.getState() != TEXTBOX_INACTIVE)
    textbox.draw();

  /* Interface */
  interface.draw();

  /* Intro, if active */
  if(intro.isActive())
    intro.draw();

  /* Menu */
  if(state == LEVEL_STATE_MENU || state == LEVEL_STATE_QUITTING)
    menu.draw();

  /* Fading during outro, respawning or dying sequence */
  if(state == LEVEL_STATE_NEXT_LEVEL ||
    state == LEVEL_STATE_QUITTING ||
    state == LEVEL_STATE_GAMEOVER ||
    state == LEVEL_STATE_RESPAWNING ||
    state == LEVEL_STATE_DYING)
    fading.draw();
}


void Level::init()
{
  event_pending = false;
  objective_reached = false;
  last_activity_check = 0;
  action_tilelayer_id = 0;
  input_focus = INPUT_FOCUS_INTRO;
  state = LEVEL_STATE_INTRO;
}


list<Object*> Level::ActiveObjects(u32 flag)
{
  list<Object*> res;

  if(flag & OBJECT_PLAYER)
  {
    if(blacklist[&player] == BLACKLIST_NOT)
      res.push_back(&player);
  }
  if(flag & OBJECT_PLATFORM)
  {
    for(list<Platform*>::iterator i = active.platforms.begin(); i != active.platforms.end(); i++)
    {
      if(blacklist[*i] == BLACKLIST_NOT)
        res.push_back(*i);
    }
  }
  if(flag & OBJECT_ENEMY)
  {
    for(list<Enemy*>::iterator i = active.enemies.begin(); i != active.enemies.end(); i++)
    {
      if(blacklist[*i] == BLACKLIST_NOT)
        res.push_back(*i);
    }
  }
  if(flag & OBJECT_BULLET)
  {
    for(list<Bullet*>::iterator i = active.bullets.begin(); i != active.bullets.end(); i++)
    {
      if(blacklist[*i] == BLACKLIST_NOT)
        res.push_back(*i);
    }
  }
  if(flag & OBJECT_ITEM)
  {
    for(list<Item*>::iterator i = active.items.begin(); i != active.items.end(); i++)
    {
      if(blacklist[*i] == BLACKLIST_NOT)
        res.push_back(*i);
    }
  }
  if(flag & OBJECT_CORPSE)
  {
    for(list<Corpse*>::iterator i = active.corpses.begin(); i != active.corpses.end(); i++)
    {
      if(blacklist[*i] == BLACKLIST_NOT)
        res.push_back(*i);
    }
  }
  if(flag & OBJECT_EVENTAREA)
  {
    for(list<EventArea*>::iterator i = active.eventareas.begin(); i != active.eventareas.end(); i++)
    {
      if(blacklist[*i] == BLACKLIST_NOT)
        res.push_back(*i);
    }
  }

  return res;
}


list<Object*> Level::InactiveObjects(u32 flag)
{
  list<Object*> res;

  if(flag & OBJECT_PLAYER)
  {
    if(blacklist[&player] == BLACKLIST_NOT)
      res.push_back(&player);
  }
  if(flag & OBJECT_PLATFORM)
  {
    for(list<Platform*>::iterator i = inactive.platforms.begin(); i != inactive.platforms.end(); i++)
    {
      if(blacklist[*i] == BLACKLIST_NOT)
        res.push_back(*i);
    }
  }
  if(flag & OBJECT_ENEMY)
  {
    for(list<Enemy*>::iterator i = inactive.enemies.begin(); i != inactive.enemies.end(); i++)
    {
      if(blacklist[*i] == BLACKLIST_NOT)
        res.push_back(*i);
    }
  }
  if(flag & OBJECT_BULLET)
  {
    for(list<Bullet*>::iterator i = inactive.bullets.begin(); i != inactive.bullets.end(); i++)
    {
      if(blacklist[*i] == BLACKLIST_NOT)
        res.push_back(*i);
    }
  }
  if(flag & OBJECT_ITEM)
  {
    for(list<Item*>::iterator i = inactive.items.begin(); i != inactive.items.end(); i++)
    {
      if(blacklist[*i] == BLACKLIST_NOT)
        res.push_back(*i);
    }
  }
  if(flag & OBJECT_EVENTAREA)
  {
    for(list<EventArea*>::iterator i = inactive.eventareas.begin(); i != inactive.eventareas.end(); i++)
    {
      if(blacklist[*i] == BLACKLIST_NOT)
        res.push_back(*i);
    }
  }

  return res;
}


void Level::TouchDetection(Object &object)
{
  object.move(AXIS_X, 1);
  object.setRighttouch(ObjectIllegalOverlap(object));
  object.move(AXIS_X, -1);

  object.move(AXIS_X, -1);
  object.setLefttouch(ObjectIllegalOverlap(object));
  object.move(AXIS_X, 1);

  object.move(AXIS_Y, 1);
  object.setGroundtouch(ObjectIllegalOverlap(object));
  object.move(AXIS_Y, -1);

  object.move(AXIS_Y, -1);
  object.setToptouch(ObjectIllegalOverlap(object));
  object.move(AXIS_Y, 1);
}

void Level::RemoveEnemy(Enemy &enemy, ENEMY_DEATH_TYPE death_type)
{
  blacklist[&enemy] = BLACKLIST_REMOVE;

  /* Generate Corpse */
  Corpse *corpse;

  switch(enemy.getType())
  {
    case ENEMY_EGG:
    {
      corpse = new corpse::Egg(*this, enemy, death_type);
    }
    break;
    case ENEMY_KITE:
    {
      corpse = new corpse::Kite(*this, enemy, death_type);
    }
    break;
    case ENEMY_PUNK:
    {
      corpse = new corpse::Punk(*this, enemy, death_type);
    }
    break;
    case ENEMY_TRI:
    {
      corpse = new corpse::Tri(*this, enemy, death_type);
    }
    break;
  }
  active.corpses.push_back(corpse);
}


void Level::RemoveBullet(Bullet &bullet)
{
  blacklist[&bullet] = BLACKLIST_REMOVE;
}


void Level::RemoveItem(Item &item)
{
  blacklist[&item] = BLACKLIST_REMOVE;
}


void Level::RemoveInactiveBullets()
{
  for(list<Bullet*>::iterator i = inactive.bullets.begin(); i != inactive.bullets.end(); i++)
  {
    Bullet *bullet = *i;
    blacklist[bullet] = BLACKLIST_REMOVE;
  }
}


void Level::RemoveInactiveCorpses()
{
  for(list<Corpse*>::iterator i = inactive.corpses.begin(); i != inactive.corpses.end(); i++)
  {
    Corpse *corpse = *i;
    blacklist[corpse] = BLACKLIST_REMOVE;
  }
}


void Level::RemoveInactiveLifeItems()
{
  for(list<Item*>::iterator i = inactive.items.begin(); i != inactive.items.end(); i++)
  {
    Item *item = *i;
    if(item->getType() == ITEM_LIFE)
      blacklist[item] = BLACKLIST_REMOVE;
  }
}


void Level::PlayerDamaged()
{
  if(player.getLevel() == 0)
  {
    PlayerKilled();
  }
  else
  {
    SoundPlayer::play(static_data::hit);
    player.sigDamaged();
    player.sigJump(JUMP_BY_SPRING, settings.getPlayerDamagedJumpDur());
  }
}


void Level::PlayerKilled()
{
  if(state == LEVEL_STATE_DYING || state == LEVEL_STATE_GAMEOVER)
    return;

  input_focus = INPUT_FOCUS_NONE;
  fading.forceStart(FADE_IN);
  player.sigKilled();

  if(player.getLifecount() < 0)
  {
    state = LEVEL_STATE_GAMEOVER;
    SoundPlayer::play(static_data::gameover);
    Savestate::remove();
  }
  else
  {
    state = LEVEL_STATE_DYING;
    SoundPlayer::play(static_data::killed);
  }
}


void Level::PlayerCollectItem(ITEM_TYPE type)
{
  player.collectItem(type);

  switch(type)
  {
    case ITEM_NONE: break;

    case ITEM_COIN:
      SoundPlayer::play(static_data::coin);
      interface.sigCoin();
    break;

    case ITEM_LIFE:
      SoundPlayer::play(static_data::one_up);
      interface.sigLife();
    break;

    case ITEM_UPGRADER:
      SoundPlayer::play(static_data::one_up);
    break;
  }
}

void Level::PlayerSpecialTilesInteraction(list<SpecialTile*> &special_tiles, AXIS axis, s8 sgn)
{
  for(list<SpecialTile*>::iterator i = special_tiles.begin(); i != special_tiles.end(); i++)
  {
    SpecialTile *tile = *i;

    switch(tile->getType())
    {
      case SPECIAL_TILE_CONTAINER:
      {
        ContainerTile *ctile = dynamic_cast<ContainerTile*>(tile);

        if(axis == AXIS_Y && sgn < 0)
        {
          if(ctile->getPhase() == 0)
          {
            ctile->nextPhase();
            switch(ctile->getItemType())
            {
              case ITEM_COIN:
                PlayerCollectItem(ITEM_COIN);
              break;

              case ITEM_UPGRADER:
              {
                Item *item = new item::Upgrader(*this);
                item->setPosition(ctile->getPosition() + IntVector(26, -20));
                item->getGravity().setValue(-700);

                active.items.push_back(item);
              }
              break;

              case ITEM_LIFE:
              {
                Item *item = new item::Life(*this);
                item->setPosition(ctile->getPosition() + IntVector(26, -20));
                item->getGravity().setValue(-700);

                active.items.push_back(item);
              }
              break;

              case ITEM_NONE: break;

            }

          }
        }
      }
      break;
      case SPECIAL_TILE_FRAGILE:
      {
        FragileTile *ftile = dynamic_cast<FragileTile*>(tile);

        if(axis == AXIS_Y && sgn < 0)
        {
          if(ftile->getPhase() == 0)
          {
            SingleRunEffect *effect;

            ftile->nextPhase();

            /* Destruction effect */
            effect = new SingleRunEffect(*this);
            animations.loadAnimation(ftile->getAnimation(1), *effect);
            effect->setBounds();
            effect->setPosition(ftile->getPosition());
            effect->setAlignment(ALIGN_BY_CORNER);
            active.effectlayers[action_tilelayer_id].push_back(effect);
          }
        }
      }
      break;
      case SPECIAL_TILE_DAMAGING:
      {
        //DamagingTile *dtile = dynamic_cast<DamagingTile*>(tile);
        PlayerKilled();
      }
      break;
    }
  }
}


void Level::CheckActivity()
{
  IntRect active_rect;

  active_rect.setX(-settings.getActivityBufferZone() + camera.getFixedPosition().getX());
  active_rect.setY(-settings.getActivityBufferZone() + camera.getFixedPosition().getY());
  active_rect.setWidth (camera.getViewSize().getWidth()  + settings.getActivityBufferZone() * 2);
  active_rect.setHeight(camera.getViewSize().getHeight() + settings.getActivityBufferZone() * 2);

  CheckActivity(active.platforms,    inactive.platforms, active_rect);
  CheckActivity(active.enemies,      inactive.enemies, active_rect);
  CheckActivity(active.bullets,      inactive.bullets, active_rect);
  CheckActivity(active.items,        inactive.items, active_rect);
  CheckActivity(active.eventareas,   inactive.eventareas, active_rect);
  CheckActivity(active.corpses, inactive.corpses, active_rect);
}

template<typename T>
void Level::CheckActivity(list<T*> &actives, list<T*> &inactives, const IntRect &active_rect)
{

  for(typename list<T*>::iterator i = actives.begin(); i != actives.end(); i++)
  {
    T* obj = *i;
    if(!obj->isActiveInRect(active_rect))
    {
      i = actives.erase(i);
      inactives.push_back(obj);
    }
  }
  for(typename list<T*>::iterator i = inactives.begin(); i != inactives.end(); i++)
  {
    T* obj = *i;
    if(obj->isActiveInRect(active_rect))
    {
      i = inactives.erase(i);
      actives.push_back(obj);
    }
  }
}

void Level::BlacklistCleanup()
{

  for(std::map<Object*, BLACKLIST_MEMBER>::iterator i = blacklist.begin(); i != blacklist.end();)
  {
    Object *object = i->first;

    if(i->second != BLACKLIST_REMOVE)
    {
      i++;
      continue;
    }


    switch(object->getType())
    {
      case OBJECT_PLATFORM: BlacklistCleanup(active.platforms,  inactive.platforms, i);  break;
      case OBJECT_ENEMY:    BlacklistCleanup(active.enemies,    inactive.enemies, i);    break;
      case OBJECT_BULLET:   BlacklistCleanup(active.bullets,    inactive.bullets, i);    break;
      case OBJECT_ITEM:     BlacklistCleanup(active.items,      inactive.items, i);      break;
      case OBJECT_EVENTAREA:BlacklistCleanup(active.eventareas, inactive.eventareas, i); break;
      case OBJECT_CORPSE:   BlacklistCleanup(active.corpses,    inactive.corpses, i);    break;

      case OBJECT_EFFECT:
      {
        for(int l = 0; l < (int)active.effectlayers.size(); l++)
        {
          bool ir = false;

          for(list<Effect*>::iterator _i = active.effectlayers[l].begin(); _i != active.effectlayers[l].end();)
          {
            if((Object*)*_i == object)
            {
              _i = active.effectlayers[l].erase(_i);
              blacklist.erase(i);
              i = blacklist.begin();
              ir = true;
              break;
            }
            else
            {
              _i++;
            }

          }

          if(ir)
            break;
        }
      }
      break;


      default:
        cerr << "Removal not handled for object of type '" << object->getType() << "'";
        i++;
        continue;
    }

    delete object;
  }

}

template<typename T>
void Level::BlacklistCleanup(list<T*> &actives, std::list<T*> &inactives, std::map<Object*, BLACKLIST_MEMBER>::iterator &i)
{
  Object *object = i->first;
  bool found;

  found = false;
  for(typename list<T*>::iterator _i = actives.begin(); _i != actives.end();)
  {
    if(dynamic_cast<Object*>(*_i) == object)
    {
      _i = actives.erase(_i);
      blacklist.erase(i);
      i = blacklist.begin();
      found = true;
      break;
    }
    else
    {
      _i++;
    }
  }
  if(found)
    return;

  for(typename list<T*>::iterator _i = inactives.begin(); _i != inactives.end();)
  {
    if(dynamic_cast<Object*>(*_i) == object)
    {
      _i = actives.erase(_i);
      blacklist.erase(i);
      i = blacklist.begin();
      found = true;
      break;
    }
    else
    {
      _i++;
    }
  }
}

void Level::InputFocus()
{
  /* TEXTBOX */
  if(input_focus == INPUT_FOCUS_TEXTBOX)
  {
    if(textbox.getState() == TEXTBOX_INACTIVE)
    {
      input_focus = INPUT_FOCUS_PLAYER;
    }
  }

  if(textbox.getState() != TEXTBOX_INACTIVE)
  {
    input_focus = INPUT_FOCUS_TEXTBOX;
    player.sigStopAll();
  }

  /* INTRO */
  if(input_focus == INPUT_FOCUS_INTRO && !intro.isActive())
  {
    input_focus = INPUT_FOCUS_PLAYER;
  }
}

void Level::EventCheck()
{
  if(textbox.getState() == TEXTBOX_INACTIVE)
  {
    event_pending = false;
  }
}

void Level::Save()
{
  Savestate savestate;

  savestate.setLifecount(player.getLifecount());
  savestate.setCoincount(player.getCoincount());
  savestate.setPlayerlevel(player.getLevel());
  savestate.setLevel(index + 1);

  savestate.save();
}
