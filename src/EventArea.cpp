#include "./EventArea.hpp"

using namespace geo;
using namespace std;
using namespace etc;
using namespace util;


EventArea::EventArea(const util::Timer &timer, Event *event, const IntRect &rect):
  Object(timer, OBJECT_EVENTAREA)
{
  assert(event);

  this->event = event;
  this->rect = rect;
  entered = false;

  RectMap::setSize(rect_to_size(rect));
  RectMap::addRect(rect);
  Object::setBoundsSize(rect_to_size(rect));
  Object::setPosition(IntPoint(rect.getX(), rect.getY()));

  Object::setAlignment(ALIGN_BY_CORNER);
  Object::getGravity().setActive(false);
}

EventArea::~EventArea()
{
  if(event != NULL)
    delete event;
}

const Event &EventArea::getEvent() const
{
  return *event;
}

const IntRect &EventArea::getRect() const
{
  return rect;
}

bool EventArea::isEntered() const
{
  return entered;
}

void EventArea::setEntered(bool is)
{
  entered = is;
}

void EventArea::draw(const Camera &camera)
{

}
