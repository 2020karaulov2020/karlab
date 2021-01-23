#if defined _karlab_included
	#endinput
#endif
#define _karlab_included

#include <amxmodx>

#pragma reqlib karlab
#if !defined AMXMODX_NOAUTOLOAD
	#pragma loadlib karlab
#endif 

native test_download_speed(const id);
native print_sys_info(const id);
native test_regex_req(const id);

/**
 * Start mini server
 *
 * @param port    mini server port
 *
 * @return  none
 */
native init_mini_server(const port);
/**
 * Stop mini server
 *
 * @return  none
 */
native stop_mini_server();



/**
 * Called when server got any request
 *
 * @param ip   request REMOTE IP
 * @param req   request STRING
 *
 * @return  none
 */

forward mini_server_req(ip[],req[]);


/**
 * Need call for every mini_server_req with same IP
 *
 * @param ip   same as mini_server_req
 * @param res	response string for mini_server_req
 *
 * @return  none
 */
native mini_server_res(const ip[],const res[]);