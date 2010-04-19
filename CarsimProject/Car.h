#ifndef CAR_H
#define CAR_H

#include "cs-main.h"

/// Represents a carsim car
class Car
{
  D3DXVECTOR3 pos ;
public:

  Car()
  {
    // zero out pos
    pos.x=pos.y=pos.z=0;
  }

  ~Car()
  {
  }

  void loadModel( FILE *srcFile )
  {
    // REQUIRES:  already opened file,
    // with srcFile POINTING AT START POINT
    // of vertex set
  }

  void update()
  {
    // Update position from Carsim,
    // !!make sound if slippage is occurring etc


    // UPDATE CARSIM
    // Compute what impetus should be
    // on the car gas, brake, and steering wheel
    // and push those updates to carsim
  }

  void draw()
  {
    // WORLD UNITS are "metres"
    float w = 1.85f, h = 1.4f, len = 5.0f ;

    // Draw centered around pos
    D3DCOLOR color = D3DCOLOR_XRGB( 255, 128, 0 ) ;

    D3DXMATRIX mat ;
    D3DXMatrixTranslation( &mat, pos.x, pos.y, pos.z ) ;

    // EEK!! I have to rotate the car in y then x,
    // depending on its native positioning.  If orget
    // how this is done smoothly :D  Goind to lsspeel 
    // now, this is where i !!left off!!
    //D3DXMatrixRotationY

    // World matrix translate
    //window->setWorld( mat ) ;
    
    // Roof
    /*
    window->drawQuad( 

      

    ) ;
    */
  }
} ;

#endif