#include "./SingleRunEffect.hpp"

using namespace std;
using namespace geo;
using namespace gfx;

SingleRunEffect::SingleRunEffect(const util::Timer &timer):
  Effect(timer, EFFECT_SINGLE_RUN)
{

}


SingleRunEffect::~SingleRunEffect()
{

}


bool SingleRunEffect::hasRan() const
{
  return Animation::getLoopCount() != 0;
}


void SingleRunEffect::setBounds()
{
  int minx, miny, maxx, maxy;

  if(Animation::getFrameCount() == 0)
    return;

  minx = Animation::getOffset().getX() + Animation::getFrame(0).getOffset().getX();
  miny = Animation::getOffset().getY() + Animation::getFrame(0).getOffset().getY();
  maxx = Animation::getOffset().getX() + Animation::getFrame(0).getOffset().getX() + Animation::getFrame(0).getWidth();
  maxy = Animation::getOffset().getY() + Animation::getFrame(0).getOffset().getY() + Animation::getFrame(0).getHeight();

  for(int f = 1; f < Animation::getFrameCount(); f++)
  {
    minx = min(minx, Animation::getOffset().getX() + Animation::getFrame(f).getOffset().getX());
    miny = min(miny, Animation::getOffset().getY() + Animation::getFrame(f).getOffset().getY());
    maxx = max(maxx, Animation::getOffset().getX() + Animation::getFrame(f).getOffset().getX() + Animation::getFrame(f).getWidth());
    maxy = max(maxy, Animation::getOffset().getY() + Animation::getFrame(f).getOffset().getY() + Animation::getFrame(f).getHeight());
  }

  Object::setBoundsSize(IntSize(maxx - minx, maxy - miny));
}


void SingleRunEffect::draw(const Camera &camera)
{
  Animation::update();
  if(Animation::getLoopCount() == 0)
    Animation::drawConst(Effect::getPosition() - point_to_vector(camera.getPosition()), Effect::getAlignment());
}
