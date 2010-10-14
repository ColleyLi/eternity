#include "Button.h"

Button::Button( int objectId,
        string iText,
        Callback *iCallback,
        D3DCOLOR iTextColor,
        D3DCOLOR iBkgColor,
        int faceFontId,
        float x, float y, float width, float height ) :
  UIObject( objectId, iText, iCallback,
            iTextColor, iBkgColor,
            faceFontId, x, y,
            width, height )
{
    
}

Button::~Button()
{
  // nothing to do in dtor
}

// A button will notify if there's no
// click event assigned (it should have one)
void Button::clicked() // override
{
  if( callback )
    callback->exec() ;
  else
    warning( "No callback assigned" ) ;
}