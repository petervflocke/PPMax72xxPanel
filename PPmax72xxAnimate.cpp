#include <Arduino.h>
#include "PPmax72xxAnimate.h"


uint8_t PPmax72xxAnimate::count_ch(const String *str, char ch) {
  uint8_t count = 0;

  for (uint16_t i = 0; i < str->length(); i++)
    if (str->charAt(i) == ch) count++;
  return count;
}

String PPmax72xxAnimate::nItem(const String *str, char ch, int item) {
    uint16_t indexS=0;
    uint16_t indexE=0;
    int i = 0;
    int index = 0;
    int newIndex = 0;
    boolean nfound = true;

    indexS = 0;

    while (i < item && nfound) {
      newIndex = str->indexOf(SEPARATOR, index);
      if (newIndex > -1) {
        indexS = index;
        indexE = newIndex;
        index  = newIndex+1;
        i ++;
      }
      else {
        nfound = false;
        indexS = index;
        indexE = str->length();
      }
    }
    return str->substring(indexS, indexE);
}


PPmax72xxAnimate::PPmax72xxAnimate(PPMax72xxPanel *PPMax)
{
  _fsmState   = _INITIALISE;
  _PPMax      = PPMax;
  _tape       = "";
  _textEffect = _NO_EFFECT;
  _textEffectMod = _TO_FULL;
  _tickTime   = 0;
  _xClipS     = 0;
  _xClipE     = _PPMax->width();
  _yClipS     = 0;
  _yClipE     = _PPMax->height();
  _tapeWidth  = 0;
  _tapeHigh   = 0;
  _x          = 0;
  _y          = 0;
  _lineNumber = 1;
  _itemNumber = 0;

}

void PPmax72xxAnimate::setText(String tape, textEffect_t effect, textEffectMod_t mod, uint16_t speed, uint8_t xClipS, uint8_t xClipE) {
  _tape          = tape;
  _textEffect    = effect;
  _textEffectMod = mod;
  _tickTime      = speed;
  _xClipS        = xClipS;
  _xClipE        = xClipE;

  int16_t x1, y1;
  _PPMax->getTextBounds((char *)_tape.c_str(), 0, 0, &x1, &y1, &_tapeWidth, &_tapeHigh);

  _fsmState   = _READY;
}

// Start as soon as the the function fires
boolean PPmax72xxAnimate::Animate(boolean _loop) {

  if (_fsmState == _READY) {
    switch (_textEffect) {
      case _SCROLL_LEFT:
        _x = _xClipE;
        _y = 0;
      break;

      case _SCROLL_RIGHT:
        _x = _xClipS-_tapeWidth;
        _y = 0;
      break;

      case _SCROLL_UP:
        _x = _xClipS;
        _y = _PPMax->height();
      break;

      case _SCROLL_UP_SMOOTH:
        _x = _xClipS;
        _y = 0;
        _lineNumber = 1;
        _itemNumber = count_ch(&_tape, SEPARATOR) +1; //if 0 there is still one
      break;
    }
    _fsmState = _RUN;
    _lastRunTime = millis()-_tickTime;
  }
  if (_fsmState == _END) _fsmState == _DOWN;
  else do {
      if (_fsmState == _RUN && (millis() - _lastRunTime >= _tickTime) ) {
        _lastRunTime = millis();
        _PPMax->setClip(_xClipS, _xClipE, _yClipS, _yClipE);
        //_PPMax->fillScreen(LOW);

        switch (_textEffect) {
          case _SCROLL_LEFT:
            if ( (_x > _xClipS && _textEffectMod == _TO_LEFT) || ( _x+_tapeWidth > _xClipE && _textEffectMod == _TO_RIGHT) || (_x+_tapeWidth > _xClipS && _textEffectMod == _TO_FULL) ) {
              _PPMax->setCursor(_x,_y);
              _PPMax->print(_tape);
              _x --;
              //_PPMax->write();
            } else {
              _PPMax->setCursor(_x,_y);
              _PPMax->print(_tape);
              _fsmState = _END;
            }
            break;

          case _SCROLL_RIGHT:
            if ( (_x < _xClipS && _textEffectMod == _TO_LEFT) || ( _x+_tapeWidth < _xClipE && _textEffectMod == _TO_RIGHT) || (_x < _xClipE && _textEffectMod == _TO_FULL) ) {
              _PPMax->setCursor(_x,_y);
              _PPMax->print(_tape);
              _x ++;
              //_PPMax->write();
            } else {
              _PPMax->setCursor(_x,_y);
              _PPMax->print(_tape);
              _fsmState = _END;
            }
            break;

          case _SCROLL_UP:
            if ( _y > 0 ) {
              _PPMax->fillScreen(LOW);
              _PPMax->setCursor(_x,_y);
              _PPMax->print(_tape);
              _y --;
              //_PPMax->write();
            } else {
              _PPMax->setCursor(_x,_y);
              _PPMax->print(_tape);
              _fsmState = _END;
            }
            break;

          case _SCROLL_UP_SMOOTH:
            int iy = _y;
            for (int i = _lineNumber; i < _lineNumber + 2; i++) {
              _PPMax->setCursor(_x, iy);
              _PPMax->print(nItem(&_tape, SEPARATOR, i));
              iy += _PPMax->height();
            }
            if (--_y < -_PPMax->height()) {
              //matrix1.write();
              _y = 0;
              _lineNumber ++;
              if (_lineNumber >= _itemNumber) {
                _lineNumber = 1;
                _y = 0;
              _fsmState = _END;
              }
             }
            break;

        }
        if (_loop) {
          _PPMax->write();
          delay(_tickTime);
        }
      }
  } while (_loop && (_fsmState != _END) );
  return (_fsmState != _DOWN);
}
