#include <SPI.h>
#include <Adafruit_GFX.h>
#include <PPMax72xxPanel.h>

#define SEPARATOR '\n'

class PPmax72xxAnimate;

enum textEffect_t
{
  _NO_EFFECT,           // Used as a place filler, executes no operation
  _PRINT,               // Text just appears (printed)
  _SCROLL_UP,           // Text scrolls up through the display
  _SCROLL_DOWN,         // Text scrolls down through the display
  _SCROLL_LEFT,         // Text scrolls right to left on the display, allign to the first left character
  _SCROLL_RIGHT,         // Text scrolls left to right on the display
  _SCROLL_UP_SMOOTH,    // Text scrolls up through the display
};

enum textEffectMod_t
{
  _NONE_MOD,
  _TO_LEFT,
  _TO_RIGHT,
  _TO_FULL,
};

class PPmax72xxAnimate {

  private:
    PPMax72xxPanel *_PPMax;
    enum fsmState_t
    {
      _INITIALISE,     // Initialize all variables
      _READY,          // Ready for animation
      _RUN,            // During animation
      _END             // Display cycle has completed
    };
    fsmState_t      _fsmState;

    textEffect_t    _textEffect;
    textEffectMod_t _textEffectMod;

    bool      _suspend;     // don't do anything
    uint32_t  _lastRunTime; // the millis() value for when the animation was last run
    uint16_t  _tickTime;    // the time between animations in milliseconds

    String _tape;

    uint8_t _xClipS;
    uint8_t _xClipE;
    uint8_t _yClipS;
    uint8_t _yClipE;
    int16_t _x;
    int16_t _y;
    uint16_t _tapeWidth;
    uint16_t _tapeHigh;
    int16_t _lineNumber;
    uint8_t _itemNumber;

  public:
    PPmax72xxAnimate(PPMax72xxPanel *PPMax);
    void setText(String tape, textEffect_t effect, textEffectMod_t mod, uint16_t speed, uint8_t xClipS, uint8_t xClipE);
    boolean Animate(boolean _loop);
    inline boolean AnimateStatus() {return _fsmState==_END;};
    inline void Reset() {_fsmState = _READY;};

    uint8_t count_ch(const String *str, char ch);
    String nItem(const String *str, char ch, int item);

};
