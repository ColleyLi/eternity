#ifndef BUTTONMAN_H
#define BUTTONMAN_H

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

//!! this is really general AABB intersect code.
class AABB2
{
protected:
  /// top left corner of this UIObject
  D3DXVECTOR2 topLeft ;

  /// Width is x, size is y.
  /// That's pretty intuitive, actually
  D3DXVECTOR2 size ;

public:
  AABB2( float x, float y, float width, float height ) :
      topLeft( x, y ), size( width, height )
  {

  }

  float left()
  {
    return topLeft.x ;
  }

  float right()
  {
    return topLeft.x + size.x ;
  }

  float top()
  {
    return topLeft.y ;
  }

  float bottom()
  {
    return topLeft.y + size.y ;
  }

  float width()
  {
    return size.x ;
  }

  float height()
  {
    return size.y ;
  }

  bool intersects( float x, float y )
  {
    /*

    +-------+
    | this  |
    |      *|
    +-------+

    */
    // exclusive test.
    if( x > left() && x < right() && 
        y > top() && y < bottom() )
      return true ;
    else
      return false ;
  }

  bool intersects( AABB2 other )
  {
    /*

    +-------+
    | this  |
    |    +------+
    +----|--+   |
         | other|
         +------+

    */
    
    if( other.top() > this->bottom() ||
        this->top() > other.bottom() ||
        
        other.left() > this->right() ||
        this->left() > other.right() )
    {
      // They cannot intersect
      return false ;
    }
    else
      return true ; // they must intersect
      
  }
} ;

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
            float x, float y, float width, float height ) :
    id( objectId ),
    text( iText ),
    callback(iCallback),
    textColor( iTextColor ),
    bkgColor(iBkgColor),
    fontId( faceFontId ),
    AABB2( x, y, width, height )
  {
    deleted = false ;
    backgroundSpriteId = 0 ;
  }

  ~UIObject()
  {
    DESTROY( callback ) ;
  }

  void setBackgroundSprite( int spriteId, D3DCOLOR iModulatingColor )
  {
    backgroundSpriteId = spriteId ;
    spriteModulatingColor = iModulatingColor ;
  }

  virtual void clicked()
  {
    // Execute default click action
    if( callback )
      callback->exec() ;
  }

  //!!Should we override operator [delete]?
  void Delete()
  {
    deleted = true ;
  }

  bool isDeleted()
  {
    return deleted ;
  }

  /// We could have a virtual function here to
  /// draw the objects, but that's not done from here.
} ;

class Button : public UIObject
{

public:
  Button( int objectId,
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

  ~Button()
  {
    // nothing to do in dtor
  }

  // A button will notify if there's no
  // click event assigned (it should have one)
  virtual void clicked() override
  {
    if( callback )
      callback->exec() ;
    else
      warning( "No callback assigned" ) ;
  }

} ;

class TextField : public UIObject
{

  /// The "blacklist" of characters
  /// not allowed in this textField.
  vector<char> banned ;

public:
  string labelText ;

  /// The type of text this text field accepts.
  /// Make a combination with bitwise OR |
  /// E.g. Numeric | Alphabet means 
  /// any alphabet letters or numeric digits
  /// will be accepted
  /// AnyText means everything is accepted
  enum TextEntryType
  {
    /// Integer numbers only
    Integer        = 1 << 0,

    /// Numbers and the decimal point
    Numeric        = 1 << 1,

    /// abcdefgh
    AlphaLower     = 1 << 2,

    // ABCDEFGH
    AlphaUpper     = 1 << 3,

    /// . , : ; ' " !@#$%^&*()<>?{}|\-+=
    /// (note Numeric also includes ".")
    Punctuation    = 1 << 4,

    AnyText        = 0xffff // all bits on
  } ;

  TextEntryType textEntryType ;

  TextField( int objectId,
             TextEntryType iTextEntryType,
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
    textEntryType = iTextEntryType ;
  }

  /// Ban a specific character from
  /// being allowed in this field
  void ban( char c )
  {
    banned.push_back( c ) ;
  }

  bool isBanned( char c )
  {
    for( UINT i = 0 ; i < banned.size() ; i++ )
      if( banned[i] == c )
        return true ;

    return false ;
  }

  bool isAllowed( char c )
  {
    // based on flags set
    if( textEntryType & TextEntryType::Integer )
      // integers are allowed, so
      // if its numeric then its allowed
      if( isdigit( c ) )
        return true ;

    // So either numbers aren't allowed,
    // or they are allowed but what was
    // entered just wasn't a number
    if( textEntryType & TextEntryType::Numeric )
    {
      if( isdigit(c) )
        return true ;
      else if( c == '.' )
      {
        // Only allowed if there isn't a . there already
        if( text.find( '.' ) == -1 )
          return true ;
        else
          info( "There already is a decimal in this number" ) ;
      }
    }


    if( textEntryType & TextEntryType::AlphaLower )
      if( islower(c) )
        return true ;

    if( textEntryType & TextEntryType::AlphaUpper )
      if( isupper(c) )
        return true ;

    if( textEntryType & TextEntryType::Punctuation )
      if( ispunct(c) )
        return true ;

    return false ; // not allowed
  }

  /// 
  void sendCharacter( char c )
  {
    // Append the character to its collection,
    // unless its backspace or delete
    if( c == 8/*backspace*/ || c == 127 /*delete*/ )
    {
      if( text.length() > 0 )
       text = text.substr( 0, text.length()-1 ) ;
    }
    else if( c == 13 || c == 10 )
    {
      // carriage return / newline
      // (( do nothing ))
    }
    else
    {
      // First, check, based on the flags set
      // if this character will be allowed
      if( isAllowed( c ) )
      {
        // Final check if its in the "blacklist" of
        // characters before adding.
        // I was going to actually ONLY use a banlist
        // but that makes every add definitely cause
        // linear search (but I'd try and allieviate that
        // by putting the alphabetic letters in the order
        // that they'd like occur e.g. earstl....
        if( !isBanned( c ) )
        {
          text += c ;
        }
      }
    }
  }
} ;

typedef map<int, UIObject*> UIObjectMap ;
typedef map<int, UIObject*>::iterator UIObjectMapIter ;

typedef map<int, Button*> ButtonMap ;
typedef map<int, Button*>::iterator ButtonMapIter ;

typedef map<int, TextField*> TextFieldMap ;
typedef map<int, TextField*>::iterator TextFieldMapIter ;




class ButtonMan
{
protected:
  ButtonMap buttons ;
  TextFieldMap textfields ;
  UIObject* activeObject ; // reference to the active object

public:
  ButtonMan()
  {
    activeObject=0;
  }
  void createButton(
    /// Integer ID by which you'll refer to this button in the future
    int buttonId,
    string iText,
    Callback *iCallback,
    D3DCOLOR textColor,
    D3DCOLOR backgroundColor,
    int fontId,
    float x, float y,
    float width, float height )
  {
    Button *b = new Button( buttonId,
      iText, iCallback,
      textColor, backgroundColor,
      fontId, x, y, width, height
    ) ;
    buttons.insert( make_pair( buttonId, b ) ) ;
  }

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
    float width, float height )
  {
    Button *b = new Button( buttonId,
      iText, iCallback,
      textColor, backgroundColor,
      fontId, x, y, width, height
    ) ;
    if( !backgroundSpriteId )
    {
      warning( "backgroundSpriteId is 0!  Your background sprite "
        "WILL NOT be drawn.  You should use a positive number "
        "for your spriteId's, "
        "like 1 or 2 or 3." ) ;
    }
    b->setBackgroundSprite( backgroundSpriteId, spriteModulatingColor ) ;

    buttons.insert( make_pair( buttonId, b ) ) ;
  }

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
    float width, float height )
  {
    TextField *tf = new TextField(
      textFieldId,
      iTextEntryType,
      iText, iCallback,
      textColor, backgroundColor,
      fontId, x, y, width, height
    ) ;
    textfields.insert( make_pair( textFieldId, tf ) ) ;
  }

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
    float width, float height )
  {
    TextField *tf = new TextField(
      textFieldId,
      iTextEntryType,
      iText, iCallback,
      textColor, backgroundColor,
      fontId, x, y, width, height
    ) ;

    tf->labelText = iLabelText ;
    textfields.insert( make_pair( textFieldId, tf ) ) ;
  }

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
  )
  {
    TextField *tf = new TextField(
      textFieldId,
      iTextEntryType,
      iText, iCallback,
      textColor, backgroundColor,
      fontId, x, y, width, height
    ) ;

    if( !backgroundSpriteId )
    {
      warning( "backgroundSpriteId is 0!  Your background sprite "
        "WILL NOT be drawn.  You should use a positive number "
        "for your spriteId's, "
        "like 1 or 2 or 3." ) ;
    }
    tf->setBackgroundSprite( backgroundSpriteId, spriteModulatingColor ) ;

    textfields.insert( make_pair( textFieldId, tf ) ) ;
  }

  void hitTestUIObjects( int x, int y )
  {
    // Start by assuming nothing was clicked on
    activeObject = NULL ;

    foreach( ButtonMapIter, buttonIter, buttons )
    {
      Button *b = buttonIter->second ;
      if( b->intersects( x, y ) )
      {
        b->clicked() ;

        // this one is now active
        activeObject = b ;
      }
    }

    foreach( TextFieldMapIter, textFieldIter, textfields )
    {
      TextField *tf = textFieldIter->second ;
      if( tf->intersects( x, y ) )
      {
        tf->clicked() ;
        activeObject = tf ;
      }
    }

    removeDeletedObjects() ;
  }

  void removeDeletedObjects()
  {
    foreach( ButtonMapIter, buttonIter, buttons )
    {
      if( buttonIter->second->isDeleted() )
      {
        // If the object being deleted
        // was the activeObject..
        if( buttonIter->second == activeObject )
        {
          // make the activeObject NOTHING.
          // (could make it "next object", but won't)
          activeObject = NULL ;
        }
        DESTROY( buttonIter->second ) ;
        buttonIter = buttons.erase( buttonIter ) ;
        if( buttonIter == buttons.end() )
        {
          //puts( "END OF LIST" ) ;
          // If the last button in the list
          // is removed, we can no longer
          // do the iter++ operation
          // that the foreach would try to do.
          break;
        }
      }
    }

    foreach( TextFieldMapIter, textfieldIter, textfields )
    {
      if( textfieldIter->second->isDeleted() )
      {
        DESTROY( textfieldIter->second ) ;
        textfieldIter = textfields.erase( textfieldIter ) ;
        if( textfieldIter == textfields.end() )
          break;
      }
    }
  }

  /// Send this object a character.
  void sendCharacter( char c )
  {
    // Pass it on to the active field
    // if its a text field
    if( activeObject )
    {
      TextField *tf = dynamic_cast<TextField*>( activeObject ) ;
      if( tf )
      {
        tf->sendCharacter( c ) ;
        if( c == 13 || c == 10 )
        {
          // This is a signal to ButtonMan,
          // if the selected object is a textfield,
          // that entry is finished.
          // 
          activeObject=NULL;
        }
        else if( c == 9 )
        {
          // Tab over to next component,
          // if at the last one, then loop
          // back to beginning
          info( "Next component" ) ;
          
          //!! for now its just the next
          // text field.  ultimately though
          // i think we'll need to maintain
          // an ADDITIONAL list of UIObject*
          // so that we can tab over to the
          // next UIObject*, in order.

          foreach( TextFieldMapIter, tfIter, textfields )
          {
            if( tfIter->second == tf )
            {
              // This is the place where
              // this text field is.  Now,
              // point ot hte Next one, if 
              // exists, else, point to begin()
              ++tfIter ;

              if( tfIter != textfields.end() )
              {
                //info( "Selecting next component" ) ;
                activeObject = tfIter->second ;
              }
              else
              {
                //info( "No next component, begin()" ) ;
                // next last one doesn't exist, so
                // point to first one
                activeObject = textfields.begin()->second ;

                // AVOID INCREMENTING THE
                // ITER AGAIn
                break;
              }
            }
          }
        }
      }
    }
  }

  Button* getButton( int buttonId )
  {
    ButtonMapIter buttonEntry ;
    buttonEntry = buttons.find( buttonId ) ;
    if( buttonEntry == buttons.end() )
    {
      error( "Could not find button %d", buttonId );
      return NULL ;
    }
    else
    {
      return buttonEntry->second ;
    }
  }

  TextField* getTextField( int textFieldId )
  {
    TextFieldMapIter tfEntry ;
    tfEntry = textfields.find( textFieldId ) ;
    if( tfEntry == textfields.end() )
    {
      error( "Could not find textfield %d", textFieldId );
      return NULL ;
    }
    else
    {
      return tfEntry->second ;
    }
  }

  void deleteButton( int buttonId )
  {
    Button* button = getButton( buttonId );
    if( button )
      button->Delete() ;

    // The reason we call this function
    // and not remove the button straight away
    // is because it is possible this function
    // get called WHILE WE ARE ITERATING OVER
    // THE 'buttons' map (e.g. if we were 
    // walking through buttons doing hittest on each..
    // one that got hit wants to delete.. now
    // we have a problem).

    // This ALSO gives a nice property that
    // to delete a button you can simply CALL
    // DELETE ON IT, instead of having to
    // call it from Window() all the time
  }

  void deleteTextField( int textFieldId )
  {
    TextField* tf = getTextField(textFieldId);
    if( tf )
      tf->Delete() ;
  }
} ;


#endif