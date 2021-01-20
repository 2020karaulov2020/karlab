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
