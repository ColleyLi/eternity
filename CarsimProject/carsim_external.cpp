#include "carsim_external.h"
#include "vs_function_pointers.h"
#include "cs-main.h"
#include "SimWorld.h"

#ifdef __cplusplus
extern "C" {
#endif



// Define a few shorthands
//#define CAR simWorld->car->csV
// Above not used anymore..

// New shorthand:  Access a property
// of the carsim vehicle by CAR(propertyName).
// This looks a bit weird from C++, but
// it's a nice shorthand syntax.
#define CAR(PROPERTY) (*simWorld->car->csV.##PROPERTY)

/// Set up variables for the model extension.
/// This is where variables for the
/// model extensions are given default values,
/// where new variables are added
/// to the VS model, and where pointers are
/// obtained from the existing model.
void external_setdef()
{
  // get pointers to vehicle X, Y, yaw, and imported steering wheel angle
  // that will be used in calculation made during the run
  // THESE VARIABLES ARE LISTED IN
  // THE i_i_outputs.txt FILE LOCATED IN
  simWorld->car->csV.x = vs_get_var_ptr( "XCG_TM" ) ;
  simWorld->car->csV.y = vs_get_var_ptr( "YCG_TM" ) ;
  simWorld->car->csV.z = vs_get_var_ptr( "ZCG_TM" ) ;

  simWorld->car->csV.vx = vs_get_var_ptr( "VX" ) ;
  simWorld->car->csV.vy = vs_get_var_ptr( "VY" ) ;
  simWorld->car->csV.vz = vs_get_var_ptr( "VZ" ) ;
  
  // I THOUGHT These were in degrees.. ACTUALLY NO.
  // although the file says (deg) after each
  // of these properties, they are actually in
  // RADIANS, becaus eI can use them without
  // convertingthem in the drawing code.
  simWorld->car->csV.yaw   = vs_get_var_ptr( "YAW" ) ;//RADS
  simWorld->car->csV.pitch = vs_get_var_ptr( "PITCH" ) ;
  simWorld->car->csV.roll  = vs_get_var_ptr( "ROLL" ) ;

  simWorld->car->csV.wheelL1Steer = vs_get_var_ptr( "STEER_L1" ) ;
  simWorld->car->csV.wheelR1Steer = vs_get_var_ptr( "STEER_R1" ) ;

  simWorld->car->csV.theirThrottle = vs_get_var_ptr( "THROTTLE" ) ;
  simWorld->car->csV.throttle = vs_get_var_ptr( "IMP_THROTTLE_ENGINE" ) ;
  simWorld->car->csV.brake = vs_get_var_ptr( "IMP_BK_BOOST" ) ;
  simWorld->car->csV.brakeStat = vs_get_var_ptr( "IMP_BK_STAT" ) ;
  simWorld->car->csV.brakePedalForce = vs_get_var_ptr( "IMP_FBK_PDL" ) ;
  simWorld->car->csV.brakeBoosterForce = vs_get_var_ptr( "IMP_FBK_BSTIN" ) ;
  
  simWorld->car->csV.IMP_MYBK_L1 = vs_get_var_ptr( "IMP_MYBK_L1" ) ; 
  simWorld->car->csV.IMP_MYBK_L2 = vs_get_var_ptr( "IMP_MYBK_L2" ) ; 
  simWorld->car->csV.IMP_MYBK_R1 = vs_get_var_ptr( "IMP_MYBK_R1" ) ; 
  simWorld->car->csV.IMP_MYBK_R2 = vs_get_var_ptr( "IMP_MYBK_R2" ) ; 

  simWorld->car->csV.steeringAngle = vs_get_var_ptr("IMP_STEER_SW");
  simWorld->car->csV.theirSteeringAngle = vs_get_var_ptr("STEER_SW");//528 of i_i
  simWorld->car->csV.startTime = vs_get_var_ptr("TSTART");

  simWorld->car->csV.tachAngle = vs_get_var_ptr( "A_TACH" ) ;
  simWorld->car->csV.speedoAngle = vs_get_var_ptr( "A_SPEEDO" ) ;

  simWorld->car->csV.PwrEngAv = vs_get_var_ptr( "PWRENGAV" ) ;

  simWorld->car->csV.betaSideSlip = vs_get_var_ptr( "BETA" ) ;

  simWorld->car->csV.timeStep = vs_get_var_ptr( "TSTEP" ) ;



  int *opt_steer, *opt_driver_model, *opt_sc,
      *opt_driver_action ;
  opt_steer = vs_get_var_ptr_int( "OPT_STEER" ) ;
  opt_driver_model = vs_get_var_ptr_int( "OPT_DRIVER_MODEL" ) ;
  opt_sc = vs_get_var_ptr_int( "OPT_SC" ) ;
  opt_driver_action = vs_get_var_ptr_int( "OPT_DRIVER_ACTION" ) ;
  
  
  

  printf( "OPT_STEER=%d, OPT_DRIVER_MODEL=%d, OPT_SC=%d\n",
    *opt_steer, *opt_driver_model, *opt_sc ) ;
  printf( "OPT_DRIVER_ACTION=%d\n",
    *opt_driver_action ) ;

}





/// Perform calculations involving the model
/// extensions. The function is called
/// from four places as indicated with the
/// argument where, which can have the 
/// values: VS_EXT_EQ_PRE_INIT, VS_EXT_EQ_INIT,
/// VS_EXT_EQ_IN, VS_EXT_EQ_OUT, 
/// and VS_EXT_EQ_END (defined in vs_deftypes.h).
void external_calc(vs_real t, vs_ext_loc where)
{
  switch (where)
  {
  case VS_EXT_EQ_PRE_INIT:
    // initialization after reading parsfile but before init
    break;

  case VS_EXT_EQ_INIT:
      // initialization after built-in init
      {
        // Road's ready.  We can get it's params now

        double sStart, sEnd ;
        vs_get_road_start_stop( &sStart, &sEnd ) ;

        // When sStart and sEnd are both 0.. then the road
        // may not be looped.  I think if a road is not looped
        // it continues straight forever past its end.  I THINK.
        info( "ROAD START: %lf, END: %lf", sStart, sEnd );

        // Store these s values off.
        simWorld->car->csV.roadSStart = sStart ;
        simWorld->car->csV.roadSEnd = sEnd ;

        if( ! sEnd && ! sStart )
        {
          sEnd = 2000.0 ;
          warning( "The road isn't looped, "
            "going only until s=%lf.", sEnd ) ; 
        } 

        // try a step size varying
        // with the size of the road.
        //double sStep = sEnd / 2000 ;
        double sStep = 5.0 ; // this is reasonable

        // Now sample the road from roadSStart to sEnd,
        // along the center line, building triangles,
        // that go either way out to the edges.
        double 
          roadL1X, roadL1Y, roadL1Z,
          roadC1X, roadC1Y, roadC1Z,
          roadR1X, roadR1Y, roadR1Z,
          roadL2X, roadL2Y, roadL2Z,
          roadC2X, roadC2Y, roadC2Z,
          roadR2X, roadR2Y, roadR2Z ;


        // The lateral measure of the road
        // i set here manually.  There must
        // be a way to read it from carsim however.
        double lMax = 10 ;

        VertexTC v1, v2, v3, v4, v5, v6 ;

        v1.tex = D3DXVECTOR2( 0, 0 ) ;
        v2.tex = D3DXVECTOR2( 0.5, 0 ) ;
        v3.tex = D3DXVECTOR2( 1, 0 ) ;

        v4.tex = D3DXVECTOR2( 0, 1 ) ;
        v5.tex = D3DXVECTOR2( 0.5, 1 ) ;
        v6.tex = D3DXVECTOR2( 1, 1 ) ;

        // (if you get all 0's,
        // remember the "road" may be
        // just a big flat plane in some demo's)
        double s ;
        for( s = 0.0 ; s < sEnd - sStep ; s += sStep )
        {
          // Get 6 points,
          //   2 at left edge of road,
          //   2 at center of road
          //   2 at right edge of road
          vs_get_road_xyz( s, -lMax, &roadL1X, &roadL1Y, &roadL1Z ) ;
          vs_get_road_xyz( s,     0, &roadC1X, &roadC1Y, &roadC1Z ) ;
          vs_get_road_xyz( s, +lMax, &roadR1X, &roadR1Y, &roadR1Z ) ;

          // look ahead one
          vs_get_road_xyz( s + sStep, -lMax, &roadL2X, &roadL2Y, &roadL2Z ) ;
          vs_get_road_xyz( s + sStep,     0, &roadC2X, &roadC2Y, &roadC2Z ) ;
          vs_get_road_xyz( s + sStep, +lMax, &roadR2X, &roadR2Y, &roadR2Z ) ;

          /*

          ->    *    <-
                |
                |
                |
          ->    *    <-

          */

          // Build the 6 vertices, can't do directly,
          // doubles
          v1.pos = D3DXVECTOR3( roadL1X, roadL1Y, roadL1Z ) ;
          v2.pos = D3DXVECTOR3( roadC1X, roadC1Y, roadC1Z ) ;
          v3.pos = D3DXVECTOR3( roadR1X, roadR1Y, roadR1Z ) ;
          
          v4.pos = D3DXVECTOR3( roadL2X, roadL2Y, roadL2Z ) ;
          v5.pos = D3DXVECTOR3( roadC2X, roadC2Y, roadC2Z ) ;
          v6.pos = D3DXVECTOR3( roadR2X, roadR2Y, roadR2Z ) ;

          int roadToUse = Sprites::Road1;
          if( rand()%2 )
            roadToUse = Sprites::Road2 ;
          
          // Build 2 quads at this spot.
          window->createQuadVTC(
            Sprites::Road1,
            v1, v4, v5, v2
          ) ;

          window->createQuadVTC(
            Sprites::Road1,
            v2, v5, v6, v3
          ) ;
          
        }

      }


    break;

  case VS_EXT_EQ_INIT2:
    // initialization after outputs are calculated
    // Add code here...
    break;

  case VS_EXT_EQ_IN:
    // calculate import variables at the start of a time step
    
    // UPDATE CARSIM
    // Compute what impetus should be
    // on the car gas, brake, and steering wheel
    // and push those updates to carsim
    simWorld->car->update( 0.0166 ) ;
    break;

  case VS_EXT_EQ_OUT:
    // calculate output variables at the end of a time step

    break;

  case VS_EXT_EQ_END:
    // calculations done at end of run
    break;
  }
}


/// Write information into the current
/// output echo file using the VS API 
/// function vs_write_to_echo_file. 
/// This function is called four times when
/// generating the echo file as indicated 
/// with the argument where, which can
/// have the values: VS_EXT_ECHO_TOP, 
/// VS_EXT_ECHO_SYPARS, VS_EXT_ECHO_PARS, and
/// VS_EXT_ECHO_END (defined in vs_deftypes.h).
void external_echo( vs_ext_loc where )
{
  static char buffer[200];

  switch (where)
  {
  case VS_EXT_ECHO_TOP:
    // top of echo file
    // Add code here...
    break;

  case VS_EXT_ECHO_SYPARS:
    // end of system parameter section
    // Add code here...
    break;

  case VS_EXT_ECHO_PARS:
    // end of model parameter section
    // Add code here...
    break;

  case VS_EXT_ECHO_END:
    // end of echo file
    // Add code here...
    break;
  }
}

/// Scan a line read from the current input
/// parsfile. Return TRUE if the keyword
/// is recognized, FALSE if not. This is not
/// needed for variables and parameters
/// added using VS API funcitons, but might
/// be helpful for additional inputs
/// needed for custom code.
/// 
/// keyword -> string with current ALL CAPS
/// keyword to be tested buffer  -> string
/// with rest of line from parsfile.
vs_bool external_scan( char *keyword, char *buffer )
{
  // Add code here to check the
  // keyword and do something if recognized...

  //info( "CarSim:  Reading `%s`:  %s\n", keyword, buffer ) ;
  if( ! strncmp( keyword, "add_obj", 6 ) )
  {
    // It would be nice to be able to stop
    // at "add_obj" because this is when
    // its loading a model.  Could tell us
    // what model to load.
  }
  return false ;
}

#ifdef __cplusplus
}
#endif

