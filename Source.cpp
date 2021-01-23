#ifdef WIN32
#define HAVE_STDINT_H
#endif

//#ifndef WIN32
//#include "force_link_glibc_2.13.h"
//#endif 

#ifndef WIN32
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#endif

#include "httplib.h"

#include "amxxmodule.h"
#include "Header.h"


#include <fstream>


std::thread g_hSpeedTestThread;
std::thread g_hMiniServerThread;

int g_iSpeedTestPart = 0;
float g_flSpeedTestResult;
int g_hPlayerSpeedCaller;
int g_hTextMsg = 0;

bool IsPlayer(int Player)
{
	return Player >= 1 && Player <= gpGlobals->maxClients;
}

bool IsPlayerSafe(int Player)
{
	return MF_IsPlayerIngame(Player);
}

void UTIL_TextMsg(edict_t* pPlayer, char* message)
{
	if (g_hTextMsg == 0)
		g_hTextMsg = GET_USER_MSG_ID(PLID, "TextMsg", NULL);

	MESSAGE_BEGIN(MSG_ONE, g_hTextMsg, NULL, pPlayer);
	WRITE_BYTE(HUD_PRINTCONSOLE); // 1 = console, 2 = console, 3 = chat, 4 = center
	WRITE_STRING(message);
	MESSAGE_END();

	MESSAGE_BEGIN(MSG_ONE, g_hTextMsg, NULL, pPlayer);
	WRITE_BYTE(HUD_PRINTTALK); // 1 = console, 2 = console, 3 = chat, 4 = center
	WRITE_STRING(message);
	MESSAGE_END();
	/*
	The byte above seems to use these:
	#define HUD_PRINTNOTIFY		1
	#define HUD_PRINTCONSOLE	2
	#define HUD_PRINTTALK		3
	#define HUD_PRINTCENTER		4
	However both 1 and 2 seems to go to console with Steam CS.
	*/
}

void UTIL_TextMsg(int iPlayer, char* message)
{
	if (!IsPlayer(iPlayer))
	{
		return;
	}
	else if (!IsPlayerSafe(iPlayer))
	{
		return;
	}
	edict_t* pPlayer = MF_GetPlayerEdict(iPlayer);

	UTIL_TextMsg(pPlayer, message);
}



void download_speed_thread()
{
	while (true && g_iSpeedTestPart != -1)
	{
		if (g_iSpeedTestPart == 1)
		{
			try
			{
				g_iSpeedTestPart = 3;
				httplib::Client cli("speedtest.hostkey.ru", 80);
				using namespace std::chrono;
				g_iSpeedTestPart = 4;
				high_resolution_clock::time_point t1 = high_resolution_clock::now();
				g_iSpeedTestPart = 5;
				cli.set_connection_timeout(5);
				g_iSpeedTestPart = 6;
				auto res = cli.Get("/files/100mb.bin", &g_iSpeedTestPart);
				g_iSpeedTestPart = 7;
				high_resolution_clock::time_point t2 = high_resolution_clock::now();
				g_iSpeedTestPart = 8;

				duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
				g_iSpeedTestPart = 9;

				g_flSpeedTestResult = time_span.count();

				g_iSpeedTestPart = 2;
			}
			catch (...)
			{
				
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

void StartFrame(void)
{
	if (g_iSpeedTestPart >= 2)
	{
		if (IsPlayerSafe(g_hPlayerSpeedCaller) && g_iSpeedTestPart == 2)
		{
			char tmpSpeedResult[256];
			snprintf(tmpSpeedResult, sizeof(tmpSpeedResult), "Result: download 100mb in %f seconds.", g_flSpeedTestResult);
			UTIL_TextMsg(g_hPlayerSpeedCaller, tmpSpeedResult);
		}
		else if (IsPlayerSafe(g_hPlayerSpeedCaller))
		{
			char tmpSpeedResult[256];
			snprintf(tmpSpeedResult, sizeof(tmpSpeedResult),"%s - %i", "Result: crash.", g_iSpeedTestPart);
			UTIL_TextMsg(g_hPlayerSpeedCaller, tmpSpeedResult);
		}
		g_iSpeedTestPart = 0;
	}
}



static cell AMX_NATIVE_CALL test_download_speed(AMX* amx, cell* params) // 1 pararam
{
	int index = params[1];
	if (IsPlayer(index))
	{
		if (!IsPlayerSafe(index))
		{
			MF_LogError(amx, AMX_ERR_NATIVE, "Cannot access player %d, it's not safe enough!", index);
			return 0;
		}
		UTIL_TextMsg(index, "Start download speed test. Please wait for finish.");
		if (g_iSpeedTestPart <= 0)
		{
			g_hPlayerSpeedCaller = index;
			g_iSpeedTestPart = 1;
		}
	}
	else
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Player %d is not valid!", index);
	}
	return 0;
}



static cell AMX_NATIVE_CALL print_sys_info(AMX* amx, cell* params) // 1 pararam
{
	int index = params[1];
	if (IsPlayer(index))
	{
		if (!IsPlayerSafe(index))
		{
			MF_LogError(amx, AMX_ERR_NATIVE, "Cannot access player %d, it's not safe enough!", index);
			return 0;
		}
		char tmpSysInfoPrint[256];
#ifndef WIN32
		/* Conversion constants. */
		const long minute = 60;
		const long hour = minute * 60;
		const long day = hour * 24;
		const double megabyte = 1024 * 1024;
		/* Obtain system statistics. */
		struct sysinfo si;
		sysinfo(&si);


		snprintf(tmpSysInfoPrint, sizeof(tmpSysInfoPrint), "Up : %ld days, %ld:%02ld:%02ld. Ram/Free: %5.1f MB / %5.1f MB. CPU's:  %d", si.uptime / day, (si.uptime % day) / hour,
			(si.uptime % hour) / minute, si.uptime % minute, si.totalram / megabyte, si.freeram / megabyte, si.procs);
		UTIL_TextMsg(index, tmpSysInfoPrint);

		struct utsname sysinfo;
		uname(&sysinfo);

		std::string tmpSysString = "Sys: " + std::string(sysinfo.sysname) +
			". Host: " + std::string(sysinfo.nodename) +
			". Release: " + std::string(sysinfo.release);

		snprintf(tmpSysInfoPrint, sizeof(tmpSysInfoPrint), "%s", tmpSysString.c_str());
		UTIL_TextMsg(index, tmpSysInfoPrint);

		tmpSysString = 
			". Build: " + std::string(sysinfo.version) +
			". Arch: " + std::string(sysinfo.machine) +
			". Name: " + std::string(sysinfo.domainname);
		snprintf(tmpSysInfoPrint, sizeof(tmpSysInfoPrint), "%s", tmpSysString.c_str());
		UTIL_TextMsg(index, tmpSysInfoPrint);
#else 
		/*SYSTEM_INFO siSysInfo;
		GetSystemInfo(&siSysInfo);
		*/
		UTIL_TextMsg(index, "Win32 not supported!");
#endif
	}
	else
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Player %d is not valid!", index);
	}
	return 0;
}

static cell AMX_NATIVE_CALL test_regex_req(AMX* amx, cell* params) // 1 pararam
{
	int index = params[1];
	if (IsPlayer(index))
	{
		if (!IsPlayerSafe(index))
		{
			MF_LogError(amx, AMX_ERR_NATIVE, "Cannot access player %d, it's not safe enough!", index);
			return 0;
		}
		char tmpSysInfoPrint[256];
		int error = 0;
		try
		{
			error = 1;

			std::regex* re = new std::regex("(HTTP/1\\.[01]) (\\d{3}) (.*?)\r\n");

			error = 2;
			std::cmatch m;
			error = 3;

			if (std::regex_match("HTTP/1.1 200 OK\r\n", m, *re)) {
				error = 4;
				auto version = std::string(m[1]);
				auto status = std::stoi(std::string(m[2]));
				auto reason = std::string(m[3]);
				error = 5;
				snprintf(tmpSysInfoPrint, sizeof(tmpSysInfoPrint), "%s:%i:%s", version.c_str(), status, reason.c_str());
				error = 6;
				UTIL_TextMsg(index, tmpSysInfoPrint);
				error = 7;
			}
			else
				UTIL_TextMsg(index, "regex_match: false");
		}
		catch (...)
		{
			snprintf(tmpSysInfoPrint, sizeof(tmpSysInfoPrint), "%s:%i", "Fatal error", error);
			UTIL_TextMsg(index, tmpSysInfoPrint);
		}
	}
	return 0;
}

AMX_NATIVE_INFO my_Natives[] =
{
	{"test_download_speed",	test_download_speed},
	{"print_sys_info",	print_sys_info},
	{"test_regex_req",	test_regex_req},
	{NULL,			NULL},
};

void OnAmxxAttach() // Server start
{
	MF_AddNatives(my_Natives);
	g_hSpeedTestThread = std::thread(download_speed_thread);
	//g_hMiniServerThread 
}

void OnAmxxDetach() // Server stop
{
	g_hTextMsg = 0;
	g_iSpeedTestPart = -1;
}

