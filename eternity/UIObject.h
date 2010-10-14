#ifndef UIOBJECT_H
#define UIOBJECT_H

#include "AABB.h"
#include "Callback.h"
#include "helperFunctions.h"

#include <string>
using namespace std ;

class UIObject : public AABB2
{
protected:
  int id ; // retain id

  Callback *callback ; // the callback function
  // usually associated with a click.  Eventually
  // this can be expanded to include different
  // sorts of events like hover etc.

  /// Marked for deletion, happens
  /// when designer wants to delete this element.
  bool deleted ;

public:
  string text ; // usually the text on the face of the UIObject

  // Usually every UIObject has 2 colors
  // at least:  bkg color and text color
  D3DCOLOR textColor, bkgColor ;

  /// The font to use when drawing
  int fontId ;

  /// The background sprite id to use
  /// when drawing this object.  If its
  /// 0, then no sprite is used.
  int backgroundSpriteId ;

  D3DCOLOR spriteModulatingColor ;

public:
  UIObject( int objectId,
            string iText,
            Callback *iCallback,
            D3DCOLOR iTextColor,
            D3DCOLOR iBkgColor,
            int faceFontId,
            float x, float y, float width, float height ) ;

  ~UIObject() ;

  void setBackgroundSprite( int spriteId, D3DCOLOR iModulatingColor ) ;

  virtual void clicked() ;

  //!!Should we override operator [delete]?
  void Delete() ;

  bool isDeleted() ;

  /// We could have a virtual function here to
  /// draw the objects, but that's not done from here.
} ;

#endif