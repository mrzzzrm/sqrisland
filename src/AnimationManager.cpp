#include "./AnimationManager.hpp"

using namespace std;
using namespace gfx;
using namespace util;


AnimationManager::AnimationManager()
{

}


AnimationManager::~AnimationManager()
{
  map_delete_all_and_clear(animations);
}


gfx::Animation &AnimationManager::loadAnimation(const util::Path &path)
{
  Animation *anim;

  anim = animations[path];
  if(anim == NULL)
    anim = animations[path] = new Animation(path);

  return *anim;
}

void AnimationManager::loadAnimation(const util::Path &path, gfx::Animation &dest)
{
  Animation *source;

  source = animations[path];
  if(source == NULL)
    source = animations[path] = new Animation(path);

  loadAnimation(*source, dest);
}


void AnimationManager::loadAnimation(const gfx::Animation &source, gfx::Animation &dest)
{
  if(&source == &dest)
    return;

  dest.clear();

  for(int f = 0; f < source.getFrameCount(); f++)
  {
    dest.addFrame(source.getFrameConst(f));
  }
}
