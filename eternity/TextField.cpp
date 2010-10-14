#include "Textfield.h"


TextField::TextField( int objectId,
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
void TextField::ban( char c )
{
  banned.push_back( c ) ;
}

bool TextField::isBanned( char c )
{
  for( UINT i = 0 ; i < banned.size() ; i++ )
    if( banned[i] == c )
      return true ;

  return false ;
}

bool TextField::isAllowed( char c )
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
void TextField::sendCharacter( char c )
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
