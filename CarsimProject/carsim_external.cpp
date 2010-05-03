#include "carsim_external.h"
#include "vs_function_pointers.h"
#include "cs-main.h"
#include "SimWorld.h"

#ifdef __cplusplus
extern "C" {
#endif


// define some static variables used for the controller
static vs_real prevX, prevY, // X and Y coordinates of a preview point
               prevLenFwd,          // length forward for preview (m)
               latOffset,      // Lateral offset for tracking target
               gain,       // parameter for a control gain
               *vehicleX, *vehicleY, *vehicleYaw, // pointers to vehicle  X, Y, and yaw
               *sImpStr, // pointer to imported steering wheel angle 
               *sTstart; // pointer to start time

static int sUseExternal; // option to use external model.               


/* ----------------------------------------------------------------------------
Set up variables for the model extension. This is where variables for the
model extensions are given default values, where new variables are added
to the VS model, and where pointers are obtained from the existing model.
---------------------------------------------------------------------------- */
void external_setdef (void)
{
  int idX, idY;
 
  // set default values for parameters defined in this file
  sUseExternal = 1;
  prevLenFwd = 20.0;  // like the amount of time to "look ahead"
  gain = 10.0*PI/180.0;
  latOffset = -1.6;

  // C version of VS command: define_units deg/m 57.29577951  
  vs_define_units ("deg/m", 180.0/PI);
  
  // define two new parameters: L_FORWARD and LAT_ERR
  vs_define_parameter ("L_FORWARD", 
                       "Distance preview point is forward of vehicle CG",
                        &prevLenFwd, "M");
  vs_define_parameter ("LAT_TRACK", 
                        "Lateral offset (to driver's left) for target",
                        &latOffset, "M");



  vs_define_parameter ("GAIN_STEER_CTRL", 
                       "Control parameter: steering angle per meter of "
                       "lateral offset", &gain, "DEG/M");
  
  // define two new outputs using variables defined here
  idX = vs_define_output ("Xpreview", "X coordinate of preview point",
                           &prevX, "M");
  idY = vs_define_output ("Ypreview", "Y coordinate of preview point", 
                          &prevY, "M");
  
  // set labels and units for new output variables
  vs_set_sym_attribute (idX, OUTVAR_GEN_NAME, "X coordinate");
  vs_set_sym_attribute (idY, OUTVAR_GEN_NAME, "Y coordinate");
  vs_set_sym_attribute (idX, OUTVAR_BODY_NAME, "Steer control look point");
  vs_set_sym_attribute (idY, OUTVAR_BODY_NAME, "Steer control look point");
  vs_set_sym_attribute (idX, OUTVAR_UNITS, "M");
  vs_set_sym_attribute (idY, OUTVAR_UNITS, "M");  

  // get pointers to vehicle X, Y, yaw, and imported steering wheel angle
  // that will be used in calculation made during the run
  // Who would have guessed these are the names..
  // apparently they should be named in the .ECHO file.
  vehicleX    = vs_get_var_ptr("XCG_TM");
  vehicleY    = vs_get_var_ptr("YCG_TM");
  vehicleYaw  = vs_get_var_ptr("YAW");
  sImpStr = vs_get_var_ptr("IMP_STEER_SW");
  sTstart = vs_get_var_ptr("TSTART");










  // ROAD INFORMATION
  //void vs_get_road_xyz (double s, double l, double *x,double *y, double *z);
  double rx, ry, rz ;

  double s=0, l=0 ; 
  for( s = 0; s < 10 ; s+=0.1)
  {
    for( l = -1 ; l < 1 ; l+=0.1 )
    {
      vs_get_road_xyz( s, l, &rx, &ry, &rz ) ;
      info( "@(s=%lf, l=%lf), road=(%lf, %lf, %lf)", 
        s, l, rx, ry, rz ) ;
    }
  }
}


/* ----------------------------------------------------------------------------
Perform calculations involving the model extensions. The function is called
from four places as indicated with the argument where, which can have the 
values: VS_EXT_EQ_PRE_INIT, VS_EXT_EQ_INIT, VS_EXT_EQ_IN, VS_EXT_EQ_OUT, 
and VS_EXT_EQ_END (defined in vs_deftypes.h).
---------------------------------------------------------------------------- */
void  external_calc (vs_real t, vs_ext_loc where)
{
  switch (where)
  {
    // initialization after reading parsfile but before init
  case VS_EXT_EQ_PRE_INIT:
    // Add code here...

    break;

  case VS_EXT_EQ_INIT: // initialization after built-in init
    break;

  case VS_EXT_EQ_INIT2: // initialization after outputs are calculated
    // Add code here...
    break;

  case VS_EXT_EQ_IN: // calculate import variables at the start of a time step
    
    // UPDATE CARSIM
    // Compute what impetus should be
    // on the car gas, brake, and steering wheel
    // and push those updates to carsim


    // calculate X and Y coordinates of preview point
    // (i.e. where the vehicle will be next time step)
    prevX = *vehicleX + prevLenFwd*cos(*vehicleYaw);
    prevY = *vehicleY + prevLenFwd*sin(*vehicleYaw);

    if( !sUseExternal )
    {
      // no effect if sUseExternal is FALSE
      puts( "sUseExternal -- no solution" ) ;
    }
    else if( t <= *sTstart )
    {
      *sImpStr = 0.0; // no steering at the start
    }
    else // steer proportional to the lateral error
    {
      // this is their "CONTROLLER" - it steers the
      // vehicle proportional to how far "offtrack"
      // the car is.  We need to make this use
      // catmull-rom interpolation
      *sImpStr = gain*(latOffset -vs_road_l(prevX, prevY));
    }
    

    
    // Update the car position
    simWorld->car->pos.x = *vehicleX ;
    simWorld->car->pos.y = *vehicleY ;

    break;

  case VS_EXT_EQ_OUT: // calculate output variables at the end of a time step
    // Add code here...
    break;

  case VS_EXT_EQ_END: // calculations done at end of run
    // Add code here...
    break;
  }
}


/* ----------------------------------------------------------------------------
Write information into the current output echo file using the VS API 
function vs_write_to_echo_file. This function is called four times when
generating the echo file as indicated with the argument where, which can
have the values: VS_EXT_ECHO_TOP, VS_EXT_ECHO_SYPARS, VS_EXT_ECHO_PARS, and
VS_EXT_ECHO_END (defined in vs_deftypes.h).
---------------------------------------------------------------------------- */
void external_echo (vs_ext_loc where)
{
  static char buffer[200];

  switch (where)
  {
  case VS_EXT_ECHO_TOP: // top of echo file
    // Add code here...
    break;

  case VS_EXT_ECHO_SYPARS: // end of system parameter section
    // Add code here...
    break;

  case VS_EXT_ECHO_PARS: // end of model parameter section
    // Add code here...
    break;

  case VS_EXT_ECHO_END: // end of echo file
    // Add code here...
    break;
  }
}

/* ----------------------------------------------------------------------------
Scan a line read from the current input parsfile. Return TRUE if the keyword
is recognized, FALSE if not. This is not needed for variables and parameters
added using VS API funcitons, but might be helpful for additional inputs
needed for custom code.

keyword -> string with current ALL CAPS keyword to be tested
buffer  -> string with rest of line from parsfile.
---------------------------------------------------------------------------- */
vs_bool external_scan (char *keyword, char *buffer)
{
  // Add code here to check the keyword and do something if recognized...
  return false ;
}

#ifdef __cplusplus
}
#endif

