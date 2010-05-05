#include "vs_function_pointers.h"

// Core functions (from vs_solver_api.c)
#ifdef __cplusplus
extern "C" {
#endif

_x_vs_bar_graph_update vs_bar_graph_update ;
_x_vs_copy_io vs_copy_io ;
_x_vs_error_occurred vs_error_occurred ;
_x_vs_free_all vs_free_all ;
_x_vs_get_error_message vs_get_error_message ;
_x_vs_get_output_message vs_get_output_message ;
_x_vs_get_tstep vs_get_tstep ;
_x_vs_get_version_vs vs_get_version_vs ;
_x_vs_get_version_product vs_get_version_product ;
_x_vs_initialize vs_initialize ;
_x_vs_integrate vs_integrate ;
_x_vs_integrate_io vs_integrate_io ;
_x_vs_opt_pause vs_opt_pause ; 
_x_vs_read_configuration vs_read_configuration ;
_x_vs_setdef_and_read vs_setdef_and_read ;
_x_vs_stop_run vs_stop_run ;
_x_vs_terminate vs_terminate ;
_x_vs_terminate_run vs_terminate_run ;
_x_vs_run vs_run ;


// VS API functions for extending models & adding equations
_x_vs_define_import vs_define_import ;
_x_vs_define_output vs_define_output ;
_x_vs_define_parameter vs_define_parameter ;
_x_vs_define_units vs_define_units ;
_x_vs_define_variable vs_define_variable ;
_x_vs_get_var_ptr vs_get_var_ptr ;
_x_vs_get_var_ptr_int vs_get_var_ptr_int ;
_x_vs_set_units vs_set_units ;
_x_vs_install_calc_func vs_install_calc_func ;
_x_vs_printf vs_printf ;
_x_vs_printf_error vs_printf_error ;
_x_vs_set_sym_int vs_set_sym_int ;
_x_vs_set_sym_real vs_set_sym_real ;
_x_vs_set_sym_attribute vs_set_sym_attribute ;
_x_vs_read_next_line vs_read_next_line ;
_x_vs_write_to_echo_file vs_write_to_echo_file ;
_x_vs_write_header_to_echo_file vs_write_header_to_echo_file ;
_x_vs_write_f_to_echo_file vs_write_f_to_echo_file ;
_x_vs_write_i_to_echo_file vs_write_i_to_echo_file ;
_x_vs_get_sym_attribute vs_get_sym_attribute ;
_x_vs_define_parameter_int vs_define_parameter_int ;


// functions for accessing the 3D road model in CarSim 7.01 and TruckSim 7.0
_x_vs_get_dzds_dzdl vs_get_dzds_dzdl ;
_x_vs_get_road_contact vs_get_road_contact ;
_x_vs_get_road_start_stop vs_get_road_start_stop ;
_x_vs_get_road_xyz vs_get_road_xyz ;
_x_vs_road_l vs_road_l ;
_x_vs_road_s vs_road_s;
_x_vs_road_x vs_road_x;
_x_vs_road_y vs_road_y ;
_x_vs_road_z vs_road_z ;
_x_vs_road_yaw vs_road_yaw ;
_x_vs_s_loop vs_s_loop ;

_x_vs_target_heading vs_target_heading ;  // added by ME
_x_vs_target_l vs_target_l ;


// functions for accessing the 3D road model starting with CarSim 7.1
_x_vs_get_dzds_dzdl_i vs_get_dzds_dzdl_i ;
_x_vs_get_road_contact_sl vs_get_road_contact_sl ;
_x_vs_road_curv_i vs_road_curv_i ;
_x_vs_road_l_i vs_road_l_i ;
_x_vs_road_pitch_sl_i vs_road_pitch_sl_i ;
_x_vs_road_roll_sl_i vs_road_roll_sl_i ;
_x_vs_road_s_i vs_road_s_i ;
_x_vs_road_x_i vs_road_x_i ;
_x_vs_road_y_i vs_road_y_i ;
_x_vs_road_yaw_i vs_road_yaw_i ;
_x_vs_road_z_i vs_road_z_i ;
_x_vs_road_z_sl_i vs_road_z_sl_i ;
_cs_tire cs_tire ;


// installation functions tested Nov 30, 2008
_x_vs_install_calc_function vs_install_calc_function ;
_x_vs_install_echo_function vs_install_echo_function ;
_x_vs_install_setdef_function vs_install_setdef_function ;
_x_vs_install_scan_function vs_install_scan_function ;
_x_vs_install_free_function vs_install_free_function ;

#ifdef __cplusplus
} // extern "C"
#endif