#include "./Checkpoint.hpp"

using namespace std;
using namespace geo;
using namespace error;
using namespace util;

namespace event
{
  Checkpoint::Checkpoint(const string &relative_reappearposition, const geo::IntSize &tilesize):
    Event(EVENT_CHECKPOINT, EVENT_PLAY_ALWAYS, EVENT_START_AUTOMATIC)
  {
    vector<string> segs = string_split_to_vector(relative_reappearposition, "/");

    if(segs.size() != 2)
      throw Exception() << "Checkpoint attribute 'relative_reappearposition' must have 2 segments seperated by a '/'.";

    this->relative_reappearposition = IntPoint((int)(string_to_float(segs[0]) * tilesize.getWidth()), (int)(string_to_float(segs[1]) * tilesize.getHeight()));
  }

  Checkpoint::~Checkpoint()
  {

  }

  const IntPoint &Checkpoint::getRelativeReappearPosition() const
  {
    return relative_reappearposition;
  }
}
