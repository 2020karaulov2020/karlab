#include <amxmodx>
#include <karlab>

public plugin_init() {
	register_plugin("Karlab tester", "1.0", "BADMAN");
	register_concmd("test_speed","InitDownload", ADMIN_RCON, "InitDownload")
	register_concmd("test_info","InitTestInfo", ADMIN_RCON, "InitTestInfo")
	register_concmd("test_regex","InitTestRegex", ADMIN_RCON, "InitTestInfo")
}

public InitDownload(Index)
{
	test_download_speed(Index )
}

public InitTestInfo(Index)
{
	print_sys_info(Index )
}

public InitTestRegex(Index)
{
	test_regex_req(Index )
}