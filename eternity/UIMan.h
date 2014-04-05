#ifndef UIMAN_H
#define UIMAN_H

#include <windows.h>
#include <d3d9.h>      // core direct3d
#include <d3dx9.h>     // aux libs
#include <dxerr.h>     // detailed error messages

#include <map>
#include <string>
#include <ctype.h>
using namespace std ;

#include "helperFunctions.h"
#include "Callback.h"


// UIMan aka UI-Manager.

#include "UIObject.h"
#include "TextField.h"
#include "Button.h"

typedef map<int, UIObject*> UIObjectMap ;
typedef map<int, UIObject*>::iterator UIObjectMapIter ;

typedef map<int, Button*> ButtonMap ;
typedef map<int, Button*>::iterator ButtonMapIter ;

typedef map<int, TextField*> TextFieldMap ;
typedef map<int, TextField*>::iterator TextFieldMapIter ;

class SpriteMan;

class UIMan
{
protected:
  ButtonMap buttons ;
  TextFieldMap textfields ;
  UIObject* activeObject ; // reference to the active object

public:
  UIMan() ;

  void createButton(
    /// Integer ID by which you'll refer to this button in the future
    int buttonId,
    string iText,

    /// Function that runs when the button is pushed
    Callback *iCallback,
    D3DCOLOR textColor,
    D3DCOLOR backgroundColor,
    int fontId,
    float x, float y,
    float width, float height ) ;

  void createButtonSprite(
    /// Integer ID by which you'll refer to this button in the future
    int buttonId,
    string iText,
    Callback *iCallback,

    /// The sprite id
    int backgroundSpriteId,
    D3DCOLOR spriteModulatingColor,

    D3DCOLOR textColor,

    /// This is still specified in case
    /// your sprite has like, alpha-"holes" and
    /// you want to fill in the background color
    D3DCOLOR backgroundColor,

    int fontId,
    float x, float y,
    float width, float height ) ;

  void createTextField(
    /// Integer ID by which you'll refer to this textField in the future
    int textFieldId,
    TextField::TextEntryType iTextEntryType,
    string iText,
    Callback *iCallback,
    D3DCOLOR textColor,
    D3DCOLOR backgroundColor,
    int fontId,
    float x, float y,
    float width, float height ) ;

  void createTextFieldWithLabel(
    int textFieldId,
    TextField::TextEntryType iTextEntryType,
    string iText,
    string iLabelText,
    Callback *iCallback,
    D3DCOLOR textColor,
    D3DCOLOR backgroundColor,
    int fontId,
    float x, float y,
    float width, float height ) ;

  void createTextFieldSprite(

    /// Integer ID by which you'll refer to this textfield in the future
    int textFieldId,
    
    TextField::TextEntryType iTextEntryType,

    string iText,

    /// Callback that occurs when text field is
    /// clicked on or text is entered
    Callback *iCallback,

    /// The sprite id
    int backgroundSpriteId,
    D3DCOLOR spriteModulatingColor,

    D3DCOLOR textColor,

    /// This is still specified in case
    /// your sprite has like, alpha-"holes" and
    /// you want to fill in the background color
    D3DCOLOR backgroundColor,

    int fontId,
    float x, float y,
    float width, float height
  ) ;

  void hitTestUIObjects( int x, int y ) ;

  void removeDeletedObjects() ;

  /// Send this object a character.
  void sendCharacter( char c ) ;

  Button* getButton( int buttonId ) ;

  TextField* getTextField( int textFieldId ) ;

  void deleteButton( int buttonId ) ;

  void deleteTextField( int textFieldId ) ;

  void draw(SpriteMan *spriteMan) ;
} ;


#endif