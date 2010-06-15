#ifndef CARSIM_EXTERNAL_H
#define CARSIM_EXTERNAL_H

#include "vs_deftypes.h"

#ifdef __cplusplus
extern "C" {
#endif

void    external_calc (vs_real t, vs_ext_loc where);
void    external_echo (vs_ext_loc where);
vs_bool external_scan(char *, char *);
void    external_setdef (void);

#ifdef __cplusplus
}
#endif


#endif // CARSIM_EXTERNAL_H