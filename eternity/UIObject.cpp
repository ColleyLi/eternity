#include "UIObject.h"

UIObject::UIObject( int objectId,
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

UIObject::~UIObject()
{
  DESTROY( callback ) ;
}

void UIObject::setBackgroundSprite( int spriteId, D3DCOLOR iModulatingColor )
{
  backgroundSpriteId = spriteId ;
  spriteModulatingColor = iModulatingColor ;
}

void UIObject::clicked()
{
  // Execute default click action
  if( callback )
    callback->exec() ;
}

//!!Should we override operator [delete]?
void UIObject::Delete()
{
  deleted = true ;
}

bool UIObject::isDeleted()
{
  return deleted ;
}