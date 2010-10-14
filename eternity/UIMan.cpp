#include "UIMan.h"

UIMan::UIMan()
{
  activeObject=0;
}

void UIMan::createButton(
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

void UIMan::createButtonSprite(
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

void UIMan::createTextField(
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

void UIMan::createTextFieldWithLabel(
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

void UIMan::createTextFieldSprite(

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

void UIMan::hitTestUIObjects( int x, int y )
{
  // Start by assuming nothing was clicked on

  // need to call activeObject's "deselected" function
  if( activeObject )
  {
    //!! For now call clicked(),
    // which its its basic action function
    activeObject->clicked() ;
    activeObject = NULL ;
  }

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

void UIMan::removeDeletedObjects()
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
void UIMan::sendCharacter( char c )
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
        // This is a signal to UIMan,
        // if the selected object is a textfield,
        // that entry is finished.
        // 
        // !! need to call deselected, call clicked() for now
        activeObject->clicked() ; 
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

Button* UIMan::getButton( int buttonId )
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

TextField* UIMan::getTextField( int textFieldId )
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

void UIMan::deleteButton( int buttonId )
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

void UIMan::deleteTextField( int textFieldId )
{
  TextField* tf = getTextField(textFieldId);
  if( tf )
    tf->Delete() ;
}

void UIMan::draw()
{
  foreach( ButtonMapIter, buttonIter, buttons )
  {
    Button *b = buttonIter->second ;

    // Draw the background box
    drawBox( b->bkgColor, b->left(), b->top(), b->width(), b->height() ) ;

    // Only draw the background sprite if
    // it is not 0.  0 is reserved for
    // meaning.. "no sprite"
    if( b->backgroundSpriteId )
      drawSprite( b->backgroundSpriteId,
        b->left(), b->top(),
        b->width(), b->height(), 0.0f,
        b->spriteModulatingColor,
        SpriteCentering::TopLeft ) ;
  
    // Draw the text inside it
    drawString(
      b->fontId,
      b->text.c_str(),
      b->textColor,
      b->left(), b->top(), b->width(), b->height(),
      DT_CENTER | DT_VCENTER ) ;
  }

  foreach( TextFieldMapIter, tfIter, textfields )
  {
    TextField *tf = tfIter->second ;

    // Draw the background box
    drawBox( tf->bkgColor, tf->left(), tf->top(), tf->width(), tf->height() ) ;

    // Only draw the background sprite if
    // it is not 0.  0 is reserved for
    // meaning.. "no sprite"
    if( tf->backgroundSpriteId )
      drawSprite( tf->backgroundSpriteId,
        tf->left(), tf->top(),
        tf->width(), tf->height(), 0.0f,
        tf->spriteModulatingColor,
        SpriteCentering::TopLeft ) ;
  
    // Draw the text inside it

    // if it has a label, draw the label
    // in the center left, and the text
    // center right.  Else draw the text
    // center
    if( tf->labelText.length() )
    {
      drawString(
        tf->fontId,
        tf->labelText.c_str(),
        tf->textColor,
        tf->left(), tf->top(), tf->width(), tf->height(),
        DT_LEFT | DT_VCENTER ) ;

      drawString(
        tf->fontId,
        tf->text.c_str(),
        tf->textColor,
        tf->left(), tf->top(), tf->width(), tf->height(),
        DT_RIGHT | DT_VCENTER ) ;     
    }
    else
    {
      // no label, so draw the text field of
      // the text field centered
      //!! THIS SHOULD BE A SETTABLE PARAM AS WELL.
      drawString(
        tf->fontId,
        tf->text.c_str(),
        tf->textColor,
        tf->left(), tf->top(), tf->width(), tf->height(),
        DT_CENTER | DT_VCENTER ) ;
    }
    // if this text field is
    // the active object, draw the caret
    // inside of it
    if( tf == activeObject )
    {
      // (for now just draw a black box
      // in its top right corner)
      drawBox( D3DCOLOR_ARGB( 180, 255, 0, 0 ),
        tf->right() - 4 - 2,
        tf->top()+2,
        4, 4 ) ;
    }
  }
}