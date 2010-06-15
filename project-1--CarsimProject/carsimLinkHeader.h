/* This file has functions that can be used to extend a VS solver. The functions 
are passed to the VS solver using the API functions as shown in the example
wrapper program.

Revision log:
Jun 05, 08. M. Sayers. Different include file vs_api.h for simplified API.
Apr 17, 07. M. Sayers. Added VS_EXT_EQ_PRE_INIT and VS_EXT_EQ_INIT2
Mar 12, 07. M. Sayers. Released CarSim 7.0.
Dec 20, 06. M. Sayers. Created.
*/

// Carsim is a C api,
// so it must be included
// as a set of C-functions

#ifdef __cplusplus
extern "C"{
#endif

  // header files for standard C libraries 
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

#include "vs_deftypes.h"

#include "vs_function_pointers.h"

#include "vs_get_api.h"

#include "carsim_external.h"

#ifdef __cplusplus
}
#endif

