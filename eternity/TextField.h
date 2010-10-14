#ifndef TEXTFIELD_H
#define TEXTFIELD_H

#include "UIObject.h"
#include <string>
#include <vector>
using namespace std ;


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
             float x, float y, float width, float height ) ;

  /// Ban a specific character from
  /// being allowed in this field
  void ban( char c ) ;

  bool isBanned( char c ) ;

  bool isAllowed( char c ) ;

  /// 
  void sendCharacter( char c ) ;
} ;

#endif