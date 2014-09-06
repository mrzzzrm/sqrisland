#include "./LevelInterface.hpp"

using namespace std;
using namespace util;
using namespace geo;
using namespace error;
using namespace font;
using namespace gfx;
using namespace etc;

TtfFont LevelInterface::static_data::font;
Animation LevelInterface::static_data::life_icon;
Animation LevelInterface::static_data::coin_icon;

void LevelInterface::loadStaticData()
{
  try
  {
    static_data::font.load("dat/fonts/ALEAWBB_.TTF", 55);
    static_data::coin_icon.load("dat/interface/coincount.ani");
    static_data::life_icon.load("dat/interface/lifecount.ani");
  }
  catch(Exception &exception)
  {
    throw Exception() << "Error loading static data of LevelInterface: " <<  exception;
  }
}

void LevelInterface::clearStaticData()
{
  static_data::font.clear();
  static_data::coin_icon.clear();
  static_data::life_icon.clear();
}

LevelInterface::LevelInterface(const util::Timer &timer):
  timer(timer)
{
  coincount = 0;
  lifecount = 0;
  coincount_img = NULL;
  lifecount_img = NULL;

  renderCoincount();
  renderLifecount();
}

LevelInterface::~LevelInterface()
{
  if(coincount_img != NULL)
    delete coincount_img;
  if(lifecount_img != NULL)
    delete lifecount_img;
}

void LevelInterface::setCoincount(int count)
{
  coincount = count;
  renderCoincount();
}

void LevelInterface::setLifecount(int count)
{
  lifecount = count;
  renderLifecount();
}

void LevelInterface::sigCoin()
{
  coincount++;
  if(coincount >= 100)
  {
    coincount = 0;
    lifecount++;
    renderLifecount();
  }
  renderCoincount();
}

void LevelInterface::sigLife()
{
  lifecount++;
  renderLifecount();
}

void LevelInterface::sigKilled()
{
  lifecount--;
  renderLifecount();
}

void LevelInterface::update()
{

}

void LevelInterface::draw()
{
  assert(coincount_img != NULL);
  assert(lifecount_img != NULL);

  static_data::coin_icon.draw(IntPoint(gfx::getScreenSize().getWidth() - 195, 4));
  coincount_img->draw(IntPoint(gfx::getScreenSize().getWidth() - 120, -1), Alignment(0, 0));
  static_data::life_icon.draw(IntPoint(50, 10));
  lifecount_img->draw(IntPoint(140, 4), Alignment(0, 0));
}

void LevelInterface::renderCoincount()
{
  if(coincount_img != NULL)
    delete coincount_img;

  if(coincount < 10)
    coincount_img = static_data::font.render(string("0") + integer_to_string(coincount), Color(230, 150, 20), TTF_BLENDED);
  else
    coincount_img = static_data::font.render(integer_to_string(coincount), Color(230, 150, 20), TTF_BLENDED);
}

void LevelInterface::renderLifecount()
{
  if(lifecount_img != NULL)
    delete lifecount_img;

  lifecount_img = static_data::font.render(integer_to_string(lifecount), Color(230, 150, 20), TTF_BLENDED);
}
