#include "SimWorld.h"

SimWorld * simWorld ;

SimWorld::SimWorld()
{
  info( "Constructing simworld.." ) ;

  camMode = FreeCam ; // defaults to FreeCam mode

  car = new Car() ;

  // Change to the assets/models directory.
  window->cd( ASSET( "models/3D_shape_files_CS/vehicles/european_sedan" ) ) ;
  car->loadModel( "body.obj" ) ;
  car->loadModel( "brakes_on.obj" ) ;
  car->loadModel( "details.obj" ) ;
  car->loadModel( "interior_trim.obj" ) ;
  car->loadModel( "seats.obj" ) ;
  car->loadModel( "steering_wheel.obj" ) ;
  //car->loadModel( "windows.obj" ) ;
  window->cdPop() ;

  // Get some tires
  window->cd( ASSET( "models/3D_shape_files_CS/tires/205_70R15" ) ) ;
  //car->loadTireModel( "rotor.obj" ) ;// I don't like the rotor its like a hubcap
  car->loadTireModel( "stripe.obj" ) ; // stripe helps see how fast tire's moving
  car->loadTireModel( "tire.obj" ) ;
  car->loadTireModel( "wheel.obj" ) ;
  //!! this is only one tire and its located
  // at the origin.
  window->cdPop() ;

  track = new Track() ;


  /////
  // Set up a couple of lights
  memset( &light0, 0, sizeof( D3DLIGHT9 ) ) ;
  memset( &light1, 0, sizeof( D3DLIGHT9 ) ) ;

  light0.Type = D3DLIGHT_DIRECTIONAL ;  // believe it or not forgetting to set this
  // gives D3DERR_INTERNAL
  light0.Direction = D3DXVECTOR3( 0, -1, 0 ) ;
  setColor( &light0.Ambient, 0.5f, 0.5f, 0.5f, 0.5f ) ;
  setColor( &light0.Diffuse, 1.0f, 1.0f, 1.0f, 1.0f ) ;

  window->setLight( 0, &light0 ) ;
}

SimWorld::~SimWorld()
{
  delete car ;
  delete track ;
}

void SimWorld::LoadCarSimFile( char* filename )
{
  warning( "LoadCarSimFile not implemented yet" ) ;

  // Scan to the point in the file where
  // the car model is


  // Scan to the point in the file where
  // the TRACK is.

}


void SimWorld::Draw()
{
#pragma region Draw the ground plane

  /*
  int limits = 1 ;

  // * | *
  //   |
  // --+--> x
  //   |
  // * | * (limits, 0, limits)
  //   v
  //    z
  window->setVertexDeclaration( D3DWindow::PositionColor ) ;
  window->drawQuad(

  VertexC( limits, 0,  limits, 128, 128, 128 ),
  VertexC( limits, 0, -limits, 128, 128, 128 ),
  VertexC(-limits, 0, -limits, 128, 128, 128 ),
  VertexC(-limits, 0,  limits,  78,  78,  78 )

  ) ;
  */
#pragma endregion



  // Draw the track.
  track->draw() ;

  // Draw the vehicle
  car->draw( window->getTimeElapsedSinceLastFrame() ) ;
}
