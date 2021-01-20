#if defined _karaulovlab_included
	#endinput
#endif
#define _karaulovlab_included

#include <amxmodx>

#pragma reqlib karaulovlab
#if !defined AMXMODX_NOAUTOLOAD
	#pragma loadlib karaulovlab
#endif 

native test_download_speed(const id);
native print_sys_info(const id);
native test_regex_req(const id);
