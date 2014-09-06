#include "./Textbox.hpp"

using namespace std;


namespace event
{
  Textbox::Textbox(EVENT_PLAYTYPE playtype, EVENT_STARTTYPE starttype, string text):
    Event(EVENT_TEXTBOX, playtype, starttype)
  {
    this->text = text;
  }

  Textbox::~Textbox()
  {

  }

  const string &Textbox::getText() const
  {
    return text;
  }
}
