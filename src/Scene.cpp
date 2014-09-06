#include "./Scene.hpp"

Scene::Scene()
{
  state = SCENE_STATE_IN;
}

Scene::~Scene()
{

}

SCENE_STATE Scene::getState() const
{
  return state;
}

void Scene::setState(SCENE_STATE state)
{
  this->state = state;
}
