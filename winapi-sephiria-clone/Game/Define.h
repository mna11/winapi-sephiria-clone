#pragma once

/////////////////////////////////////////

#define		PURE =0

#define		WINCX 800
#define		WINCY 600

#define		DEAD    1
#define     NOEVENT 0

#define     TILEX   100
#define     TILEY   100 
#define     TILECX  16
#define     TILECY  16

#define		VK_MAX	0xff

/////////////////////////////////////////
// 매니저 싱글톤 단축
// - TimeMgr
#define		DT				CTimeMgr::GetInstance()->GetDeltaTime()
#define     GET_TIME()		CTimeMgr::GetInstance()->GetTime()
#define		GET_DURATION(x)	CTimeMgr::GetInstance()->GetTime(x)
//// - KeyMgr
#define		KEY_PRESS(x)	CKeyMgr::GetInstance()->KeyPress(x)
#define		KEY_DOWN(x)		CKeyMgr::GetInstance()->KeyDown(x)
#define		KEY_UP(x)		CKeyMgr::GetInstance()->KeyUp(x)
#define		KEY_HOLD(x)		CKeyMgr::GetInstance()->KeyHold(x)
//
/////////////////////////////////////////
// 열거체

enum class OBJID	{ PLAYER, OBJID_MONSTER, END };
enum class RENDERID	{ PRIORITY, GAMEOBJECT, EFFECT, UI, END };
enum class SCENEID	{ STAGE, END };

enum class KEY_STATE { NONE, DOWN, HOLD, UP, END };

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

typedef struct tagLinePoint
{
	float fX, fY;

	tagLinePoint() { ZeroMemory(this, sizeof(tagLinePoint)); }
	tagLinePoint(float X, float Y) : fX(X), fY(Y) {}

}LINEPOINT;

typedef struct tagLine
{
	tagLinePoint	tLPoint;
	tagLinePoint	tRPoint;

	tagLine() { ZeroMemory(this, sizeof(tagLine)); }
	tagLine(tagLinePoint& LPoint, tagLinePoint& RPoint)
		: tLPoint(LPoint), tRPoint(RPoint) {
	}
}LINE;

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