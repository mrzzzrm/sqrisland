#include "./LevelIntro.hpp"

using namespace std;
using namespace gfx;
using namespace font;
using namespace etc;
using namespace util;
using namespace geo;
using namespace input;
using namespace error;



TtfFont LevelIntro::static_data::line_0_font;
TtfFont LevelIntro::static_data::line_1_font;



void LevelIntro::loadStaticData()
{
  try
  {
    static_data::line_0_font.load("dat/fonts/ALEAWBB_.TTF", 96);
    static_data::line_1_font.load("dat/fonts/ALEAWBB_.TTF", 32);
  }
  catch(Exception &exception)
  {
    throw Exception() << "Error loading static data of LevelIntro: " << exception;
  }
}

void LevelIntro::clearStaticData()
{
  static_data::line_0_font.clear();
  static_data::line_1_font.clear();
}

LevelIntro::LevelIntro(const Timer &timer):
  timer(timer)
{
  state = LEVELINTRO_STATE_INACTIVE;
  fading.setSize(IntSize(-1, -1));
  fading.setTicks(1000);
  fading.setColor(BLACK);
  line_0.setFont(&static_data::line_0_font);
  line_1.setFont(&static_data::line_1_font);
  line_0_x = -gfx::getScreenSize().getWidth()/2;
  line_1_x = 3*gfx::getScreenSize().getWidth()/2
;}

LevelIntro::~LevelIntro()
{

}

void LevelIntro::start()
{
  state = LEVELINTRO_STATE_IN;
  fading.forceStart(FADE_OUT);
  line_0_x = -gfx::getScreenSize().getWidth()/2;
  line_1_x = 3*gfx::getScreenSize().getWidth()/2;
}

bool LevelIntro::isActive() const
{
  return state != LEVELINTRO_STATE_INACTIVE;
}

void LevelIntro::setLine0(const std::string &text)
{
  line_0.setText(text);
  line_0.setColor(Color(250, 100, 0));
  line_0.setRendermode(TTF_BLENDED);

  line_0.render();
}

void LevelIntro::setLine1(const std::string &text)
{
  line_1.setText(text);
  line_1.setColor(Color(250, 100, 0));
  line_1.setRendermode(TTF_BLENDED);

  line_1.render();
}

void LevelIntro::update()
{
  if(state == LEVELINTRO_STATE_IN || state == LEVELINTRO_STATE_MAIN)
  {
    line_0_x += timer.getPerSecond(1500);
    line_1_x -= timer.getPerSecond(1500);

    limit(line_0_x, (double)-gfx::getScreenSize().getWidth()/2, (double)gfx::getScreenSize().getWidth()/2);
    limit(line_1_x, (double)gfx::getScreenSize().getWidth()/2, (double)3*gfx::getScreenSize().getWidth()/2);
  }

  switch(state)
  {
    case LEVELINTRO_STATE_INACTIVE: break;

    case LEVELINTRO_STATE_IN:
      if(fading.getAlpha() <= 0.5)
      {
        fading.pause();
        fading.setAlpha(0.5);
        state = LEVELINTRO_STATE_MAIN;
      }
    break;

    case LEVELINTRO_STATE_MAIN:
      if(Input::isButtonPressed(BUTTON_B))
      {
        fading.run();
        state = LEVELINTRO_STATE_OUT;
      }
    break;

    case LEVELINTRO_STATE_OUT:
      line_0_x += timer.getPerSecond(1500);
      line_1_x -= timer.getPerSecond(1500);
      if(fading.isDone())
        state = LEVELINTRO_STATE_INACTIVE;
    break;
  }
}

void LevelIntro::draw()
{
  fading.draw();

//  cout << line_0_x << " ";
//  cout << line_1_x << endl;

  line_0.draw(IntPoint(line_0_x, 220), ALIGN_BY_CENTER);
  line_1.draw(IntPoint(line_1_x, 280), ALIGN_BY_CENTER);
}


