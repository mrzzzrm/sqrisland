#include "./SpecialTileMap.hpp"

using namespace std;
using namespace geo;
using namespace gfx;
using namespace util;
using namespace error;


SpecialTileMap::SpecialTileMap(const util::Timer &timer, const tmx::base::Map &tmxmap, AnimationManager &animations):
  timer(timer), tmxmap(tmxmap), animations(animations)
{

}


SpecialTileMap::~SpecialTileMap()
{

}


void SpecialTileMap::load(tmx::base::TileLayer &tilelayer, std::map<int, tmx::base::Properties*> &tile_properties)
{
  map.resize(tilelayer.getWidth());
  for(int x = 0; x < tilelayer.getWidth(); x++)
  {
    map[x].resize(tilelayer.getHeight());
  }


  for(int x = 0; x < tilelayer.getWidth(); x++)
  {
    for(int y = 0; y < tilelayer.getHeight(); y++)
    {
      tmx::base::Properties *properties;

      properties = tile_properties[tilelayer.getTile(x, y)];
      if(properties == NULL)
        continue;

      if(properties->hasValue("special_type"))
      {
        IntRect rect;
        SpecialTile *tile;

        try
        {
          switch(special_tile_string_to_type(properties->getValue("special_type")))
          {
            case SPECIAL_TILE_CONTAINER: tile = LoadContainerTile(tilelayer.getTile(x, y), *properties, tilelayer); break;
            case SPECIAL_TILE_FRAGILE:   tile = LoadFragileTile(tilelayer.getTile(x, y), *properties, tilelayer);   break;
            case SPECIAL_TILE_DAMAGING:  tile = LoadDamagingTile(tilelayer.getTile(x, y), *properties, tilelayer);  break;
            default: continue;
          }
        }
        catch(Exception &exception)
        {
          throw Exception() << "Error loading tile with gid '" << tilelayer.getTile(x,y) << "': " << exception;
        }

        /* Position, removal from mainlayer and storing */
        tile->setPosition(IntPoint(x * tmxmap.getTileWidth(), y * tmxmap.getTileHeight()));

        tilelayer.setTile(x, y, 0);
        map[x][y] = tile;
      }
      else
      {
        map[x][y] = NULL;
      }
    }
  }
}


bool SpecialTileMap::isCollision(const Object &object) const
{
  int ftx, fty;
  int ltx, lty;

  CollisionCheckArea(object, ftx, fty, ltx, lty);

  for(int x = ftx; x <= ltx; x++)
  {
    for(int y = fty; y <= lty; y++)
    {
      if(map[x][y] != NULL)
      {
        if(map[x][y]->isCollision(object))
          return true;
      }
    }
  }

  return false;
}


std::list<SpecialTile*> SpecialTileMap::getCollisionWithPlayer(const Player &player)
{
  int ftx, fty;
  int ltx, lty;
  std::list<SpecialTile*> res;

  CollisionCheckArea(player, ftx, fty, ltx, lty);

  for(int x = ftx; x <= ltx; x++)
  {
    for(int y = fty; y <= lty; y++)
    {
      if(map[x][y] != NULL)
      {
        if(map[x][y]->isCollision(player))
          res.push_back(map[x][y]);
      }
    }
  }

  return res;
}



void SpecialTileMap::update()
{
  for(int x = 0; x < (int)map.size(); x++)
  {
    for(int y = 0; y < (int)map[x].size(); y++)
    {
      SpecialTile *tile = map[x][y];

      if(tile == NULL)
        continue;

      tile->update();

      switch(tile->getType())
      {
        case SPECIAL_TILE_FRAGILE:
          if(tile->getPhase() > 0)
          {
            delete map[x][y];
            map[x][y] = NULL;
          }
        break;

        default:
        break;
      }
    }
  }
}


void SpecialTileMap::draw(const Camera &camera)
{
  int ftx, fty;
  int ltx, lty;

  ftx = (int)camera.getPosition().getX() / tmxmap.getTileWidth();
  fty = (int)camera.getPosition().getY() / tmxmap.getTileHeight();
  ltx = ((int)camera.getPosition().getX() + camera.getViewSize().getWidth()) / tmxmap.getTileWidth();
  lty = ((int)camera.getPosition().getY() + camera.getViewSize().getHeight()) / tmxmap.getTileHeight();

  limit(ftx, 0, tmxmap.getWidth() - 1);
  limit(fty, 0, tmxmap.getHeight() - 1);
  limit(ltx, 0, tmxmap.getWidth() - 1);
  limit(lty, 0, tmxmap.getHeight() - 1);

  for(int x = ftx; x <= ltx; x++)
  {
    for(int y = fty; y <= lty; y++)
    {
      if(map[x][y] != NULL)
        map[x][y]->draw(camera);
    }
  }
}


SpecialTile *SpecialTileMap::LoadContainerTile(int gid, const tmx::base::Properties &properties, const tmx::base::TileLayer &tilelayer)
{
  SpecialTile *tile;
  ContainerTile *ctile;


  ctile = new ContainerTile(timer, properties, tilelayer);
  tile = ctile;

  try
  {
    for(int a = 0; a < 3; a++)
    {
      if(properties.hasValue(string("container_anim_") + integer_to_string(a)))
        animations.loadAnimation(Path(properties.getValue(string("container_anim_") + integer_to_string(a))), ctile->getAnimation(a));
      else
        ctile->getAnimation(a).addFrame(Frame(tmxmap.getTileAsImageClipping(gid), 1));
    }

    try
    {
      ctile->setItemType(item_string_to_type(properties.getValue("container_item")));
    }
    catch(Exception &exception)
    {
      throw Exception() << "Error in gid '" << gid << "': " << exception;
    }
  }
  catch(Exception &exception)
  {
    throw Exception() << "Error loading ContainerTile from gid '" << gid << "': " << exception;
  }

  return tile;
}


SpecialTile *SpecialTileMap::LoadFragileTile(int gid, const tmx::base::Properties &properties, const tmx::base::TileLayer &tilelayer)
{
  SpecialTile *tile;
  FragileTile *ftile;


  ftile = new FragileTile(timer, properties, tilelayer);
  tile = ftile;

  try
  {
    for(int a = 0; a < 2; a++)
    {
      if(properties.hasValue(string("fragile_anim_") + integer_to_string(a)))
        animations.loadAnimation(Path(properties.getValue(string("fragile_anim_") + integer_to_string(a))), ftile->getAnimation(a));
      else
        ftile->getAnimation(a).addFrame(Frame(tmxmap.getTileAsImageClipping(gid), 1));
    }
  }
  catch(Exception &exception)
  {
    throw Exception() << "Error loading FragileTile from gid '" << gid << "': " << exception;
  }

  return tile;
}


SpecialTile *SpecialTileMap::LoadDamagingTile(int gid, const tmx::base::Properties &properties, const tmx::base::TileLayer &tilelayer)
{
  SpecialTile *tile;
  DamagingTile *dtile;


  dtile = new DamagingTile(timer, properties, tilelayer);
  tile = dtile;

  try
  {
    if(properties.hasValue("damaging_anim"))
      animations.loadAnimation(Path(properties.getValue("damaging_anim")), dtile->getAnimation());
    else
      dtile->getAnimation().addFrame(Frame(tmxmap.getTileAsImageClipping(gid), 1));
  }
  catch(Exception &exception)
  {
    throw Exception() << "Error loading DamagingTile from gid '" << gid << "': " << exception;
  }

  return tile;
}


void SpecialTileMap::CollisionCheckArea(const Object &object, int &ftx, int &fty, int &ltx, int &lty) const
{
  IntPoint tlc; // topleft corner
  tlc = object.getPosition() - object.getAlignment() * object.getSize();

  ftx = (tlc.getX() - 0) / tmxmap.getTileWidth();
  fty = (tlc.getY() - 0) / tmxmap.getTileHeight();
  ltx = (tlc.getX() + object.getSize().getWidth() - 1) / tmxmap.getTileWidth();
  lty = (tlc.getY() + object.getSize().getHeight() - 1) / tmxmap.getTileHeight();

  limit(ftx, 0, tmxmap.getWidth() - 1);
  limit(fty, 0, tmxmap.getHeight() - 1);
  limit(ltx, 0, tmxmap.getWidth() - 1);
  limit(lty, 0, tmxmap.getHeight() - 1);
}


