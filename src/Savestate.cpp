#include "./Savestate.hpp"

using namespace std;
using namespace geo;
using namespace util;
using namespace error;


Savestate::Savestate()
{
  level = 0;
  lifecount = 0;
  coincount = 0;
  playerlevel = 0;
}

Savestate::~Savestate()
{

}

void Savestate::setLevel(int level)
{
  this->level = level;
}

int Savestate::getLevel() const
{
  return level;
}

void Savestate::setLifecount(int lifecount)
{
  this->lifecount = lifecount;
}

int Savestate::getLifecount() const
{
  return lifecount;
}

void Savestate::setCoincount(int coincount)
{
  this->coincount = coincount;
}

int Savestate::getCoincount()
{
  return coincount;
}

void Savestate::setPlayerlevel(int playerlevel)
{
  this->playerlevel = playerlevel;
}

int Savestate::getPlayerlevel() const
{
  return playerlevel;
}

void Savestate::load()
{
  string data_str = file_to_string("save.dat");
  vector<string> data_segs = string_split_to_vector(data_str, "/");

  if(data_segs.size() != 4)
    throw Exception() << "Corrupt savedata, delete appdata";


  level = string_to_integer(data_segs[0]);
  lifecount = string_to_integer(data_segs[1]);
  coincount = string_to_integer(data_segs[2]);
  playerlevel = string_to_integer(data_segs[3]);
}

void Savestate::save()
{
  string data;

  data += integer_to_string(level) + string("/");
  data += integer_to_string(lifecount) + string("/");
  data += integer_to_string(coincount) + string("/");
  data += integer_to_string(playerlevel) + string("/");

  string_to_file(data, "save.dat");
}

bool Savestate::exists()
{
  return Path("save.dat").exists();
}

void Savestate::remove()
{
  Path("save.dat").remove();
}
