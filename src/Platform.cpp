#include "./Platform.hpp"

using namespace std;
using namespace etc;
using namespace geo;
using namespace gfx;
using namespace error;
using namespace util;


Platform::Platform(const Timer &timer, tmx::base::Map &map, tmx::base::TileLayer &tilelayer, tmx::base::Object &object):
  Object(timer, OBJECT_PLATFORM)
{
  waypoint_iterator = waypoints.end();
  waiting.since = 0;
  waiting.is = false;

  LoadWaypoints(object);
  LoadRects(object);
  LoadImage(map, tilelayer, object);

  Object::setPosition(IntPoint(object.getX(), object.getY()));
  Object::setAlignment(ALIGN_BY_CORNER);
  Object::getGravity().setActive(false);
  Object::setBoundsSize(IntSize(object.getWidth(), object.getHeight()));
}


Platform::~Platform()
{
  list_delete_all_and_clear(waypoints);
}


bool Platform::isActiveInRect(const geo::IntRect &rect) const
{
  if(Object::isActiveInRect(rect))
    return true;

  for(list<Waypoint*>::const_iterator i = waypoints.begin(); i != waypoints.end(); i++)
  {
    IntPoint point = *((IntPoint*)*i);

    if((point.getX() >= rect.getX() && point.getX() < rect.getX() + rect.getWidth()) &&
      (point.getY() >= rect.getY() && point.getY() < rect.getY() + rect.getHeight()))
      return true;
  }
  return false;
}


void Platform::update()
{
  Waypoint *waypoint;
  FloatVector vector;
  double dir, len, step;


  Object::update();

  if(waypoints.size() == 0)
    return;

  if(waypoint_iterator == waypoints.end())
    waypoint_iterator = waypoints.begin();
  waypoint = *waypoint_iterator;

  if(waiting.is)
  {
    if(Object::getTimer().getTicks() - waiting.since >= waypoint->getWait())
    {
      waiting.is = false;
      waypoint_iterator++;

      if(waypoint_iterator == waypoints.end())
        waypoint_iterator = waypoints.begin();
      waypoint = *waypoint_iterator;
    }
  }


  if(!waiting.is)
  {
    vector.load(*(IntPoint*)waypoint - Object::getPosition());
    dir = vector.getDirection();
    len = vector.getLength();
    step = Object::getTimer().getPerSecond(waypoint->getSpeed());

    if(len < step)
      step = len;

    Object::getMovementBuffer().pushVector(FloatVector(cos(dir) * step, -sin(dir) * step));

    if(*(IntPoint*)waypoint == Object::getPosition())
    {
      waiting.is = true;
      waiting.since = Object::getTimer().getTicks();
    }
  }

}


void Platform::draw(const Camera &camera)
{
  for(list<pair<ImageClipping, IntVector> >::iterator i = tiles.begin(); i != tiles.end(); i++)
  {
    i->first.draw(Object::getPosition() - point_to_vector(camera.getPosition()) + i->second, etc::ALIGN_BY_CORNER, IntRect(0, 0, -1, -1));
  }
}


void Platform::LoadWaypoints(tmx::base::Object &object)
{
  int tw, th;
  int global_wait;
  double global_speed;
  bool has_global_speed;
  double speed_fac;


  global_wait = 0;
  global_speed = 0;
  has_global_speed = false;

  tw = object.getObjectgroup().getMap().getTileWidth();
  th = object.getObjectgroup().getMap().getTileHeight();

  speed_fac = sqrt(tw*tw + th*th);


  /* Global wait and speed */
  if(object.getProperties().hasValue("wait"))
  {
    global_wait = string_to_integer(object.getProperties().getValue("wait"));
  }
  if(object.getProperties().hasValue("speed"))
  {
    global_speed = string_to_float(object.getProperties().getValue("speed"));
    has_global_speed = true;
  }


  /* Iterate */
  for(int wp = 0;;wp++)
  {
    string point_str;
    vector<string> point_segs;
    IntPoint point;
    int wait;
    double speed;


    if(!object.getProperties().hasValue(string("waypoint[") + integer_to_string(wp) + string("]")))
      break;


    /* Point */
    point_str = object.getProperties().getValue(string("waypoint[") + integer_to_string(wp) + string("]"));
    point_segs = string_split_to_vector(point_str, "/");

    if(point_segs.size() != 2)
      throw Exception() << "Wrong number of segments in waypoint[" << wp << "]='" << point_str << "' of platform " << object << ", has to be 2";

    point.setX(string_to_integer(point_segs[0]) * tw);
    point.setY(string_to_integer(point_segs[1]) * th);


    /* Speed */
    if(object.getProperties().hasValue(string("waypoint[") + integer_to_string(wp) + string("].speed")))
      speed = string_to_float(object.getProperties().getValue(string("waypoint[") + integer_to_string(wp) + string("].speed")));
    else if(has_global_speed)
      speed = global_speed;
    else
      throw Exception() << "No speed set for 'waypoint[" << wp << "]' of platform " << object;

    speed *= speed_fac;


    /* Wait */
    if(object.getProperties().hasValue(string("waypoint[") + integer_to_string(wp) + string("].wait")))
      wait = string_to_integer(object.getProperties().getValue(string("waypoint[") + integer_to_string(wp) + string("].wait")));
    else
      wait = global_wait;


    /* Add */
    waypoints.push_back(new Waypoint(point, wait, speed));
  }
}


void Platform::LoadRects(tmx::base::Object &object)
{
  int tw, th;

  tw = object.getObjectgroup().getMap().getTileWidth();
  th = object.getObjectgroup().getMap().getTileHeight();

  RectMap::setSize((IntSize)rect_to_size(object));

  if(object.getProperties().hasValue("rect[0]"))
  {
    int ri = 0;

    do
    {
      string rect_str;
      vector<string> rect_segs;
      IntRect rect;

      rect_str = object.getProperties().getValue(string("rect[") + integer_to_string(ri) + string("]"));
      rect_segs = string_split_to_vector(rect_str, "/");

      if(rect_segs.size() != 4)
        throw Exception() << "Wrong number of segments in rect[" << ri << "]='" << rect_str << "' of platform " << object << ", has to be 4";

      rect.setX((int)(string_to_float(rect_segs[0]) * tw));
      rect.setY((int)(string_to_float(rect_segs[1]) * th));
      rect.setWidth((int)(string_to_float(rect_segs[2]) * tw));
      rect.setHeight((int)(string_to_float(rect_segs[3]) * th));

      Object::addRect(rect);

      ri++;
    }while(object.getProperties().hasValue(string("rect[") + integer_to_string(ri) + string("]")));
  }
  else
  {
    Object::addRect(IntRect(0, 0, (int)object.getWidth(), (int)object.getHeight()));
  }
}


void Platform::LoadImage(tmx::base::Map &map, tmx::base::TileLayer &tilelayer, tmx::base::Object &object)
{
  int ox, oy;
  int ow, oh;


  ox = object.getX() / map.getTileWidth();
  oy = object.getY() / map.getTileHeight();
  ow = object.getWidth() / map.getTileWidth();
  oh = object.getHeight() / map.getTileHeight();

  for(int rx = 0; rx < ow; rx++)
  {
    for(int ry = 0; ry < oh; ry++)
    {
      int gid;

      gid = tilelayer.getTile(rx + ox, ry + oy);

      if(gid == 0)
        continue;

      tiles.push_back(pair<gfx::ImageClipping, geo::IntVector>(map.getTileAsImageClipping(gid), IntVector(rx * map.getTileWidth(), ry * map.getTileHeight())));
      tilelayer.setTile(rx + ox, ry + oy, 0);
    }
  }


}

