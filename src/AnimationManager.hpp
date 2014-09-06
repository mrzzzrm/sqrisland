#ifndef ANIMATIONMANAGER_HPP
#define ANIMATIONMANAGER_HPP

  #include "framework.hpp"


  class AnimationManager
  {
    public:

      AnimationManager();
      ~AnimationManager();


      gfx::Animation &loadAnimation(const util::Path &path);
      void loadAnimation(const util::Path &path, gfx::Animation &dest);
      void loadAnimation(const gfx::Animation &source, gfx::Animation &dest);


    private:

      std::map<util::Path, gfx::Animation*> animations;
  };

#endif // ANIMATIONMANAGER_HPP
