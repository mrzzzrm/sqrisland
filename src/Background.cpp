#include "./Background.hpp"

using namespace std;
using namespace geo;
using namespace util;
using namespace error;


Background::Background()
{

}


Background::~Background()
{

}


void Background::load(const tmx::base::Map &map)
{
  Path dir;


  dir = map.getPath();
  dir.stripFile();

  for(int l = 0;;l++)
  {
    if(!map.getPropertiesConst().hasValue(string("back[") + integer_to_string(l) + string("].path")))
      break;

    if(!map.getPropertiesConst().hasValue(string("back[") + integer_to_string(l) + string("].relx")))
       throw Exception() << "Missing 'relx' property in background-layer '" << l << "'";
    if(!map.getPropertiesConst().hasValue(string("back[") + integer_to_string(l) + string("].rely")))
       throw Exception() << "Missing 'rely' property in background-layer '" << l << "'";

    layers.push_back(new Layer);
      layers[l]->image.load(dir.append(map.getPropertiesConst().getValue(string("back[") + integer_to_string(l) + string("].path"))));
      layers[l]->relx = string_to_float(map.getPropertiesConst().getValue(string("back[") + integer_to_string(l) + string("].relx")));
      layers[l]->rely = string_to_float(map.getPropertiesConst().getValue(string("back[") + integer_to_string(l) + string("].rely")));
  }
}


void Background::draw(const Camera &camera)
{
  for(int l = 0; l < (int)layers.size(); l++)
  {
    int y = (int)(- camera.getPosition().getY() * layers[l]->rely);

    while(y < gfx::getScreenSize().getHeight())
    {
      int x = (int)(- camera.getPosition().getX() * layers[l]->relx);

      while(x < gfx::getScreenSize().getWidth())
      {
        layers[l]->image.draw(IntPoint(x, y));
        x += layers[l]->image.getSize().getWidth();
      }

      y += layers[l]->image.getSize().getHeight();
    }
  }
}




