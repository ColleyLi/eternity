#ifndef VS_API_H
#define VS_API_H

#pragma region vs_api.h

// Core functions (from vs_solver_api.c)
typedef int     (*_x_vs_bar_graph_update) (int *);
_x_vs_bar_graph_update vs_bar_graph_update ;

typedef void    (*_x_vs_copy_io) (vs_real *imports, vs_real *exports);
_x_vs_copy_io vs_copy_io ;

typedef vs_bool (*_x_vs_error_occurred) (void);
_x_vs_error_occurred vs_error_occurred ;

typedef void    (*_x_vs_free_all) (void);
_x_vs_free_all vs_free_all ;

typedef char    *(*_x_vs_get_error_message) (void);
_x_vs_get_error_message vs_get_error_message ;

typedef char    *(*_x_vs_get_output_message) (void);
_x_vs_get_output_message vs_get_output_message ;

typedef vs_real (*_x_vs_get_tstep) (void);
_x_vs_get_tstep vs_get_tstep ;

typedef char    *(*_x_vs_get_version_vs) (void);
_x_vs_get_version_vs vs_get_version_vs ;

typedef char    *(*_x_vs_get_version_product) (void);
_x_vs_get_version_product vs_get_version_product ;

typedef void    (*_x_vs_initialize) (vs_real t, void (*ext_calc) (vs_real, vs_ext_loc), void (*ext_echo) (vs_ext_loc));
_x_vs_initialize vs_initialize ;

typedef int    (*_x_vs_integrate) (vs_real *t, void (*ext_eq_in) (vs_real, vs_ext_loc));
_x_vs_integrate vs_integrate ;

typedef int    (*_x_vs_integrate_io) (vs_real t, vs_real *imports, vs_real *exports);
_x_vs_integrate_io vs_integrate_io ;

typedef vs_bool (*_x_vs_opt_pause)(void);
_x_vs_opt_pause vs_opt_pause ; 

typedef void   (*_x_vs_read_configuration) (const char *simfile, int *n_import, int *n_export, vs_real *tstart, vs_real *tstop, vs_real *tstep);
_x_vs_read_configuration vs_read_configuration ;

typedef vs_real (*_x_vs_setdef_and_read) (const char *simfile, void (*ext_setdef) (void), int (*_ext_scan) (char *, char *));
_x_vs_setdef_and_read vs_setdef_and_read ;

typedef int    (*_x_vs_stop_run) (void);
_x_vs_stop_run vs_stop_run ;

typedef void   (*_x_vs_terminate) (vs_real t, void (*ext_echo) (vs_ext_loc));
_x_vs_terminate vs_terminate ;

typedef void   (*_x_vs_terminate_run) (vs_real t);
_x_vs_terminate_run vs_terminate_run ;

typedef int    (*_x_vs_run) (char *simfile);
_x_vs_run vs_run ;

// VS API functions for extending models & adding equations
typedef int     (*_x_vs_define_import) (char *keyword, char *desc, vs_real *real, char *);
_x_vs_define_import vs_define_import ;

typedef int     (*_x_vs_define_output) (char *shortname, char *longname, vs_real *real, char *);
_x_vs_define_output vs_define_output ;

typedef int     (*_x_vs_define_parameter) (char *keyword, char *desc, vs_real *, char *);
_x_vs_define_parameter vs_define_parameter ;

typedef void    (*_x_vs_define_units) (char *desc, vs_real gain);
_x_vs_define_units vs_define_units ;

typedef int     (*_x_vs_define_variable) (char *keyword, char *desc, vs_real *);    
_x_vs_define_variable vs_define_variable ;

typedef vs_real *(*_x_vs_get_var_ptr) (char *keyword);                                        
_x_vs_get_var_ptr vs_get_var_ptr ;

typedef int     *(*_x_vs_get_var_ptr_int) (char *keyword);
_x_vs_get_var_ptr_int vs_get_var_ptr_int ;

typedef void    (*_x_vs_set_units) (char *var_keyword, char *units_keyword);
_x_vs_set_units vs_set_units ;

typedef void    (*_x_vs_install_calc_func) (char *name, void *func);
_x_vs_install_calc_func vs_install_calc_func ;

typedef void    (*_x_vs_printf) (const char *format, ...);
_x_vs_printf vs_printf ;

typedef void    (*_x_vs_printf_error) (const char *format, ...);
_x_vs_printf_error vs_printf_error ;

typedef int     (*_x_vs_set_sym_int) (int id, vs_sym_attr_type dataType, int value);
_x_vs_set_sym_int vs_set_sym_int ;

typedef int     (*_x_vs_set_sym_real) (int id, vs_sym_attr_type dataType, vs_real value);
_x_vs_set_sym_real vs_set_sym_real ;

typedef int     (*_x_vs_set_sym_attribute) (int id, vs_sym_attr_type type, const void *att);
_x_vs_set_sym_attribute vs_set_sym_attribute ;

typedef void    (*_x_vs_read_next_line) (char *buffer, int n);
_x_vs_read_next_line vs_read_next_line ;

typedef void    (*_x_vs_write_to_echo_file) (char *buffer);
_x_vs_write_to_echo_file vs_write_to_echo_file ;

typedef void    (*_x_vs_write_header_to_echo_file) (char *buffer);
_x_vs_write_header_to_echo_file vs_write_header_to_echo_file ;

typedef void    (*_x_vs_write_f_to_echo_file) (char *key, vs_real , char *doc);
_x_vs_write_f_to_echo_file vs_write_f_to_echo_file ;

typedef void    (*_x_vs_write_i_to_echo_file) (char *key, int , char *doc);
_x_vs_write_i_to_echo_file vs_write_i_to_echo_file ;

typedef int     (*_x_vs_get_sym_attribute) (int id, vs_sym_attr_type type, void **att);
_x_vs_get_sym_attribute vs_get_sym_attribute ;

typedef int     (*_x_vs_define_parameter_int) (char *keyword, char *desc, int *);
_x_vs_define_parameter_int vs_define_parameter_int ;


// functions for accessing the 3D road model in CarSim 7.01 and TruckSim 7.0
typedef void    (*_x_vs_get_dzds_dzdl) (vs_real s, vs_real l, vs_real *dzds, vs_real *dzdl);
_x_vs_get_dzds_dzdl vs_get_dzds_dzdl ;

typedef void    (*_x_vs_get_road_contact) (vs_real y, vs_real x, int inst, vs_real *z, vs_real *dzdy, vs_real *dzdx, vs_real *mu);
_x_vs_get_road_contact vs_get_road_contact ;

typedef void    (*_x_vs_get_road_start_stop) (vs_real *start, vs_real *stop);
_x_vs_get_road_start_stop vs_get_road_start_stop ;

typedef void    (*_x_vs_get_road_xyz) (vs_real s, vs_real l, vs_real *x, vs_real *y, vs_real *z);
_x_vs_get_road_xyz vs_get_road_xyz ;

typedef vs_real (*_x_vs_road_l) (vs_real x, vs_real y);
_x_vs_road_l vs_road_l ;

typedef vs_real (*_x_vs_road_s) (vs_real x, vs_real y);
_x_vs_road_s vs_road_s;

typedef vs_real (*_x_vs_road_x) (vs_real s);
_x_vs_road_x vs_road_x;

typedef vs_real (*_x_vs_road_y) (vs_real s);
_x_vs_road_y vs_road_y ;

typedef vs_real (*_x_vs_road_z) (vs_real x, vs_real y);
_x_vs_road_z vs_road_z ;

typedef vs_real (*_x_vs_road_yaw) (vs_real sta, vs_real direction);
_x_vs_road_yaw vs_road_yaw ;

typedef vs_real (*_x_vs_s_loop) (vs_real s);
_x_vs_s_loop vs_s_loop ;

typedef vs_real (*_x_vs_target_l) (vs_real s);
_x_vs_target_l vs_target_l ;

// functions for accessing the 3D road model starting with CarSim 7.1
typedef void    (*_x_vs_get_dzds_dzdl_i) (vs_real s, vs_real l, vs_real *dzds, vs_real *dzdl, vs_real inst);
_x_vs_get_dzds_dzdl_i vs_get_dzds_dzdl_i ;

typedef void    (*_x_vs_get_road_contact_sl) (vs_real s, vs_real l, int inst, vs_real *z, vs_real *dzds, vs_real *dzdl, vs_real *mu);
_x_vs_get_road_contact_sl vs_get_road_contact_sl ;

typedef vs_real (*_x_vs_road_curv_i) (vs_real s, vs_real inst);
_x_vs_road_curv_i vs_road_curv_i ;

typedef vs_real (*_x_vs_road_l_i) (vs_real x, vs_real y, vs_real inst);
_x_vs_road_l_i vs_road_l_i ;

typedef vs_real (*_x_vs_road_pitch_sl_i) (vs_real s, vs_real l, vs_real yaw, vs_real inst);
_x_vs_road_pitch_sl_i vs_road_pitch_sl_i ;

typedef vs_real (*_x_vs_road_roll_sl_i) (vs_real s, vs_real l, vs_real yaw, vs_real inst);
_x_vs_road_roll_sl_i vs_road_roll_sl_i ;

typedef vs_real (*_x_vs_road_s_i) (vs_real x, vs_real y, vs_real inst);
_x_vs_road_s_i vs_road_s_i ;

typedef vs_real (*_x_vs_road_x_i) (vs_real sy, vs_real inst);
_x_vs_road_x_i vs_road_x_i ;

typedef vs_real (*_x_vs_road_y_i) (vs_real sy, vs_real inst);
_x_vs_road_y_i vs_road_y_i ;

typedef vs_real (*_x_vs_road_yaw_i) (vs_real sta, vs_real directiony, vs_real inst);
_x_vs_road_yaw_i vs_road_yaw_i ;

typedef vs_real (*_x_vs_road_z_i) (vs_real x, vs_real yy, vs_real inst);
_x_vs_road_z_i vs_road_z_i ;

typedef vs_real (*_x_vs_road_z_sl_i) (vs_real s, vs_real l, vs_real inst);
_x_vs_road_z_sl_i vs_road_z_sl_i ;

// tire function for model used in CarSim and TruckSim
typedef void    (*_cs_tire) (vs_real *fx, vs_real *fy, vs_real *mx, vs_real *my, vs_real *mz,
                             vs_real *ds_lag_long, vs_real *ds_lag_lat,
                             vs_real fz, vs_real *s_lag_long, vs_real *s_lag_lat, vs_real gamma,
                             vs_real mux, vs_real muy, vs_real mu_ref_x, vs_real mu_ref_y,
                             vs_real vxwc, vs_real vyctc, vs_real *rre, vs_real w, 
                             vs_real lrelax_long, vs_real lrelax_lat, vs_real vlow_alpha, 
                             vs_real tstep, int opt_no_lag, vs_real rr_rsc, vs_real rr_0, vs_real rr_v,
                             vs_real cro_fy, vs_real cro_mz, vs_real cro_mx,
                             int itab, int inst, int opt_model); 
_cs_tire cs_tire ;

// installation functions tested Nov 30, 2008
typedef void (*_x_vs_install_calc_function) (void (*calc) (vs_real time, vs_ext_loc where));
_x_vs_install_calc_function vs_install_calc_function ;

typedef void (*_x_vs_install_echo_function) (void (*echo) (vs_ext_loc where));
_x_vs_install_echo_function vs_install_echo_function ;

typedef void (*_x_vs_install_setdef_function) (void (*setdef) (void));
_x_vs_install_setdef_function vs_install_setdef_function ;

typedef void (*_x_vs_install_scan_function) (vs_bool (*scan) (char *, char *));
_x_vs_install_scan_function vs_install_scan_function ;

typedef void (*_x_vs_install_free_function) (void (*free) (void));
_x_vs_install_free_function vs_install_free_function ;

// load DLL and get API functions
int vs_get_dll_path(char *simfile, char *pathDLL);
int vs_get_api_basic (HMODULE dll, char *dll_fname);
int vs_get_api_extend (HMODULE dll, char *dll_fname);
int vs_get_api_road (HMODULE dll, char *dll_fname);
int vs_get_api_install_external (HMODULE dll, char *dll_fname);

#pragma endregion

#pragma region vs_get_api.c

// These functions have to be in the
// same file as the definitions above,
// else, linker issues

/* ----------------------------------------------------------------------------
Get solver path according keyword DLLFILE in the simfile. Return 0 if OK,
-1 if not.
---------------------------------------------------------------------------- */
int vs_get_dll_path(char *simfile, char *pathDLL)
{
  FILE *fp;
  char tmpstr[FILENAME_MAX*2], *key, *rest;

  if ((fp = fopen(simfile, "r")) == NULL)
  {
    sprintf (tmpstr, 
      "\nThis program needs a simfile to obtain other file names. The file\n"
      "\"%s\" either does not exist or could not be opened.", simfile);
    MessageBoxA (NULL, tmpstr, "Error", MB_ICONERROR);
    return -1;
  }
  while (fgets(tmpstr, 255, fp))
  {
    key = (char *)strtok(tmpstr, " \t\n");
    rest = (char *)strtok(NULL, "\n\t");
    if (!strcmp(key, "DLLFILE") && rest && rest[0])
    {
      strcpy (pathDLL, rest);
      fclose(fp);

      // Now see if the DLL exists
      if ((fp = fopen(pathDLL, "rb")) == NULL)
      {
        sprintf (tmpstr, 
          "\nThe simfile identified the DLL file \"%s\"\n"
          "with the keyword DLLFILE. This DLL file either does not exist or "
          "cannot be opened.", pathDLL);
        MessageBoxA (NULL, tmpstr, "Error", MB_ICONERROR);
        return -1;
      }
      fclose(fp);
      return 0;
    }
    else if (!strcmp(key, "END")) break;
  }

  fclose(fp);
  sprintf (tmpstr, "\nThis program needs a DLL to run, identified with the\n"
    "keyword DLLFILE. The simfile \"%s\" did\n"
    "not identify a DLL file.", simfile);
  MessageBoxA (NULL, tmpstr, "Error", MB_ICONERROR);
  return -1;
}


// Utility for error handling in getting DLL functions
static int vss_dll_problem (char *dll, char *func, int code)
{
  char tmpstr[FILENAME_MAX*2];
  if (code == -1)
    sprintf (tmpstr,  "The function %s was not given a valid DLL.\n"
    "\"%s\" did not load.", func, dll);
  else 
    sprintf (tmpstr,  "The function %s could not get the required VS API functions\n"
    "from DLL \"%s\".", func, dll);
  MessageBoxA(NULL, tmpstr, "Error", MB_ICONERROR);
  return code;
}

/* ----------------------------------------------------------------------------
Get basic VS API functions needed to run a simulation from a VS DLL.
---------------------------------------------------------------------------- */
int vs_get_api_basic (HMODULE dll, char *dll_fname)
{
  if (dll == NULL) 
    return vss_dll_problem(dll_fname, "vs_get_api_basic", -1);

  // Get addresses for basic VS API functions
  vs_bar_graph_update = (_x_vs_bar_graph_update)GetProcAddress(dll, "vs_bar_graph_update");
  vs_copy_io = (_x_vs_copy_io)GetProcAddress(dll, "vs_copy_io");
  vs_error_occurred = (_x_vs_error_occurred)GetProcAddress(dll, "vs_error_occurred");
  vs_free_all = (_x_vs_free_all)GetProcAddress(dll, "vs_free_all");
  vs_get_error_message = (_x_vs_get_error_message)GetProcAddress(dll, "vs_get_error_message");
  vs_get_output_message = (_x_vs_get_output_message)GetProcAddress(dll, "vs_get_output_message");
  vs_get_tstep = (_x_vs_get_tstep)GetProcAddress(dll, "vs_get_tstep");
  vs_get_version_product = (_x_vs_get_version_product)GetProcAddress(dll, "vs_get_version_product");
  vs_get_version_vs = (_x_vs_get_version_vs)GetProcAddress(dll, "vs_get_version_vs");
  vs_initialize = (_x_vs_initialize)GetProcAddress(dll, "vs_initialize");
  vs_integrate = (_x_vs_integrate)GetProcAddress(dll, "vs_integrate");
  vs_integrate_io = (_x_vs_integrate_io)GetProcAddress(dll, "vs_integrate_io");
  vs_opt_pause = (_x_vs_opt_pause)GetProcAddress(dll, "vs_opt_pause");
  vs_read_configuration = (_x_vs_read_configuration)GetProcAddress(dll, "vs_read_configuration");
  vs_setdef_and_read = (_x_vs_setdef_and_read)GetProcAddress(dll, "vs_setdef_and_read");
  vs_stop_run = (_x_vs_stop_run)GetProcAddress(dll, "vs_stop_run");
  vs_terminate = (_x_vs_terminate)GetProcAddress(dll, "vs_terminate");

  // Check to see if all functions were found
  if (!vs_bar_graph_update || !vs_copy_io || !vs_error_occurred ||
    !vs_free_all || !vs_get_error_message || !vs_get_output_message ||
    !vs_get_tstep || !vs_get_version_vs || !vs_get_version_product ||
    !vs_initialize || !vs_integrate || !vs_integrate_io || !vs_opt_pause ||
    !vs_read_configuration || !vs_setdef_and_read ||!vs_stop_run ||
    !vs_terminate)
    return vss_dll_problem(dll_fname, "vs_get_api_basic", -2);


  return 0;
}

/* ----------------------------------------------------------------------------
Get basic VS API functions needed to extend a model.
---------------------------------------------------------------------------- */
int vs_get_api_extend (HMODULE dll, char *dll_fname)
{
  if (dll == NULL) 
    return vss_dll_problem(dll_fname, "vs_get_api_extend", -1);

  // Get addresses for VS API functions used to extend a model
  vs_define_import = (_x_vs_define_import)GetProcAddress(dll, "vs_define_import");
  vs_define_output = (_x_vs_define_output)GetProcAddress(dll, "vs_define_output");
  vs_define_parameter = (_x_vs_define_parameter)GetProcAddress(dll, "vs_define_parameter");
  vs_define_units = (_x_vs_define_units)GetProcAddress(dll, "vs_define_units");
  vs_define_variable = (_x_vs_define_variable)GetProcAddress(dll, "vs_define_variable");
  vs_get_var_ptr = (_x_vs_get_var_ptr)GetProcAddress(dll, "vs_get_var_ptr");
  vs_get_var_ptr_int = (_x_vs_get_var_ptr_int)GetProcAddress(dll, "vs_get_var_ptr_int");
  vs_set_units = (_x_vs_set_units)GetProcAddress(dll, "vs_set_units");
  vs_install_calc_func = (_x_vs_install_calc_func)GetProcAddress(dll, "vs_install_calc_func");
  vs_printf = (_x_vs_printf)GetProcAddress(dll, "vs_printf");
  vs_printf_error = (_x_vs_printf_error)GetProcAddress(dll, "vs_printf_error");
  vs_set_sym_int = (_x_vs_set_sym_int)GetProcAddress(dll, "vs_set_sym_int");
  vs_set_sym_real = (_x_vs_set_sym_real)GetProcAddress(dll, "vs_set_sym_real");
  vs_set_sym_attribute = (_x_vs_set_sym_attribute)GetProcAddress(dll, "vs_set_sym_attribute");
  vs_read_next_line = (_x_vs_read_next_line)GetProcAddress(dll, "vs_read_next_line");
  vs_write_to_echo_file = (_x_vs_write_to_echo_file)GetProcAddress(dll, "vs_write_to_echo_file");
  vs_write_header_to_echo_file = (_x_vs_write_header_to_echo_file)GetProcAddress(dll, "vs_write_header_to_echo_file");
  vs_write_f_to_echo_file = (_x_vs_write_f_to_echo_file)GetProcAddress(dll, "vs_write_f_to_echo_file");
  vs_write_i_to_echo_file = (_x_vs_write_i_to_echo_file)GetProcAddress(dll, "vs_write_i_to_echo_file");
  vs_get_sym_attribute = (_x_vs_get_sym_attribute)GetProcAddress(dll, "vs_get_sym_attribute");
  vs_define_parameter_int = (_x_vs_define_parameter_int)GetProcAddress(dll, "vs_define_parameter_int");

  // Check to see if all functions were found
  if (!vs_define_import || !vs_define_output || !vs_define_parameter ||
    !vs_define_units || !vs_define_variable || !vs_get_var_ptr ||
    !vs_get_var_ptr_int || !vs_set_units || !vs_install_calc_func ||
    !vs_printf || !vs_printf_error || !vs_set_sym_int || !vs_set_sym_real ||
    !vs_set_sym_attribute || !vs_read_next_line || !vs_write_to_echo_file ||
    !vs_write_header_to_echo_file || !vs_write_f_to_echo_file ||
    !vs_write_i_to_echo_file || !vs_get_sym_attribute || !vs_define_parameter_int)
    return vss_dll_problem(dll_fname, "vs_get_api_extend", -2);

  return 0;
}

/* ----------------------------------------------------------------------------
Get VS API functions to access road geometry.
---------------------------------------------------------------------------- */
int vs_get_api_road (HMODULE dll, char *dll_fname)
{
  if (dll == NULL) 
    return vss_dll_problem(dll_fname, "vs_get_api_road", -1);

  // Get addresses for VS API functions used to extend a model (since June 2007)
  vs_road_s = (_x_vs_road_s)GetProcAddress(dll, "vs_road_s");
  vs_road_l = (_x_vs_road_l)GetProcAddress(dll, "vs_road_l");
  vs_road_x = (_x_vs_road_x)GetProcAddress(dll, "vs_road_x");
  vs_road_y = (_x_vs_road_y)GetProcAddress(dll, "vs_road_y");
  vs_road_z = (_x_vs_road_z)GetProcAddress(dll, "vs_road_z");
  vs_road_yaw = (_x_vs_road_yaw)GetProcAddress(dll, "vs_road_yaw");
  vs_s_loop = (_x_vs_s_loop)GetProcAddress(dll, "vs_s_loop");
  vs_get_dzds_dzdl = (_x_vs_get_dzds_dzdl)GetProcAddress(dll, "vs_get_dzds_dzdl");
  vs_get_road_start_stop = (_x_vs_get_road_start_stop)GetProcAddress(dll, "vs_get_road_start_stop");
  vs_get_road_xyz = (_x_vs_get_road_xyz)GetProcAddress(dll, "vs_get_road_xyz");
  vs_get_road_contact = (_x_vs_get_road_contact)GetProcAddress(dll, "vs_get_road_contact");
  vs_target_l = (_x_vs_target_l)GetProcAddress(dll, "vs_target_l");

  // Get addresses for VS API functions used to extend a model (since June 2008)
  vs_get_dzds_dzdl_i = (_x_vs_get_dzds_dzdl_i)GetProcAddress(dll, "vs_get_dzds_dzdl_i");
  vs_get_road_contact_sl = (_x_vs_get_road_contact_sl)GetProcAddress(dll, "vs_get_road_contact_sl");
  vs_road_curv_i = (_x_vs_road_curv_i)GetProcAddress(dll, "vs_road_curv_i");
  vs_road_l_i = (_x_vs_road_l_i)GetProcAddress(dll, "vs_road_l_i");
  vs_road_pitch_sl_i = (_x_vs_road_pitch_sl_i)GetProcAddress(dll, "vs_road_pitch_sl_i");
  vs_road_roll_sl_i = (_x_vs_road_roll_sl_i)GetProcAddress(dll, "vs_road_roll_sl_i");
  vs_road_s_i = (_x_vs_road_s_i)GetProcAddress(dll, "vs_road_s_i");
  vs_road_x_i = (_x_vs_road_x_i)GetProcAddress(dll, "vs_road_x_i");
  vs_road_y_i = (_x_vs_road_y_i)GetProcAddress(dll, "vs_road_y_i");
  vs_road_yaw_i = (_x_vs_road_yaw_i)GetProcAddress(dll, "vs_road_yaw_i");
  vs_road_z_i = (_x_vs_road_z_i)GetProcAddress(dll, "vs_road_z_i");
  vs_road_z_sl_i = (_x_vs_road_z_sl_i)GetProcAddress(dll, "vs_road_z_sl_i");

  // Check to see if all functions were found
  if (!vs_road_s || !vs_road_l || !vs_road_x || !vs_road_y || !vs_road_z || 
    !vs_road_yaw || !vs_s_loop || !vs_get_dzds_dzdl || !vs_get_road_start_stop || 
    !vs_get_road_xyz || !vs_get_road_contact || !vs_target_l ||
    !vs_get_dzds_dzdl_i || !vs_get_road_contact_sl || !vs_road_curv_i ||
    !vs_road_l_i || !vs_road_pitch_sl_i || !vs_road_roll_sl_i || !vs_road_s_i ||
    !vs_road_x_i || !vs_road_y_i || !vs_road_yaw_i || !vs_road_z_i ||
    !vs_road_z_sl_i
    )
    return vss_dll_problem(dll_fname, "vs_get_api_road", -2);

  return 0;
}

/* ----------------------------------------------------------------------------
Get VS API functions to install externally defined functions
---------------------------------------------------------------------------- */
int vs_get_api_install_external (HMODULE dll, char *dll_fname)
{
  if (dll == NULL) 
    return vss_dll_problem(dll_fname, "vs_get_api_install_external", -1);

  // Get addresses for VS API functions used to extend a model (since June 2007)
  vs_install_calc_function = (_x_vs_install_calc_function)GetProcAddress(dll, "vs_install_calc_function");
  vs_install_echo_function = (_x_vs_install_echo_function)GetProcAddress(dll, "vs_install_echo_function");
  vs_install_setdef_function = (_x_vs_install_setdef_function)GetProcAddress(dll, "vs_install_setdef_function");
  vs_install_scan_function = (_x_vs_install_scan_function)GetProcAddress(dll, "vs_install_scan_function");
  vs_install_free_function = (_x_vs_install_free_function)GetProcAddress(dll, "vs_install_free_function");
  vs_run = (_x_vs_run)GetProcAddress(dll, "vs_run");

  // Check to see if all functions were found
  if (!vs_install_calc_function || !vs_install_echo_function || !vs_install_setdef_function || 
    !vs_install_scan_function || !vs_install_free_function || !vs_run )
    return vss_dll_problem(dll_fname, "vs_get_api_install_external", -2);

  return 0;
}
#pragma endregion

#endif // VS_API_H