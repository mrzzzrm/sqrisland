#ifndef SETTINGS_HPP
#define SETTINGS_HPP

  #include "framework.hpp"


  class Settings
  {
    public:

      Settings();
      Settings(const Settings &settings);
      ~Settings();


      void load();


      void multiplyTileBasedSpeeds(double fac);


      const double &getGravityAcc() const;
      const double &getGravityMax() const;

      const double &getCameraSpeed() const;

      const double &getPlayerSpeed() const;
      const int &getPlayerJumpDur() const;
      const double &getPlayerJumpForce() const;
      const int &getPlayerOnEnemyJumpDur() const;
      const int &getPlayerSpringJumpMaxDelay() const;
      const int &getPlayerDamagedJumpDur() const;

      const int getActivityCheckInterval() const;
      const int getActivityBufferZone() const;

      const geo::IntRect &getTextboxTextrect() const;
      int getTextboxLineHeight() const;
      int getTextboxCharIntervalNormal() const;
      int getTextboxCharIntervalFast() const;
      const double &getTextboxScrollSpeedNormal() const;
      const double &getTextboxScrollSpeedFast() const;
      const double &getTextboxBoxSlideSpeed() const;
      int getTextboxYIn() const;
      int getTextboxYOut() const;
      int getTextboxX() const;
      int getTextboxFontSize() const;


    private:

      /* Data */
      double gravity_acc;
      double gravity_max;

      double camera_speed;

      double player_speed;
      int player_jump_dur;
      double player_jump_force;
      int player_on_enemy_jump_dur;
      int player_spring_jump_max_delay;
      int player_damaged_jump_dur;

      int activity_check_interval;
      int activity_buffer_zone;

      geo::IntRect textbox_textrect;
      int textbox_line_height;
      int textbox_char_interval_normal;
      int textbox_char_interval_fast;
      double textbox_scroll_speed_normal;
      double textbox_scroll_speed_fast;
      double textbox_box_slide_speed;
      int textbox_y_in;
      int textbox_y_out;
      int textbox_x;
      int textbox_font_size;


  };

#endif // SETTINGS_HPP
