#ifndef VS_GET_API_H
#define VS_GET_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <windows.h>
#include <stdio.h>
#include "vs_function_pointers.h"

// load DLL and get API functions
int vs_get_dll_path(char *simfile, char *pathDLL);
int vs_get_api_basic (HMODULE dll, char *dll_fname);
int vs_get_api_extend (HMODULE dll, char *dll_fname);
int vs_get_api_road (HMODULE dll, char *dll_fname);
int vs_get_api_install_external (HMODULE dll, char *dll_fname);

#ifdef __cplusplus
}
#endif

#endif // VS_GET_API_H

