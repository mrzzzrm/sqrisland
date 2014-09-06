#include "./Settings.hpp"

using namespace std;
using namespace util;
using namespace geo;
using namespace error;


Settings::Settings()
{
  gravity_acc = 0;
  gravity_max = 0;

  camera_speed = 0;

  player_speed = 0;
  player_jump_dur = 0;
  player_jump_force = 0;
  player_on_enemy_jump_dur = 0;
  player_spring_jump_max_delay = 0;
  player_damaged_jump_dur = 0;

  activity_check_interval = 0;
  activity_buffer_zone    = 0;

  textbox_line_height          = 0;
  textbox_char_interval_normal = 0;
  textbox_char_interval_fast   = 0;
  textbox_scroll_speed_normal  = 0;
  textbox_scroll_speed_fast    = 0;
  textbox_box_slide_speed      = 0;
  textbox_y_in                 = 0;
  textbox_y_out                = 0;
  textbox_font_size            = 0;
}


Settings::Settings(const Settings &settings)
{
  gravity_acc = settings.gravity_acc;
  gravity_max = settings.gravity_max;

  camera_speed = settings.camera_speed;

  player_speed                 = settings.player_speed;
  player_jump_dur              = settings.player_jump_dur;
  player_jump_force            = settings.player_jump_force;
  player_on_enemy_jump_dur     = settings.player_on_enemy_jump_dur;
  player_spring_jump_max_delay = settings.player_spring_jump_max_delay;
  player_damaged_jump_dur      = settings.player_damaged_jump_dur;

  activity_check_interval = settings.activity_check_interval;
  activity_buffer_zone    = settings.activity_buffer_zone;

  textbox_textrect             = settings.textbox_textrect;
  textbox_line_height          = settings.textbox_line_height;
  textbox_char_interval_normal = settings.textbox_char_interval_normal;
  textbox_char_interval_fast   = settings.textbox_char_interval_fast;
  textbox_scroll_speed_normal  = settings.textbox_scroll_speed_normal;
  textbox_scroll_speed_fast    = settings.textbox_scroll_speed_fast;
  textbox_box_slide_speed      = settings.textbox_box_slide_speed;
  textbox_y_in                 = settings.textbox_y_in;
  textbox_y_out                = settings.textbox_y_out;
  textbox_font_size            = settings.textbox_font_size;
}


Settings::~Settings()
{

}


void Settings::load()
{
  Config config;


  try{
    config.load(Path("dat/settings.conf"));
  }
  catch(Exception &exception){
    throw Exception() << "Error loadingconfig file 'dat/settings.conf': " << exception;
  }


  try
  {
    vector<string> textbox_textrect_segs;


    gravity_acc  = config.getFloat("gravity_acc");
    gravity_max  = config.getFloat("gravity_max");

    camera_speed = config.getFloat("camera_speed");

    player_speed                 = config.getFloat("player_speed");
    player_jump_dur              = config.getInteger("player_jump_dur");
    player_jump_force            = config.getFloat("player_jump_force");
    player_on_enemy_jump_dur     = config.getInteger("player_on_enemy_jump_dur");
    player_spring_jump_max_delay = config.getInteger("player_spring_jump_max_delay");
    player_damaged_jump_dur     = config.getInteger("player_damaged_jump_dur");

    activity_check_interval = config.getInteger("activity_check_interval");
    activity_buffer_zone    = config.getInteger("activity_buffer_zone");

    textbox_textrect_segs        = string_split_to_vector(config.getString("textbox_textrect"), "/");
      if(textbox_textrect_segs.size() != 4)
        throw Exception() << "Invalid number of segments in textbox_textrect, should be 4";
      textbox_textrect.setX(string_to_integer(textbox_textrect_segs[0]));
      textbox_textrect.setY(string_to_integer(textbox_textrect_segs[1]));
      textbox_textrect.setWidth(string_to_integer(textbox_textrect_segs[2]));
      textbox_textrect.setHeight(string_to_integer(textbox_textrect_segs[3]));

    textbox_line_height          = config.getInteger("textbox_line_height");
    textbox_char_interval_normal = config.getInteger("textbox_char_interval_normal");
    textbox_char_interval_fast   = config.getInteger("textbox_char_interval_fast");
    textbox_scroll_speed_normal  = config.getFloat("textbox_scroll_speed_normal");
    textbox_scroll_speed_fast    = config.getFloat("textbox_scroll_speed_fast");
    textbox_box_slide_speed      = config.getFloat("textbox_box_slide_speed");
    textbox_y_in                 = config.getInteger("textbox_y_in");
    textbox_y_out                = config.getInteger("textbox_y_out");
    textbox_x                    = config.getInteger("textbox_x");
    textbox_font_size            = config.getInteger("textbox_font_size");
  }
  catch(Exception &exception)
  {
    throw Exception() << "Error loading setting: " << exception;
  }
}


void Settings::multiplyTileBasedSpeeds(double fac)
{
  gravity_acc       *= fac;
  gravity_max       *= fac;
  camera_speed      *= fac;
  player_speed      *= fac;
  player_jump_force *= fac;
}


const double &Settings::getGravityAcc() const
{
  return gravity_acc;
}


const double &Settings::getGravityMax() const
{
  return gravity_max;
}


const double &Settings::getCameraSpeed() const
{
  return camera_speed;
}


const double &Settings::getPlayerSpeed() const
{
  return player_speed;
}


const int &Settings::getPlayerJumpDur() const
{
  return player_jump_dur;
}


const double &Settings::getPlayerJumpForce() const
{
  return player_jump_force;
}


const int &Settings::getPlayerOnEnemyJumpDur() const
{
  return player_on_enemy_jump_dur;
}


const int &Settings::getPlayerSpringJumpMaxDelay() const
{
  return player_spring_jump_max_delay;
}


const int &Settings::getPlayerDamagedJumpDur() const
{
  return player_damaged_jump_dur;
}


const int Settings::getActivityCheckInterval() const
{
  return activity_check_interval;
}


const int Settings::getActivityBufferZone() const
{
  return activity_buffer_zone;
}


const IntRect &Settings::getTextboxTextrect() const
{
  return textbox_textrect;
}


int Settings::getTextboxLineHeight() const
{
  return textbox_line_height;
}


int Settings::getTextboxCharIntervalNormal() const
{
  return textbox_char_interval_normal;
}


int Settings::getTextboxCharIntervalFast() const
{
  return textbox_char_interval_fast;
}


const double &Settings::getTextboxScrollSpeedNormal() const
{
  return textbox_scroll_speed_normal;
}


const double &Settings::getTextboxScrollSpeedFast() const
{
  return textbox_scroll_speed_fast;
}


const double &Settings::getTextboxBoxSlideSpeed() const
{
  return textbox_box_slide_speed;
}


int Settings::getTextboxYIn() const
{
  return textbox_y_in;
}


int Settings::getTextboxYOut() const
{
  return textbox_y_out;
}

int Settings::getTextboxX() const
{
  return textbox_x;
}


int Settings::getTextboxFontSize() const
{
  return textbox_font_size;
}

