#pragma once

#include "CTimeMgr.h"
/////////////////////////////////////////

#define		PURE =0

#define		WINCX 800
#define		WINCY 600

#define		DEAD    0
#define     NOEVENT 0

#define		VK_MAX	0xff

/////////////////////////////////////////
// 매니저 싱글톤 단축
// - TimeMgr
#define		DT			CTimeMgr::GetInstance()->GetDeltaTime()
#define		GET_TIME(x)	CTimeMgr::GetInstance()->GetDuration(x)
// - KeyMgr
#define		KEY_DOWN   0
#define     KEY_PRESS  0
#define     KEY_UP     0
//

enum class OBJID	{ PLAYER, OBJID_MONSTER, END };
enum class RENDERID	{ PRIORITY, GAMEOBJECT, EFFECT, UI, END };
enum class SCENEID	{ STAGE, END };

/////////////////////////////////////////
// 구조체

typedef struct tagInfo
{
	float fX, fY;
	float fCX, fCY;
} INFO;

typedef struct tagFrame
{
	int iStart;
	int iEnd;
	int iMotion;
	double		  dFrameSpeed;
	LARGE_INTEGER iFrameTime; 
} FRAME;

/////////////////////////////////////////
// 함수

template<typename T>
void SafeDelete(T& p)
{
	if (p)
	{
		delete p;
		p = nullptr;
	}
}

template<typename T>
constexpr int EnumToInt(T e)
{
	return static_cast<int>(e);
}

extern HWND g_hWnd;