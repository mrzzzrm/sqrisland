#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP

  #include "framework.hpp"

  #include "./Settings.hpp"


  enum TEXTBOX_STATE
  {
    TEXTBOX_INACTIVE,

    TEXTBOX_SLIDING_IN,
    TEXTBOX_SLIDING_OUT,

    TEXTBOX_WRITING,
    TEXTBOX_SCROLLING,
    TEXTBOX_WAITING,
    TEXTBOX_FINISHED
  };


  class Textbox
  {
    public:

      static void loadStaticData();
      static void clearStaticData();


      Textbox(const util::Timer &timer);
      ~Textbox();

      TEXTBOX_STATE getState() const;
      void setText(const std::string &text);
      void setPosition(const geo::FloatPoint &position);

      void sigContinue();
      void sigSetFast(bool is);
      void sigAbort();

      void start();
      void update();
      void draw();


    private:

      /* Static data */
      struct static_data
      {
        static font::TtfFont font;
        static gfx::Animation animation;
        static geo::IntRect textrect;
        static int line_height;
        static int max_linecount;
        static int char_interval_normal, char_interval_fast;
        static double scroll_speed_normal, scroll_speed_fast;
        static double box_slide_speed;
        static int x, y_in, y_out;
      };

      const util::Timer &timer;


      std::vector<std::vector<std::string> > blocks;
      std::list<std::list<gfx::Image*> > lines;

      TEXTBOX_STATE state;

      int current_block;
      int current_word;
      int current_char;
      std::string current_line;

      bool fast;
      time_t last_char_drawn;
      int last_char_right_x;

      double scroll_offset;

      geo::FloatPoint position;


      /* Update functions */
      void UpdateSlidingIn();
      void UpdateSlidingOut();
      void UpdateWriting();
      void UpdateWaiting();
      void UpdateScrolling();
      void UpdateFinished();

  };

#endif // TEXTBOX_HPP
