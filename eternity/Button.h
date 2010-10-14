#ifndef BUTTON_H
#define BUTTON_H

#include "UIObject.h"

class Button : public UIObject
{

public:
  Button( int objectId,
          string iText,
          Callback *iCallback,
          D3DCOLOR iTextColor,
          D3DCOLOR iBkgColor,
          int faceFontId,
          float x, float y, float width, float height ) ;

  ~Button() ;

  // A button will notify if there's no
  // click event assigned (it should have one)
  virtual void clicked() override ;

} ;

#endif