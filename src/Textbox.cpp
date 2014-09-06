#include "./Textbox.hpp"

using namespace std;
using namespace geo;
using namespace font;
using namespace gfx;
using namespace etc;
using namespace util;



TtfFont Textbox::static_data::font;
Animation Textbox::static_data::animation;
IntRect Textbox::static_data::textrect;
int Textbox::static_data::line_height;
int Textbox::static_data::max_linecount;
int Textbox::static_data::char_interval_normal;
int Textbox::static_data::char_interval_fast;
double Textbox::static_data::scroll_speed_normal;
double Textbox::static_data::scroll_speed_fast;
double Textbox::static_data::box_slide_speed;
int Textbox::static_data::x;
int Textbox::static_data::y_in;
int Textbox::static_data::y_out;

void Textbox::loadStaticData()
{
  Settings settings;

  settings.load();

  static_data::font.load("dat/fonts/ALEAWBB_.TTF", settings.getTextboxFontSize());
  static_data::animation.load("dat/interface/textbox.ani");
  static_data::line_height = settings.getTextboxLineHeight();
  static_data::char_interval_normal = settings.getTextboxCharIntervalNormal();
  static_data::char_interval_fast = settings.getTextboxCharIntervalFast();
  static_data::scroll_speed_normal = settings.getTextboxScrollSpeedNormal();
  static_data::scroll_speed_fast = settings.getTextboxScrollSpeedFast();
  static_data::box_slide_speed = settings.getTextboxBoxSlideSpeed();
  static_data::x = settings.getTextboxX();
  static_data::y_in = settings.getTextboxYIn();
  static_data::y_out = settings.getTextboxYOut();


  static_data::textrect = settings.getTextboxTextrect();
  if(static_data::line_height != 0)
    static_data::max_linecount = static_data::textrect.getHeight() / static_data::line_height;
  else
    static_data::max_linecount = 0;
}


void Textbox::clearStaticData()
{
  static_data::font.clear();
  static_data::animation.clear();
  static_data::textrect.clear();
}


Textbox::Textbox(const util::Timer &timer):
  timer(timer)
{
  state = TEXTBOX_INACTIVE;
  current_char = 0;
  current_block = 0;
  current_word = 0;
  fast = false;
  last_char_drawn = 0;
  last_char_right_x = 0;
  scroll_offset = 0;

  position.load(static_data::x, static_data::y_out);
}


Textbox::~Textbox()
{

}


TEXTBOX_STATE Textbox::getState() const
{
  return state;
}


void Textbox::setText(const std::string &text)
{
  vector<string> uf_blocks;


  uf_blocks = string_split_to_vector(text, "[b]");
  blocks.resize(uf_blocks.size());

  for(int b = 0; b < (int)uf_blocks.size(); b++)
  {
    blocks[b] = string_split_to_vector(uf_blocks[b], " ");
  }
}


void Textbox::setPosition(const FloatPoint &position)
{
  this->position = position;
}


void Textbox::sigContinue()
{
  if(state == TEXTBOX_WAITING)
  {
    state = TEXTBOX_SCROLLING;

    /* New line */
    lines.push_back(list<Image*>());
    last_char_right_x = 0;
  }
  else if(state == TEXTBOX_FINISHED)
  {
    state = TEXTBOX_SLIDING_OUT;
  }
}


void Textbox::sigSetFast(bool is)
{
  fast = is;
}


void Textbox::sigAbort()
{
  state = TEXTBOX_SLIDING_OUT;
}


void Textbox::start()
{
  state = TEXTBOX_SLIDING_IN;
  current_char = 0;
  current_block = 0;
  current_word = 0;
  fast = false;
  last_char_drawn = 0;
  last_char_right_x = 0;

  /* Clear images */
  for(list<list<Image*> >::iterator i = lines.begin(); i != lines.end(); i++)
  {
    list_delete_all_and_clear(*i);
  }
  lines.clear();
}


void Textbox::update()
{
  switch(state)
  {
    case TEXTBOX_SLIDING_IN:  UpdateSlidingIn();  break;
    case TEXTBOX_SLIDING_OUT: UpdateSlidingOut(); break;
    case TEXTBOX_WRITING:     UpdateWriting();     break;
    case TEXTBOX_SCROLLING:   UpdateScrolling();   break;
    case TEXTBOX_WAITING:     UpdateWaiting();     break;
    case TEXTBOX_FINISHED:    UpdateFinished();    break;

    case TEXTBOX_INACTIVE: break;
  }
}


void Textbox::draw()
{
  if(state != TEXTBOX_INACTIVE)
  {
    int l;

    static_data::animation.draw(position);

    l = 0;
    for(list<list<Image*> >::iterator li = lines.begin(); li != lines.end(); li++, l++)
    {
      int x = 0;
      for(list<Image*>::iterator ci = li->begin(); ci != li->end(); ci++)
      {
        Image *c = *ci;

        if(c == NULL)
        {
          x += static_data::font.getCharSize(' ').getWidth();
        }
        else
        {
          int y;
          IntRect rect;

          if(l == 0)
          {
            rect.load(0, scroll_offset, c->getSize().getWidth(), c->getSize().getHeight() - scroll_offset);
            y = static_data::textrect.getY() + l * static_data::line_height;
          }
          else
          {
            rect.load(0, 0, -1, -1);
            y = static_data::textrect.getY() - scroll_offset + l * static_data::line_height;
          }

          c->draw(position + IntVector(static_data::textrect.getX() + x, y), ALIGN_BY_CORNER, rect);
          x += c->getSize().getWidth();
        }
      }
    }
  }

}


void Textbox::UpdateSlidingIn()
{
  position.decY(timer.getPerSecond(static_data::box_slide_speed));
  if(position.getY() <= static_data::y_in)
  {
    position.setY(static_data::y_in);
    state = TEXTBOX_WRITING;
  }
}


void Textbox::UpdateSlidingOut()
{
  position.incY(timer.getPerSecond(static_data::box_slide_speed));
  if(position.getY() >= static_data::y_out)
  {
    position.setY(static_data::y_out);
    state = TEXTBOX_INACTIVE;

  }
}


void Textbox::UpdateWriting()
{
  /* Put next char */
  if(
     (fast && (timer.getTicks() - last_char_drawn >= static_data::char_interval_fast)) ||
     (!fast && (timer.getTicks() - last_char_drawn >= static_data::char_interval_normal))
    )
    {
      char c;
      bool newline;

      /* No blocks no chars */
      if(blocks.size() == 0)
      {
        state = TEXTBOX_FINISHED;
        return;
      }


      /* Get char */
      c = blocks[current_block][current_word][current_char];


      /* Check if a new word would still fit in line */
      if(current_char == 0)
      {
        int x = last_char_right_x;

        for(int c = 0; c < (int)blocks[current_block][current_word].size(); c++)
        {
          x += static_data::font.getCharSize(blocks[current_block][current_word][c]).getWidth();
        }

        newline = x >= static_data::textrect.getWidth();
      }
      else
      {
        newline = false;
      }

      /* New line */
      if(newline || lines.size() == 0)
      {
        lines.push_back(list<Image*>());
        last_char_right_x = 0;

        /* Scroll */
        if((int)lines.size() > static_data::max_linecount)
        {
          state = TEXTBOX_SCROLLING;
          return;
        }
      }

      last_char_right_x += static_data::font.getCharSize(c).getWidth();



      /* Put char */
      lines.back().push_back(static_data::font.render(char_to_string(c), BLACK, TTF_BLENDED));


      /* Update cursors of char, word and block */
      last_char_drawn = timer.getTicks();
      current_char++;
      if(current_char >= (int)blocks[current_block][current_word].size())
      {
        current_char = 0;
        current_word++;
        lines.back().push_back(NULL); // Space
        last_char_right_x += static_data::font.getCharSize(' ').getWidth(); // Space
      }
      if(current_word >= (int)blocks[current_block].size())
      {
        current_word = 0;
        current_block++;
        state = TEXTBOX_WAITING;
      }
      if(current_block >= (int)blocks.size())
      {
        current_block = 0;
        state = TEXTBOX_FINISHED;
      }
    }
}


void Textbox::UpdateWaiting()
{
  if(fast)
  {
    state = TEXTBOX_SCROLLING;

    /* New line */
    lines.push_back(list<Image*>());
    last_char_right_x = 0;
  }
}


void Textbox::UpdateScrolling()
{
    scroll_offset += timer.getPerSecond(fast ? static_data::scroll_speed_fast : static_data::scroll_speed_normal);

    if(scroll_offset >= static_data::line_height)
    {
      list_delete_all_and_clear(lines.front());
      lines.erase(lines.begin());

      scroll_offset = 0;
      state = TEXTBOX_WRITING;
    }
}


void Textbox::UpdateFinished()
{

}


