#include "SimWorld.h"

SimWorld * simWorld ;

SimWorld::SimWorld()
{
  info( "Constructing simworld.." ) ;

  camMode =FollowCam ; // defaults to FreeCam mode

  car = new Car() ;

  // Change to the assets/models directory.
  window->cd( ASSET( "models/3D_shape_files_CS/vehicles/european_sedan" ) ) ;

  //window->cd( ASSET( "models/3D_shape_files_CS/vehicles/Sports_car" ) ) ;
  car->loadCarModel( "body.obj" ) ;
  car->loadCarModel( "brakes_on.obj" ) ;
  car->loadCarModel( "details.obj" ) ;
  //car->loadCarModel( "details_convertible.obj" ) ;//s2000
  //car->loadCarModel( "interior.obj" ) ; //s2000
  car->loadCarModel( "interior_trim.obj" ) ; // european_sedan
  //car->loadCarModel( "interior_convertible.obj" ) ; // s2000
  car->loadCarModel( "seats.obj" ) ;
  car->loadCarModel( "steering_wheel.obj" ) ;
  //car->loadCarModel( "steeringwheel.obj" ) ; // s2000
  //car->loadModel( "windows.obj" ) ;
  window->cdPop() ;

  // Get some tires
  window->cd( ASSET( "models/3D_shape_files_CS/tires/195_45R16" ) ) ;
  //car->loadTireModel( "rotor.obj" ) ;// I don't like the rotor its like a hubcap
  car->loadTireModel( "stripe.obj" ) ; // stripe helps see how fast tire's moving
  car->loadTireModel( "tire.obj" ) ;
  car->loadTireModel( "wheel.obj" ) ;
  //!! this is only one tire and its located
  // at the origin.
  window->cdPop() ;


  /////
  // Set up a couple of lights
  memset( &light0, 0, sizeof( D3DLIGHT9 ) ) ;
  memset( &light1, 0, sizeof( D3DLIGHT9 ) ) ;

  light0.Type = D3DLIGHT_DIRECTIONAL ;  // believe it or not forgetting to set this
  // gives D3DERR_INTERNAL
  light0.Direction = D3DXVECTOR3( 0, -1, 0 ) ;
  setColor( &light0.Ambient, 1.0f, 0.5f, 0.5f, 0.5f ) ;
  setColor( &light0.Diffuse, 1.0f, 0.25f, 0.25f, 0.25f ) ;
  setColor( &light0.Specular, 1.0f, 0.35f, 0.35f, 0.35f ) ; // not
  // too overwhelming specular component

  //window->setLight( 0, &light0 ) ;


  light1.Type = D3DLIGHT_DIRECTIONAL ;  // believe it or not forgetting to set this
  
  // gives D3DERR_INTERNAL
  D3DXVECTOR3 light1Dir( -1, -1, 0 ) ;
  D3DXVec3Normalize( &light1Dir, &light1Dir ) ;
  light1.Direction = light1Dir ;

  setColor( &light1.Ambient, 1.0f, 0.5f, 0.5f, 0.5f ) ;
  setColor( &light1.Diffuse, 1.0f, 0.25f, 0.25f, 0.25f ) ;
  setColor( &light1.Specular, 1.0f, 0.35f, 0.35f, 0.35f ) ; // not
  // too overwhelming specular component

  window->setLight( 1, &light1 ) ;




  // create teh ground plane
  int limits = 1000 ;

  // This texture is never to appear.. its only
  // so that you don't think you'r ein the middle
  // of nowhwere
  float z = -5 ;// appear below ground

  /* y

  1 | 4
    |
  --+--x
    |
  2 | 3

  Texcoord 0,0 is top left

  */

// colorful
  window->createQuadVTC(

    Sprites::SixteenCounter,
    VertexTC(-limits,  limits, z, 0, 0 ),
    VertexTC(-limits, -limits, z, 0, 1 ),
    VertexTC( limits, -limits, z, 1, 1 ),
    VertexTC( limits,  limits, z, 1, 0 )
    
  ) ;
  
/*
// Solid green grass
  window->createQuadVC(
    VertexC(-limits,  limits, z, 0, 128, 9 ),
    VertexC(-limits, -limits, z, 0, 128, 9 ),
    VertexC( limits, -limits, z, 0, 128, 9 ),
    VertexC( limits,  limits, z, 0, 128, 9 )
  );
*/
}

SimWorld::~SimWorld()
{
  delete car ;
}

void SimWorld::Draw()
{
  
  car->drawStats() ;

  // Draw the vehicle, but not if in cockpit mode
  // (there's no point)
  if( camMode != CockpitCam )
    car->draw( window->getTimeElapsedSinceLastFrame() ) ;
}
