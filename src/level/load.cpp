#include "../Level.hpp"

using namespace std;
using namespace gfx;
using namespace etc;
using namespace geo;
using namespace util;
using namespace input;
using namespace collision;
using namespace error;


void Level::LoadPlatforms(const tmx::base::Objectgroup &objectgroup, tmx::base::TileLayer &tilelayer)
{
  for(int pf = 0; pf < objectgroup.getObjectCount(); pf++)
  {
    Platform *platform = new Platform(*this, *this, tilelayer, objectgroup.getObject(pf));
    active.platforms.push_back(platform);
  }
}

void Level::LoadEnemies(const tmx::base::Objectgroup &objectgroup)
{
  for(int e = 0; e < objectgroup.getObjectCount(); e++)
  {
    Enemy *enemy;
    tmx::base::Object &object = objectgroup.getObject(e);

    if(object.getType() == "egg")
      enemy = new enemy::Egg(*this, object);
    else if(object.getType() == "kite")
      enemy = new enemy::Kite(*this, object);
    else if(object.getType() == "punk")
      enemy = new enemy::Punk(*this, object);
    else if(object.getType() == "tri")
      enemy = new enemy::Tri(*this, object);
    else
      throw Exception() << "Unknown enemy-type '" << object.getType() << "' of object " << object;

    active.enemies.push_back(enemy);
  }
}

void Level::LoadItems(const tmx::base::Objectgroup &objectgroup)
{
  for(int i = 0; i < objectgroup.getObjectCount(); i++)
  {
    Item *item;
    ITEM_TYPE type;

    tmx::base::Object &object = objectgroup.getObject(i);

    try
    {
      type = item_string_to_type(object.getType());
    }
    catch(Exception &exception)
    {
      throw Exception() << "Error in object '" << object << "': " << exception;
    }

    switch(type)
    {
      case ITEM_COIN:     item = new item::Coin(*this, object); break;
      case ITEM_UPGRADER: item = new item::Upgrader(*this, object); break;
      default: throw Exception() << "Unknown item-type '" << object.getType() << "'"; break;
    }

    active.items.push_back(item);
  }
}

void Level::LoadEventAreas(const tmx::base::Objectgroup &objectgroup)
{
  for(int i = 0; i < objectgroup.getObjectCount(); i++)
  {
    Event *event;
    EventArea *eventarea;
    EVENT_TYPE type;

    tmx::base::Object &object = objectgroup.getObject(i);
    type = event_string_to_type(object.getType());

    switch(type)
    {
      case EVENT_TEXTBOX:
      {
        string playtype_str, starttype_str;
        EVENT_PLAYTYPE playtype;
        EVENT_STARTTYPE starttype;
        string text;

        playtype_str = object.getProperties().getValue("play");
        starttype_str = object.getProperties().getValue("start");

        if(playtype_str == "once")        playtype = EVENT_PLAY_ONCE;
        else if(playtype_str == "always") playtype = EVENT_PLAY_ALWAYS;
        else throw Exception() << "'play' property not set in event::Textbox object '" << object << "'";

        if(starttype_str == "manual")         starttype = EVENT_START_MANUAL;
        else if(starttype_str == "automatic") starttype = EVENT_START_AUTOMATIC;
        else throw Exception() << "'start' property not set in event::Textbox object '" << object << "'";

        text = textmanager.getText(object.getProperties().getValue("text"));

        event = new event::Textbox(playtype, starttype, text);
      }
      break;

      case EVENT_CHECKPOINT:
      {
        if(object.getProperties().hasValue("reappearposition"))
          event = new event::Checkpoint(object.getProperties().getValue("reappearposition"), IntSize(Map::getTileWidth(), Map::getTileHeight()));
        else
          throw Exception() << "'reappearposition' property not set in object '" << object << "'";
      }
      break;

      case EVENT_OBJECTIVE_REACHED:
      {
        string starttype_str;
        EVENT_STARTTYPE starttype;

        starttype_str = object.getProperties().getValue("start");

        if(starttype_str == "manual")         starttype = EVENT_START_MANUAL;
        else if(starttype_str == "automatic") starttype = EVENT_START_AUTOMATIC;
        else throw Exception() << "'start' property not set in object '" << object << "'";

        event = new event::ObjectiveReached(starttype);
      }
      break;
      default: throw Exception() << "Unknown item-type '" << object.getType() << "'"; break;
    }
    eventarea = new EventArea(*this, event, (IntRect&)object);
    active.eventareas.push_back(eventarea);
  }
}

void Level::LoadCollisionMap(const tmx::base::TileLayer &tilelayer)
{
  RasterMap::setMapSize(IntSize(Map::getWidth(), Map::getHeight()));
  RasterMap::setCellSize(IntSize(Map::getTileWidth(), Map::getTileHeight()));

  for(int x = 0; x < Map::getWidth(); x++)
  {
    for(int y = 0; y < Map::getHeight(); y++)
    {
      if(tilelayer.getTile(x, y) != 0)
        RasterMap::setCell(IntPoint(x, y), true);
    }
  }
}

void Level::LoadSpecialTilesAndEffects(tmx::base::TileLayer &collision_tilelayer)
{
  std::map<int, tmx::base::Properties*> tile_properties;

  active.effectlayers.resize(Map::getTileLayerCount());
  inactive.effectlayers.resize(Map::getTileLayerCount());

  /* generate a map to have easy access to tile attributes by gid */
  for(int ts = 0; ts < Map::getTilesetCount(); ts++)
  {
    tmx::base::Tileset &tileset = Map::getTileset(ts);

    for(int id = 0; id < tileset.getTileCount(); id++)
    {
      tmx::base::TilesetTile *tile = tileset.getTile(id);

      if(tile == NULL)
        continue;

      tile_properties[id + tileset.getFirstGid()] = new tmx::base::Properties(tile->getPropertiesConst());
    }
  }

  /* Load effects */
  for(int tl = 0; tl < Map::getTileLayerCount(); tl++)
  {
    tmx::base::TileLayer &tilelayer = Map::getTileLayer(tl);

    for(int x = 0; x < Map::getWidth(); x++)
    {
      for(int y = 0; y < Map::getHeight(); y++)
      {
        tmx::base::Properties *properties = tile_properties[tilelayer.getTile(x, y)];

        if(properties == NULL)
          continue;

        /* Effects */
        if(properties->hasValue("animation"))
        {
          Path path;
          LoopedEffect *effect;

          path = directory.appendConst(properties->getValue("animation"));
          effect = new LoopedEffect(*this);

          animations.loadAnimation(path, *effect);
          effect->setBounds();
          effect->setPosition(IntPoint(x*Map::getTileWidth(), y*Map::getTileHeight()));
          effect->setAlignment(ALIGN_BY_CORNER);


          active.effectlayers[tl].push_back(effect);
          tilelayer.setTile(x, y, 0);
        }

      }
    }
  }

  special_tiles.load(collision_tilelayer, tile_properties);
}

void Level::LoadIntro()
{
  if(Map::getProperties().hasValue("intro_line_0"))
    intro.setLine0(Map::getProperties().getValue("intro_line_0"));
  if(Map::getProperties().hasValue("intro_line_1"))
    intro.setLine1(Map::getProperties().getValue("intro_line_1"));
}

void Level::LoadPlayer()
{
  player.setSpeed(this->settings.getPlayerSpeed());
  player.setJumpForce(this->settings.getPlayerJumpForce());
  player.setSpringJumpMaxDelay(this->settings.getPlayerSpringJumpMaxDelay());

  /* Startpoint */
  if(Map::getProperties().hasValue("startposition"))
  {
    string str = Map::getProperties().getValue("startposition");
    vector<string> segs = string_split_to_vector(str, "/");

    if(segs.size() != 2)
      throw Exception() << "Mapproperty 'startposition' must have 2 segments seperated by a '/'";
    reappearposition = IntPoint(string_to_float(segs[0]) * Map::getTileWidth(), string_to_float(segs[1]) * Map::getTileHeight());
    player.setPosition(reappearposition);
  }
  else
  {
    throw Exception() << "No such attribute 'startposition' in map.";
  }
}

void Level::LoadCamera()
{
  camera.setViewSize(gfx::getScreenSize());
  camera.setMapSize(IntSize(Map::getTileWidth() * Map::getWidth(), Map::getTileHeight() * Map::getHeight()));
  camera.setSpeed(this->settings.getCameraSpeed());
  camera.warp(player.getPosition());
}

void Level::LoadFading()
{
  fading.setSize(IntSize(-1, -1));
  fading.setTicks(500);
  fading.setColor(BLACK);
}
