#ifndef VS_FUNCTION_POINTERS_H
#define VS_FUNCTION_POINTERS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "vs_deftypes.h"

// Core functions (from vs_solver_api.c)
typedef int     (*_x_vs_bar_graph_update) (int *);
extern _x_vs_bar_graph_update vs_bar_graph_update ;

typedef void    (*_x_vs_copy_io) (vs_real *imports, vs_real *exports);
extern _x_vs_copy_io vs_copy_io ;

typedef vs_bool (*_x_vs_error_occurred) (void);
extern _x_vs_error_occurred vs_error_occurred ;

typedef void    (*_x_vs_free_all) (void);
extern _x_vs_free_all vs_free_all ;

typedef char    *(*_x_vs_get_error_message) (void);
extern _x_vs_get_error_message vs_get_error_message ;

typedef char    *(*_x_vs_get_output_message) (void);
extern _x_vs_get_output_message vs_get_output_message ;

typedef vs_real (*_x_vs_get_tstep) (void);
extern _x_vs_get_tstep vs_get_tstep ;

typedef char    *(*_x_vs_get_version_vs) (void);
extern _x_vs_get_version_vs vs_get_version_vs ;

typedef char    *(*_x_vs_get_version_product) (void);
extern _x_vs_get_version_product vs_get_version_product ;

typedef void    (*_x_vs_initialize) (vs_real t, void (*ext_calc) (vs_real, vs_ext_loc), void (*ext_echo) (vs_ext_loc));
extern _x_vs_initialize vs_initialize ;

typedef int    (*_x_vs_integrate) (vs_real *t, void (*ext_eq_in) (vs_real, vs_ext_loc));
extern _x_vs_integrate vs_integrate ;

typedef int    (*_x_vs_integrate_io) (vs_real t, vs_real *imports, vs_real *exports);
extern _x_vs_integrate_io vs_integrate_io ;

typedef vs_bool (*_x_vs_opt_pause)(void);
extern _x_vs_opt_pause vs_opt_pause ; 

typedef void   (*_x_vs_read_configuration) (const char *simfile, int *n_import, int *n_export, vs_real *tstart, vs_real *tstop, vs_real *tstep);
extern _x_vs_read_configuration vs_read_configuration ;

typedef vs_real (*_x_vs_setdef_and_read) (const char *simfile, void (*ext_setdef) (void), int (*_ext_scan) (char *, char *));
extern _x_vs_setdef_and_read vs_setdef_and_read ;

typedef int    (*_x_vs_stop_run) (void);
extern _x_vs_stop_run vs_stop_run ;

typedef void   (*_x_vs_terminate) (vs_real t, void (*ext_echo) (vs_ext_loc));
extern _x_vs_terminate vs_terminate ;

typedef void   (*_x_vs_terminate_run) (vs_real t);
extern _x_vs_terminate_run vs_terminate_run ;

typedef int    (*_x_vs_run) (char *simfile);
extern _x_vs_run vs_run ;

// VS API functions for extending models & adding equations
typedef int     (*_x_vs_define_import) (char *keyword, char *desc, vs_real *real, char *);
extern _x_vs_define_import vs_define_import ;

typedef int     (*_x_vs_define_output) (char *shortname, char *longname, vs_real *real, char *);
extern _x_vs_define_output vs_define_output ;

typedef int     (*_x_vs_define_parameter) (char *keyword, char *desc, vs_real *, char *);
extern _x_vs_define_parameter vs_define_parameter ;

typedef void    (*_x_vs_define_units) (char *desc, vs_real gain);
extern _x_vs_define_units vs_define_units ;

typedef int     (*_x_vs_define_variable) (char *keyword, char *desc, vs_real *);    
extern _x_vs_define_variable vs_define_variable ;

typedef vs_real *(*_x_vs_get_var_ptr) (char *keyword);                                        
extern _x_vs_get_var_ptr vs_get_var_ptr ;

typedef int     *(*_x_vs_get_var_ptr_int) (char *keyword);
extern _x_vs_get_var_ptr_int vs_get_var_ptr_int ;

typedef void    (*_x_vs_set_units) (char *var_keyword, char *units_keyword);
extern _x_vs_set_units vs_set_units ;

typedef void    (*_x_vs_install_calc_func) (char *name, void *func);
extern _x_vs_install_calc_func vs_install_calc_func ;

typedef void    (*_x_vs_printf) (const char *format, ...);
extern _x_vs_printf vs_printf ;

typedef void    (*_x_vs_printf_error) (const char *format, ...);
extern _x_vs_printf_error vs_printf_error ;

typedef int     (*_x_vs_set_sym_int) (int id, vs_sym_attr_type dataType, int value);
extern _x_vs_set_sym_int vs_set_sym_int ;

typedef int     (*_x_vs_set_sym_real) (int id, vs_sym_attr_type dataType, vs_real value);
extern _x_vs_set_sym_real vs_set_sym_real ;

typedef int     (*_x_vs_set_sym_attribute) (int id, vs_sym_attr_type type, const void *att);
extern _x_vs_set_sym_attribute vs_set_sym_attribute ;

typedef void    (*_x_vs_read_next_line) (char *buffer, int n);
extern _x_vs_read_next_line vs_read_next_line ;

typedef void    (*_x_vs_write_to_echo_file) (char *buffer);
extern _x_vs_write_to_echo_file vs_write_to_echo_file ;

typedef void    (*_x_vs_write_header_to_echo_file) (char *buffer);
extern _x_vs_write_header_to_echo_file vs_write_header_to_echo_file ;

typedef void    (*_x_vs_write_f_to_echo_file) (char *key, vs_real , char *doc);
extern _x_vs_write_f_to_echo_file vs_write_f_to_echo_file ;

typedef void    (*_x_vs_write_i_to_echo_file) (char *key, int , char *doc);
extern _x_vs_write_i_to_echo_file vs_write_i_to_echo_file ;

typedef int     (*_x_vs_get_sym_attribute) (int id, vs_sym_attr_type type, void **att);
extern _x_vs_get_sym_attribute vs_get_sym_attribute ;

typedef int     (*_x_vs_define_parameter_int) (char *keyword, char *desc, int *);
extern _x_vs_define_parameter_int vs_define_parameter_int ;


// functions for accessing the 3D road model in CarSim 7.01 and TruckSim 7.0
typedef void    (*_x_vs_get_dzds_dzdl) (vs_real s, vs_real l, vs_real *dzds, vs_real *dzdl);
extern _x_vs_get_dzds_dzdl vs_get_dzds_dzdl ;

typedef void    (*_x_vs_get_road_contact) (vs_real y, vs_real x, int inst, vs_real *z, vs_real *dzdy, vs_real *dzdx, vs_real *mu);
extern _x_vs_get_road_contact vs_get_road_contact ;

typedef void    (*_x_vs_get_road_start_stop) (vs_real *start, vs_real *stop);
extern _x_vs_get_road_start_stop vs_get_road_start_stop ;

typedef void    (*_x_vs_get_road_xyz) (vs_real s, vs_real l, vs_real *x, vs_real *y, vs_real *z);
extern _x_vs_get_road_xyz vs_get_road_xyz ;

typedef vs_real (*_x_vs_road_l) (vs_real x, vs_real y);
extern _x_vs_road_l vs_road_l ;

typedef vs_real (*_x_vs_road_s) (vs_real x, vs_real y);
extern _x_vs_road_s vs_road_s;

typedef vs_real (*_x_vs_road_x) (vs_real s);
extern _x_vs_road_x vs_road_x;

typedef vs_real (*_x_vs_road_y) (vs_real s);
extern _x_vs_road_y vs_road_y ;

typedef vs_real (*_x_vs_road_z) (vs_real x, vs_real y);
extern _x_vs_road_z vs_road_z ;

typedef vs_real (*_x_vs_road_yaw) (vs_real sta, vs_real direction);
extern _x_vs_road_yaw vs_road_yaw ;

typedef vs_real (*_x_vs_s_loop) (vs_real s);
extern _x_vs_s_loop vs_s_loop ;

typedef vs_real (*_x_vs_target_l) (vs_real s);
extern _x_vs_target_l vs_target_l ;

// functions for accessing the 3D road model starting with CarSim 7.1
typedef void    (*_x_vs_get_dzds_dzdl_i) (vs_real s, vs_real l, vs_real *dzds, vs_real *dzdl, vs_real inst);
extern _x_vs_get_dzds_dzdl_i vs_get_dzds_dzdl_i ;

typedef void    (*_x_vs_get_road_contact_sl) (vs_real s, vs_real l, int inst, vs_real *z, vs_real *dzds, vs_real *dzdl, vs_real *mu);
extern _x_vs_get_road_contact_sl vs_get_road_contact_sl ;

typedef vs_real (*_x_vs_road_curv_i) (vs_real s, vs_real inst);
extern _x_vs_road_curv_i vs_road_curv_i ;

typedef vs_real (*_x_vs_road_l_i) (vs_real x, vs_real y, vs_real inst);
extern _x_vs_road_l_i vs_road_l_i ;

typedef vs_real (*_x_vs_road_pitch_sl_i) (vs_real s, vs_real l, vs_real yaw, vs_real inst);
extern _x_vs_road_pitch_sl_i vs_road_pitch_sl_i ;

typedef vs_real (*_x_vs_road_roll_sl_i) (vs_real s, vs_real l, vs_real yaw, vs_real inst);
extern _x_vs_road_roll_sl_i vs_road_roll_sl_i ;

typedef vs_real (*_x_vs_road_s_i) (vs_real x, vs_real y, vs_real inst);
extern _x_vs_road_s_i vs_road_s_i ;

typedef vs_real (*_x_vs_road_x_i) (vs_real sy, vs_real inst);
extern _x_vs_road_x_i vs_road_x_i ;

typedef vs_real (*_x_vs_road_y_i) (vs_real sy, vs_real inst);
extern _x_vs_road_y_i vs_road_y_i ;

typedef vs_real (*_x_vs_road_yaw_i) (vs_real sta, vs_real directiony, vs_real inst);
extern _x_vs_road_yaw_i vs_road_yaw_i ;

typedef vs_real (*_x_vs_road_z_i) (vs_real x, vs_real yy, vs_real inst);
extern _x_vs_road_z_i vs_road_z_i ;

typedef vs_real (*_x_vs_road_z_sl_i) (vs_real s, vs_real l, vs_real inst);
extern _x_vs_road_z_sl_i vs_road_z_sl_i ;

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
extern _cs_tire cs_tire ;

// installation functions tested Nov 30, 2008
typedef void (*_x_vs_install_calc_function) (void (*calc) (vs_real time, vs_ext_loc where));
extern _x_vs_install_calc_function vs_install_calc_function ;

typedef void (*_x_vs_install_echo_function) (void (*echo) (vs_ext_loc where));
extern _x_vs_install_echo_function vs_install_echo_function ;

typedef void (*_x_vs_install_setdef_function) (void (*setdef) (void));
extern _x_vs_install_setdef_function vs_install_setdef_function ;

typedef void (*_x_vs_install_scan_function) (vs_bool (*scan) (char *, char *));
extern _x_vs_install_scan_function vs_install_scan_function ;

typedef void (*_x_vs_install_free_function) (void (*free) (void));
extern _x_vs_install_free_function vs_install_free_function ;

#ifdef __cplusplus
}// extern "C"
#endif

#endif