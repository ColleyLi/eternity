#ifndef EXTERNAL_H
#define EXTERNAL_H

void    external_calc (vs_real t, vs_ext_loc where);
void    external_echo (vs_ext_loc where);
vs_bool external_scan(char *, char *);
void    external_setdef (void);

/* ----------------------------------------------------------------------------
Set up variables for the model extension. This is where variables for the
model extensions are given default values, where new variables are added
to the VS model, and where pointers are obtained from the existing model.
---------------------------------------------------------------------------- */
void external_setdef (void)
{
  // Add code here...
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
    puts( "i'm initialized!!" ) ;
    // Add code here...
    break;

  case VS_EXT_EQ_INIT: // initialization after built-in init
    // Add code here...
    break;

  case VS_EXT_EQ_INIT2: // initialization after outputs are calculated
    // Add code here...
    break;

  case VS_EXT_EQ_IN: // calculate import variables at the start of a time step
    // Add code here...

    break;

  case VS_EXT_EQ_OUT: // calculate output variables at the end of a time step
    // Add code here...
    break;

  case VS_EXT_EQ_END: // calculations done at end of run
    // Add code here...
    puts( "I AM DONE!" ) ;
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
  return FALSE;
}

#endif // EXTERNAL_H