#pragma once


#include <string>
using namespace std;

#ifdef YZWLRCSERVERSO_EXPORTS
#define YZWLRCSERVERSO_API __declspec(dllexport)
#else
#define YZWLRCSERVERSO_API __declspec(dllimport)
#pragma comment(lib, "yzwlRCServerSo.lib")
#endif

typedef enum _ServerType
{
	eCSServer = 0, // 一对多模式
}ServerType;


typedef bool(__stdcall *facePhotoDeal)(char*/*[in]*/ szcardInfoJson, char*/*[out]*/ szerrInfo, 
	char*/*[out]*/ szuserData, float&/*[out]*/ fcompareValue);

bool __stdcall yzwlRS_init(facePhotoDeal cb, bool bcanFace, ServerType eType, bool blocal=false, char* szDomain = "com.domain");
void __stdcall yzwlRS_uninit();
