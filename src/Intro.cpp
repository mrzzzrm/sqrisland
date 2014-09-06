#include "./Intro.hpp"

using namespace std;
using namespace gfx;
using namespace etc;
using namespace geo;
using namespace input;



Intro::Intro()
{
  fading.setTicks(500);
}

Intro::~Intro()
{

}

void Intro::load()
{
  img.load("dat/etc/intro.png");
  fading.setImage(&img);
  fading.setTicks(500);
}

void Intro::in()
{
  fading.forceStart(FADE_IN);
  while(!fading.isDone())
  {
    Input::update();
    fading.draw(IntPoint(gfx::getScreenSize().getWidth()/2, gfx::getScreenSize().getHeight()/2), ALIGN_BY_CENTER);
    gfx::updateScreen(true);
  }

  in_complete = timer.getTicks();
}

void Intro::out()
{
  while(timer.getTicks() - in_complete < 1000)
  {
    Input::update();
    timer.update();
    img.draw(IntPoint(gfx::getScreenSize().getWidth()/2, gfx::getScreenSize().getHeight()/2), ALIGN_BY_CENTER);
    gfx::updateScreen(true);
  }

  fading.forceStart(FADE_OUT);

  while(!fading.isDone())
  {
    Input::update();
    fading.draw(IntPoint(gfx::getScreenSize().getWidth()/2, gfx::getScreenSize().getHeight()/2), ALIGN_BY_CENTER);
    gfx::updateScreen(true);
  }
}
